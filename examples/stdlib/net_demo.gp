# GPLANG Network Module Demo
# Demonstrates networking capabilities

import net

func main():
    print("🌐 GPLANG Network Module Demo")
    print("=============================")
    print("")
    
    # Network Information
    print("📡 Network Information:")
    var local_ip = net.local_ip()
    var hostname = net.hostname()
    
    print("Local IP: " + local_ip)
    print("Hostname: " + hostname)
    print("")
    
    # DNS Resolution
    print("🔍 DNS Resolution:")
    var google_ip = net.resolve("google.com")
    var github_ip = net.resolve("github.com")
    
    if google_ip:
        print("google.com resolves to: " + google_ip)
    else:
        print("❌ Failed to resolve google.com")
    
    if github_ip:
        print("github.com resolves to: " + github_ip)
    else:
        print("❌ Failed to resolve github.com")
    
    print("")
    
    # Network Testing
    print("🏓 Network Testing:")
    print("Pinging google.com...")
    var ping_result = net.ping("google.com")
    if ping_result:
        print("✅ google.com is reachable")
        
        var ping_time = net.ping_time("google.com")
        if ping_time > 0:
            print("Ping time: " + str(ping_time) + " ms")
    else:
        print("❌ google.com is not reachable")
    
    print("")
    
    # Port Scanning
    print("🔌 Port Scanning:")
    var hosts_to_scan = ["google.com", "github.com", "localhost"]
    var ports_to_scan = [80, 443, 22, 3000]
    
    for host in hosts_to_scan:
        print("Scanning " + host + ":")
        
        for port in ports_to_scan:
            var is_open = net.port_scan(host, port)
            var status = is_open ? "OPEN" : "CLOSED"
            print("  Port " + str(port) + ": " + status)
        
        print("")
    
    # HTTP Requests
    print("🌍 HTTP Requests:")
    
    # Simple GET request
    print("Making GET request to httpbin.org...")
    var response = net.http_get("http://httpbin.org/get")
    
    if response:
        var status = net.http_response_status(response)
        var body = net.http_response_body(response)
        
        print("Status Code: " + str(status))
        print("Response Body: " + body)
        
        net.http_response_free(response)
    else:
        print("❌ Failed to make HTTP request")
    
    print("")
    
    # POST Request with JSON data
    print("Making POST request with JSON data...")
    var json_data = '{"name": "GPLANG", "version": "1.0.0", "type": "programming_language"}'
    var post_response = net.http_post("http://httpbin.org/post", json_data)
    
    if post_response:
        var post_status = net.http_response_status(post_response)
        var post_body = net.http_response_body(post_response)
        
        print("POST Status Code: " + str(post_status))
        print("POST Response: " + post_body)
        
        net.http_response_free(post_response)
    else:
        print("❌ Failed to make POST request")
    
    print("")
    
    # URL Utilities
    print("🔗 URL Utilities:")
    var test_url = "https://api.github.com/users/octocat?tab=repositories"
    
    var protocol = ""
    var host = ""
    var port = 0
    var path = ""
    
    var parse_result = net.url_parse(test_url, protocol, host, port, path)
    if parse_result == 0:
        print("URL: " + test_url)
        print("Protocol: " + protocol)
        print("Host: " + host)
        print("Port: " + str(port))
        print("Path: " + path)
    else:
        print("❌ Failed to parse URL")
    
    print("")
    
    # URL Encoding/Decoding
    var original_text = "Hello World! This has spaces & special chars"
    var encoded = net.url_encode(original_text)
    var decoded = net.url_decode(encoded)
    
    print("Original: " + original_text)
    print("Encoded: " + encoded)
    print("Decoded: " + decoded)
    print("")
    
    # TCP Socket Example
    print("🔌 TCP Socket Example:")
    print("Attempting to connect to google.com:80...")
    
    var socket = net.tcp_connect("google.com", 80)
    if socket:
        print("✅ Connected to google.com:80")
        
        # Send HTTP request
        var http_request = "GET / HTTP/1.1\r\nHost: google.com\r\nConnection: close\r\n\r\n"
        var send_result = net.tcp_send(socket, http_request)
        
        if send_result > 0:
            print("✅ Sent HTTP request (" + str(send_result) + " bytes)")
            
            # Receive response
            var response_data = net.tcp_receive(socket)
            if response_data:
                print("✅ Received response:")
                print("Response length: " + str(len(response_data)) + " bytes")
                
                # Show first 200 characters
                var preview = response_data
                if len(preview) > 200:
                    preview = preview[0:200] + "..."
                
                print("Response preview:")
                print(preview)
            else:
                print("❌ Failed to receive response")
        else:
            print("❌ Failed to send request")
        
        net.tcp_close(socket)
        print("✅ Connection closed")
    else:
        print("❌ Failed to connect to google.com:80")
    
    print("")
    
    # Network Interface Information
    print("🖧  Network Interfaces:")
    var interfaces = net.get_interfaces()
    if interfaces:
        var interface_count = net.interface_count(interfaces)
        print("Found " + str(interface_count) + " network interfaces:")
        
        for i in range(0, interface_count):
            var interface = net.get_interface(interfaces, i)
            if interface:
                print("Interface " + str(i + 1) + ":")
                print("  Name: " + interface.name)
                print("  IP: " + interface.ip_address)
                print("  Status: " + (interface.is_up ? "UP" : "DOWN"))
                print("  Type: " + (interface.is_loopback ? "Loopback" : "Physical"))
        
        net.interfaces_free(interfaces)
    else:
        print("❌ Failed to get network interfaces")
    
    print("")
    
    # Connection Test
    print("🌐 Connection Test:")
    var is_connected = net.is_connected()
    if is_connected:
        print("✅ Internet connection is available")
        
        # Speed test (simplified)
        print("Running basic speed test...")
        var download_speed = net.download_speed_test("http://httpbin.org/bytes/1024")
        if download_speed > 0:
            print("Download speed: " + str(download_speed) + " KB/s")
        else:
            print("❌ Speed test failed")
    else:
        print("❌ No internet connection detected")
    
    print("")
    
    # Security Information
    print("🔒 Security Information:")
    var ssl_supported = net.is_ssl_supported()
    if ssl_supported:
        print("✅ SSL/TLS is supported")
        
        # Try HTTPS request
        print("Making HTTPS request...")
        var https_response = net.https_get("https://httpbin.org/get")
        if https_response:
            var https_status = net.http_response_status(https_response)
            print("HTTPS Status: " + str(https_status))
            net.http_response_free(https_response)
        else:
            print("❌ HTTPS request failed")
    else:
        print("❌ SSL/TLS is not supported")
    
    print("")
    print("✅ Network Module Demo Complete!")
    
    return 0
