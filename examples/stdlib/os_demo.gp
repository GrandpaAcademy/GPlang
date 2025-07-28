# GPLANG OS Module Demo
# Demonstrates operating system interaction capabilities

import os

func main():
    print("🖥️  GPLANG OS Module Demo")
    print("========================")
    print("")
    
    # System Information
    print("📊 System Information:")
    var os_name = os.name()
    var username = os.username()
    var home_dir = os.home()
    
    print("OS Name: " + os_name)
    print("Username: " + username)
    print("Home Directory: " + home_dir)
    print("")
    
    # Environment Variables
    print("🌍 Environment Variables:")
    var path = os.getenv("PATH")
    var shell = os.getenv("SHELL")
    
    if path:
        print("PATH: " + path)
    else:
        print("PATH: Not set")
    
    if shell:
        print("SHELL: " + shell)
    else:
        print("SHELL: Not set")
    
    # Set a custom environment variable
    var result = os.setenv("GPLANG_TEST", "Hello from GPLANG!")
    if result == 0:
        var test_var = os.getenv("GPLANG_TEST")
        print("Custom Variable: " + test_var)
    
    print("")
    
    # Process Information
    print("⚙️  Process Information:")
    var pid = os.get_pid()
    var ppid = os.get_ppid()
    
    print("Process ID: " + str(pid))
    print("Parent Process ID: " + str(ppid))
    print("")
    
    # Execute Commands
    print("🚀 Command Execution:")
    print("Running 'echo Hello from GPLANG!'...")
    
    var exit_code = os.execute("echo 'Hello from GPLANG!'")
    print("Command exit code: " + str(exit_code))
    print("")
    
    # System Resources
    print("💾 System Resources:")
    var total_memory = os.get_total_memory()
    var free_memory = os.get_free_memory()
    var cpu_count = os.get_cpu_count()
    
    if total_memory > 0:
        var total_gb = total_memory / (1024 * 1024 * 1024)
        print("Total Memory: " + str(total_gb) + " GB")
    
    if free_memory > 0:
        var free_gb = free_memory / (1024 * 1024 * 1024)
        print("Free Memory: " + str(free_gb) + " GB")
    
    if cpu_count > 0:
        print("CPU Cores: " + str(cpu_count))
    
    print("")
    
    # Time and Date
    print("🕒 Time and Date:")
    var timestamp = os.get_timestamp()
    var time_str = os.get_time_string()
    var date_str = os.get_date_string()
    var datetime_str = os.get_datetime_string()
    
    print("Timestamp: " + str(timestamp))
    print("Time: " + time_str)
    print("Date: " + date_str)
    print("DateTime: " + datetime_str)
    print("")
    
    # Utility Functions
    print("🔧 Utility Functions:")
    var is_admin = os.is_admin()
    var is_64bit = os.is_64bit()
    
    if is_admin:
        print("Running as administrator: Yes")
    else:
        print("Running as administrator: No")
    
    if is_64bit:
        print("64-bit system: Yes")
    else:
        print("64-bit system: No")
    
    # Find programs
    var python_path = os.which("python3")
    if python_path:
        print("Python3 found at: " + python_path)
    else:
        print("Python3 not found in PATH")
    
    print("")
    print("✅ OS Module Demo Complete!")
    
    return 0
