# GPLANG: Count 1 to 1 Million - Performance Demo
# Demonstrates: .gp → IR → Assembly → .o → .bin compilation pipeline

import std.time

func main():
    print("🚀 GPLANG Count Performance Test")
    print("Counting from 1 to 1,000,000...")
    
    var start_time = Time.now()
    
    for i in range(1, 1000001):
        if i % 100000 == 0:
            print("Count: " + str(i))
    
    var end_time = Time.now()
    var total_time = end_time - start_time
    
    print("✅ COMPLETED!")
    print("Total time: " + str(total_time))
    print("Operations/sec: " + str(int(1000000.0 / total_time.seconds())))
    
    return 0
