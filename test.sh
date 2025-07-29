#!/bin/bash

API_URL="http://127.0.0.1:8080/health"   # তোমার API URL
TOTAL_REQUESTS=10000                     # মোট রিকোয়েস্ট সংখ্যা
CONCURRENCY=100                          # একসাথে চলবে এমন রিকোয়েস্ট সংখ্যা
REQUESTS_PER_THREAD=$((TOTAL_REQUESTS / CONCURRENCY))

echo "🚀 Load Testing $TOTAL_REQUESTS requests with $CONCURRENCY concurrency on $API_URL"

# Latency log
mkdir -p logs
rm -f logs/*.txt

latency_check() {
    for ((i = 0; i < $1; i++)); do
        start=$(date +%s%N)
        curl -s -o /dev/null -w "%{http_code}" "$API_URL" > /dev/null
        end=$(date +%s%N)
        duration=$((end - start)) # nanoseconds
        echo "$duration" >> logs/latency_$2.txt
    done
}

export -f latency_check
export API_URL

# Start parallel requests
seq 0 $((CONCURRENCY - 1)) | parallel -j $CONCURRENCY latency_check $REQUESTS_PER_THREAD {}

echo "✅ All requests done. Processing latency..."

# Convert nanoseconds to readable time
cat logs/latency_*.txt | sort -n > logs/all_latency_ns.txt

echo ""
echo "📊 Latency Summary:"

# Min latency
min=$(awk 'NR==1 {print $1}' logs/all_latency_ns.txt)
echo "Min:    $(awk -v ns=$min 'BEGIN { printf "%.3f µs | %.3f ms | %.6f s\n", ns/1000, ns/1000000, ns/1000000000 }')"

# Median latency
median_line=$(wc -l < logs/all_latency_ns.txt)
median_line=$(( (median_line + 1) / 2 ))
median=$(awk -v line=$median_line 'NR==line {print $1}' logs/all_latency_ns.txt)
echo "Median: $(awk -v ns=$median 'BEGIN { printf "%.3f µs | %.3f ms | %.6f s\n", ns/1000, ns/1000000, ns/1000000000 }')"

# Max latency
max=$(awk 'END {print $1}' logs/all_latency_ns.txt)
echo "Max:    $(awk -v ns=$max 'BEGIN { printf "%.3f µs | %.3f ms | %.6f s\n", ns/1000, ns/1000000, ns/1000000000 }')"
