#ifndef GPLANG_FS_H
#define GPLANG_FS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

// File Information
typedef struct {
    char* path;           // Full path
    char* name;           // File name only
    char* extension;      // File extension
    char* directory;      // Directory path
    long size;            // File size in bytes
    time_t created;       // Creation time
    time_t modified;      // Last modification time
    time_t accessed;      // Last access time
    int permissions;      // File permissions
    int is_file;          // Is regular file
    int is_directory;     // Is directory
    int is_symlink;       // Is symbolic link
    int is_readable;      // Is readable
    int is_writable;      // Is writable
    int is_executable;    // Is executable
} FileInfo;

// Directory Entry
typedef struct {
    char* name;           // Entry name
    char* path;           // Full path
    int is_file;          // Is regular file
    int is_directory;     // Is directory
    int is_symlink;       // Is symbolic link
    long size;            // File size
    time_t modified;      // Last modification time
} DirEntry;

// File Handle
typedef struct {
    FILE* fp;             // File pointer
    char* path;           // File path
    char* mode;           // Open mode
    int is_open;          // Is file open
    long position;        // Current position
    long size;            // File size
} FileHandle;

// Path utilities
char* path_join(const char* path1, const char* path2);
char* path_dirname(const char* path);
char* path_basename(const char* path);
char* path_extension(const char* path);
char* path_absolute(const char* path);
char* path_relative(const char* from, const char* to);
char* path_normalize(const char* path);
int path_exists(const char* path);
int path_is_absolute(const char* path);
int path_is_relative(const char* path);

// File operations
FileHandle* file_open(const char* path, const char* mode);
int file_close(FileHandle* handle);
char* file_read_all(const char* path);
char* file_read_text(const char* path);
char* file_read_lines(const char* path, int* line_count);
int file_write_text(const char* path, const char* content);
int file_write_lines(const char* path, char** lines, int line_count);
int file_append_text(const char* path, const char* content);
int file_copy(const char* src, const char* dest);
int file_move(const char* src, const char* dest);
int file_delete(const char* path);
int file_exists(const char* path);
long file_size(const char* path);

// File information
FileInfo* file_get_info(const char* path);
void file_info_destroy(FileInfo* info);
time_t file_get_modified_time(const char* path);
time_t file_get_created_time(const char* path);
time_t file_get_accessed_time(const char* path);
int file_get_permissions(const char* path);
int file_set_permissions(const char* path, int permissions);

// File type checking
int file_is_file(const char* path);
int file_is_directory(const char* path);
int file_is_symlink(const char* path);
int file_is_readable(const char* path);
int file_is_writable(const char* path);
int file_is_executable(const char* path);

// Directory operations
int dir_create(const char* path);
int dir_create_recursive(const char* path);
int dir_delete(const char* path);
int dir_delete_recursive(const char* path);
DirEntry** dir_list(const char* path, int* count);
DirEntry** dir_list_recursive(const char* path, int* count);
void dir_entries_destroy(DirEntry** entries, int count);
void dir_entry_destroy(DirEntry* entry);
int dir_exists(const char* path);
char* dir_get_current(void);
int dir_set_current(const char* path);

// File searching
char** file_find(const char* directory, const char* pattern, int* count);
char** file_find_recursive(const char* directory, const char* pattern, int* count);
char** file_find_by_extension(const char* directory, const char* extension, int* count);
char** file_find_by_size(const char* directory, long min_size, long max_size, int* count);
char** file_find_by_date(const char* directory, time_t start_date, time_t end_date, int* count);
void file_list_destroy(char** files, int count);

// File watching (basic)
typedef struct {
    char* path;
    time_t last_modified;
    long last_size;
    int is_watching;
} FileWatcher;

FileWatcher* file_watch_create(const char* path);
int file_watch_check(FileWatcher* watcher);
void file_watch_destroy(FileWatcher* watcher);

// Temporary files
char* file_create_temp(const char* prefix);
char* file_create_temp_dir(const char* prefix);
int file_delete_temp(const char* path);

// File locking
int file_lock(const char* path);
int file_unlock(const char* path);
int file_is_locked(const char* path);

// File compression (basic)
int file_compress(const char* src, const char* dest);
int file_decompress(const char* src, const char* dest);

// File hashing
char* file_hash_md5(const char* path);
char* file_hash_sha1(const char* path);
char* file_hash_sha256(const char* path);

// File comparison
int file_compare(const char* path1, const char* path2);
int file_compare_content(const char* path1, const char* path2);
double file_similarity(const char* path1, const char* path2);

// Batch operations
int files_copy_batch(char** src_files, char** dest_files, int count);
int files_move_batch(char** src_files, char** dest_files, int count);
int files_delete_batch(char** files, int count);

// File system information
typedef struct {
    char* mount_point;
    char* filesystem;
    long total_space;
    long free_space;
    long used_space;
    double usage_percent;
} FileSystemInfo;

FileSystemInfo** fs_get_info(int* count);
FileSystemInfo* fs_get_info_for_path(const char* path);
void fs_info_destroy(FileSystemInfo** info, int count);

// Error handling
char* file_get_error_string(int error_code);
int file_get_last_error(void);

#endif // GPLANG_FS_H
