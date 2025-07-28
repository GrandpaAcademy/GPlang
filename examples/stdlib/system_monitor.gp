# GPLANG System Monitor
# A comprehensive example using all standard library modules
# Monitors system resources and saves data to JSON

import os
import fs
import json
import net

func collect_system_info():
    print("📊 Collecting System Information...")
    
    var system_info = json.object()
    
    # OS Information
    var os_info = json.object()
    json.object_set_string(os_info, "name", os.name())
    json.object_set_string(os_info, "version", os.version())
    json.object_set_string(os_info, "architecture", os.architecture())
    json.object_set_string(os_info, "hostname", os.hostname())
    json.object_set_string(os_info, "username", os.username())
    json.object_set_number(os_info, "timestamp", os.get_timestamp())
    json.object_set_string(os_info, "datetime", os.get_datetime_string())
    
    json.object_set_object(system_info, "os", os_info)
    
    # Process Information
    var process_info = json.object()
    json.object_set_number(process_info, "pid", os.get_pid())
    json.object_set_number(process_info, "ppid", os.get_ppid())
    json.object_set_bool(process_info, "is_admin", os.is_admin())
    json.object_set_bool(process_info, "is_64bit", os.is_64bit())
    
    json.object_set_object(system_info, "process", process_info)
    
    # System Resources
    var resources = json.object()
    var total_memory = os.get_total_memory()
    var free_memory = os.get_free_memory()
    var used_memory = os.get_used_memory()
    var cpu_count = os.get_cpu_count()
    var uptime = os.get_uptime()
    
    if total_memory > 0:
        json.object_set_number(resources, "total_memory_gb", total_memory / (1024 * 1024 * 1024))
    
    if free_memory > 0:
        json.object_set_number(resources, "free_memory_gb", free_memory / (1024 * 1024 * 1024))
    
    if used_memory > 0:
        json.object_set_number(resources, "used_memory_gb", used_memory / (1024 * 1024 * 1024))
        var memory_usage = (used_memory * 100.0) / total_memory
        json.object_set_number(resources, "memory_usage_percent", memory_usage)
    
    if cpu_count > 0:
        json.object_set_number(resources, "cpu_cores", cpu_count)
    
    if uptime > 0:
        json.object_set_number(resources, "uptime_hours", uptime / 3600.0)
    
    json.object_set_object(system_info, "resources", resources)
    
    return system_info

func collect_network_info():
    print("🌐 Collecting Network Information...")
    
    var network_info = json.object()
    
    # Basic network info
    var local_ip = net.local_ip()
    var hostname = net.hostname()
    
    json.object_set_string(network_info, "local_ip", local_ip)
    json.object_set_string(network_info, "hostname", hostname)
    json.object_set_bool(network_info, "is_connected", net.is_connected())
    json.object_set_bool(network_info, "ssl_supported", net.is_ssl_supported())
    
    # Test connectivity to common services
    var connectivity = json.array()
    
    var test_hosts = ["google.com", "github.com", "stackoverflow.com"]
    for host in test_hosts:
        var host_test = json.object()
        json.object_set_string(host_test, "host", host)
        
        var ping_result = net.ping(host)
        json.object_set_bool(host_test, "reachable", ping_result)
        
        if ping_result:
            var ping_time = net.ping_time(host)
            if ping_time > 0:
                json.object_set_number(host_test, "ping_time_ms", ping_time)
        
        var resolved_ip = net.resolve(host)
        if resolved_ip:
            json.object_set_string(host_test, "ip_address", resolved_ip)
        
        json.array_append_object(connectivity, host_test)
    
    json.object_set_array(network_info, "connectivity_tests", connectivity)
    
    return network_info

func collect_file_system_info():
    print("📁 Collecting File System Information...")
    
    var fs_info = json.object()
    
    # Current directory info
    var current_dir = fs.get_current_dir()
    json.object_set_string(fs_info, "current_directory", current_dir)
    
    # File system statistics
    var fs_stats = fs.get_fs_info(".")
    if fs_stats:
        var fs_details = json.object()
        json.object_set_string(fs_details, "mount_point", fs_stats.mount_point)
        json.object_set_string(fs_details, "filesystem", fs_stats.filesystem)
        json.object_set_number(fs_details, "total_space_gb", fs_stats.total_space / (1024 * 1024 * 1024))
        json.object_set_number(fs_details, "free_space_gb", fs_stats.free_space / (1024 * 1024 * 1024))
        json.object_set_number(fs_details, "used_space_gb", fs_stats.used_space / (1024 * 1024 * 1024))
        json.object_set_number(fs_details, "usage_percent", fs_stats.usage_percent)
        
        json.object_set_object(fs_info, "filesystem", fs_details)
    
    # Directory listing
    var dir_contents = fs.list_dir(".")
    if dir_contents:
        var file_count = 0
        var dir_count = 0
        var total_size = 0
        
        var items = json.array()
        var count = fs.dir_count(dir_contents)
        
        for i in range(0, count):
            var item_name = fs.dir_name(dir_contents, i)
            var item_path = fs.join(".", item_name)
            
            if fs.exists(item_path):
                var item_info = json.object()
                json.object_set_string(item_info, "name", item_name)
                
                if fs.is_file(item_path):
                    file_count = file_count + 1
                    var size = fs.size(item_path)
                    total_size = total_size + size
                    json.object_set_string(item_info, "type", "file")
                    json.object_set_number(item_info, "size", size)
                elif fs.is_directory(item_path):
                    dir_count = dir_count + 1
                    json.object_set_string(item_info, "type", "directory")
                
                json.object_set_bool(item_info, "readable", fs.is_readable(item_path))
                json.object_set_bool(item_info, "writable", fs.is_writable(item_path))
                json.object_set_bool(item_info, "executable", fs.is_executable(item_path))
                
                json.array_append_object(items, item_info)
        
        json.object_set_array(fs_info, "directory_contents", items)
        json.object_set_number(fs_info, "file_count", file_count)
        json.object_set_number(fs_info, "directory_count", dir_count)
        json.object_set_number(fs_info, "total_size_bytes", total_size)
        
        fs.dir_free(dir_contents)
    
    return fs_info

func save_report(system_data, network_data, filesystem_data):
    print("💾 Saving System Monitor Report...")
    
    var report = json.object()
    
    # Report metadata
    json.object_set_string(report, "report_type", "GPLANG System Monitor")
    json.object_set_string(report, "version", "1.0.0")
    json.object_set_string(report, "generated_by", "GPLANG Standard Library")
    json.object_set_number(report, "timestamp", os.get_timestamp())
    json.object_set_string(report, "datetime", os.get_datetime_string())
    
    # Add collected data
    json.object_set_object(report, "system", system_data)
    json.object_set_object(report, "network", network_data)
    json.object_set_object(report, "filesystem", filesystem_data)
    
    # Generate report filename with timestamp
    var timestamp = str(os.get_timestamp())
    var filename = "system_monitor_" + timestamp + ".json"
    
    var report_json = json.stringify_pretty(report)
    var write_result = fs.write(filename, report_json)
    
    if write_result == 0:
        print("✅ Report saved to: " + filename)
        
        var file_size = fs.size(filename)
        print("Report size: " + str(file_size) + " bytes")
        
        return filename
    else:
        print("❌ Failed to save report")
        return ""

func display_summary(system_data, network_data, filesystem_data):
    print("")
    print("📋 System Monitor Summary")
    print("========================")
    
    # System Summary
    var os_name = json.object_get_string(system_data, "os.name", "Unknown")
    var hostname = json.object_get_string(system_data, "os.hostname", "Unknown")
    var username = json.object_get_string(system_data, "os.username", "Unknown")
    
    print("🖥️  System: " + os_name + " on " + hostname + " (user: " + username + ")")
    
    # Resource Summary
    var memory_usage = json.object_get_number(system_data, "resources.memory_usage_percent", 0)
    var cpu_cores = json.object_get_number(system_data, "resources.cpu_cores", 0)
    var uptime_hours = json.object_get_number(system_data, "resources.uptime_hours", 0)
    
    print("💾 Memory Usage: " + str(memory_usage) + "%")
    print("🔧 CPU Cores: " + str(cpu_cores))
    print("⏱️  Uptime: " + str(uptime_hours) + " hours")
    
    # Network Summary
    var local_ip = json.object_get_string(network_data, "local_ip", "Unknown")
    var is_connected = json.object_get_bool(network_data, "is_connected", false)
    
    print("🌐 Local IP: " + local_ip)
    print("📡 Internet: " + (is_connected ? "Connected" : "Disconnected"))
    
    # File System Summary
    var fs_usage = json.object_get_number(filesystem_data, "filesystem.usage_percent", 0)
    var file_count = json.object_get_number(filesystem_data, "file_count", 0)
    var dir_count = json.object_get_number(filesystem_data, "directory_count", 0)
    
    print("💽 Disk Usage: " + str(fs_usage) + "%")
    print("📁 Current Dir: " + str(file_count) + " files, " + str(dir_count) + " directories")

func main():
    print("🚀 GPLANG System Monitor")
    print("========================")
    print("Comprehensive system monitoring using all standard library modules")
    print("")
    
    # Collect all system information
    var system_data = collect_system_info()
    var network_data = collect_network_info()
    var filesystem_data = collect_file_system_info()
    
    print("")
    
    # Save detailed report
    var report_file = save_report(system_data, network_data, filesystem_data)
    
    # Display summary
    display_summary(system_data, network_data, filesystem_data)
    
    print("")
    
    if report_file:
        print("📄 Detailed report available in: " + report_file)
        print("")
        print("To view the report:")
        print("  cat " + report_file)
        print("  # or")
        print("  python3 -m json.tool " + report_file)
    
    print("")
    print("✅ System Monitor Complete!")
    
    # Cleanup
    json.free(system_data)
    json.free(network_data)
    json.free(filesystem_data)
    
    return 0
