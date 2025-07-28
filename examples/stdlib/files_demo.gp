# GPLANG Files Module Demo
# Demonstrates file system operations

import files

func main():
    print("📁 GPLANG Files Module Demo")
    print("===========================")
    print("")
    
    # Path Operations
    print("🛤️  Path Operations:")
    var home_path = "/home/user"
    var file_name = "test.txt"
    var full_path = files.join(home_path, file_name)
    
    print("Home Path: " + home_path)
    print("File Name: " + file_name)
    print("Full Path: " + full_path)
    
    var dir_name = files.dirname(full_path)
    var base_name = files.basename(full_path)
    
    print("Directory: " + dir_name)
    print("Basename: " + base_name)
    print("")
    
    # File Creation and Writing
    print("✍️  File Writing:")
    var test_file = "gplang_test.txt"
    var content = "Hello from GPLANG!\nThis is a test file.\nCreated with the Files module."
    
    var write_result = files.write(test_file, content)
    if write_result == 0:
        print("✅ Successfully wrote to: " + test_file)
    else:
        print("❌ Failed to write file")
    
    print("")
    
    # File Reading
    print("📖 File Reading:")
    if files.exists(test_file):
        print("File exists: " + test_file)
        
        var file_content = files.read(test_file)
        if file_content:
            print("File content:")
            print("-------------")
            print(file_content)
            print("-------------")
        else:
            print("❌ Failed to read file")
        
        # File size
        var file_size = files.size(test_file)
        if file_size >= 0:
            print("File size: " + str(file_size) + " bytes")
        
    else:
        print("❌ File does not exist: " + test_file)
    
    print("")
    
    # File Information
    print("ℹ️  File Information:")
    if files.exists(test_file):
        var info = files.get_info(test_file)
        if info:
            print("File path: " + info.path)
            print("File name: " + info.name)
            print("File size: " + str(info.size) + " bytes")
            print("Is file: " + str(info.is_file))
            print("Is directory: " + str(info.is_directory))
            print("Is readable: " + str(info.is_readable))
            print("Is writable: " + str(info.is_writable))
            print("Permissions: " + str(info.permissions))
        
    print("")
    
    # Directory Operations
    print("📂 Directory Operations:")
    var test_dir = "gplang_test_dir"
    
    # Create directory
    var dir_result = files.create_dir(test_dir)
    if dir_result == 0:
        print("✅ Created directory: " + test_dir)
    else:
        print("❌ Failed to create directory")
    
    # Check if directory exists
    if files.exists(test_dir):
        print("Directory exists: " + test_dir)
        
        # List directory contents
        var dir_list = files.list_dir(".")
        if dir_list:
            var count = files.dir_count(dir_list)
            print("Current directory contains " + str(count) + " items:")
            
            for i in range(0, count):
                var item_name = files.dir_name(dir_list, i)
                print("  - " + item_name)
            
            files.dir_free(dir_list)
        
    print("")
    
    # File Operations
    print("🔄 File Operations:")
    var backup_file = "gplang_test_backup.txt"
    
    # Copy file
    if files.exists(test_file):
        var copy_result = files.copy(test_file, backup_file)
        if copy_result == 0:
            print("✅ Copied " + test_file + " to " + backup_file)
        else:
            print("❌ Failed to copy file")
    
    # Check both files exist
    if files.exists(test_file) and files.exists(backup_file):
        print("Both files exist:")
        print("  Original: " + str(files.size(test_file)) + " bytes")
        print("  Backup: " + str(files.size(backup_file)) + " bytes")
    
    print("")
    
    # File System Information
    print("💽 File System Information:")
    var fs_info = files.get_fs_info(".")
    if fs_info:
        print("Mount point: " + fs_info.mount_point)
        print("File system: " + fs_info.filesystem)
        
        var total_gb = fs_info.total_space / (1024 * 1024 * 1024)
        var free_gb = fs_info.free_space / (1024 * 1024 * 1024)
        var used_gb = fs_info.used_space / (1024 * 1024 * 1024)
        
        print("Total space: " + str(total_gb) + " GB")
        print("Free space: " + str(free_gb) + " GB")
        print("Used space: " + str(used_gb) + " GB")
        print("Usage: " + str(fs_info.usage_percent) + "%")
    
    print("")
    
    # Cleanup
    print("🧹 Cleanup:")
    
    # Delete test files
    if files.exists(test_file):
        var delete_result = files.delete(test_file)
        if delete_result == 0:
            print("✅ Deleted: " + test_file)
        else:
            print("❌ Failed to delete: " + test_file)
    
    if files.exists(backup_file):
        var delete_result2 = files.delete(backup_file)
        if delete_result2 == 0:
            print("✅ Deleted: " + backup_file)
        else:
            print("❌ Failed to delete: " + backup_file)
    
    # Delete test directory
    if files.exists(test_dir):
        var dir_delete_result = files.delete_dir(test_dir)
        if dir_delete_result == 0:
            print("✅ Deleted directory: " + test_dir)
        else:
            print("❌ Failed to delete directory: " + test_dir)
    
    print("")
    print("✅ Files Module Demo Complete!")
    
    return 0
