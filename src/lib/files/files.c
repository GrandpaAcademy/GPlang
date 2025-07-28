#include "files.h"
#include <libgen.h>
#include <limits.h>

// Simple strdup implementation
static char* gp_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) strcpy(copy, s);
    return copy;
}

// Path Utilities
char* path_join(const char* path1, const char* path2) {
    if (!path1 || !path2) return NULL;
    
    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);
    char* result = malloc(len1 + len2 + 2);
    
    if (!result) return NULL;
    
    strcpy(result, path1);
    
    // Add separator if needed
    if (len1 > 0 && path1[len1-1] != '/' && path2[0] != '/') {
        strcat(result, "/");
    }
    
    strcat(result, path2);
    return result;
}

char* path_dirname(const char* path) {
    if (!path) return NULL;
    char* path_copy = gp_strdup(path);
    char* dir = dirname(path_copy);
    char* result = gp_strdup(dir);
    free(path_copy);
    return result;
}

char* path_basename(const char* path) {
    if (!path) return NULL;
    char* path_copy = gp_strdup(path);
    char* base = basename(path_copy);
    char* result = gp_strdup(base);
    free(path_copy);
    return result;
}

char* path_extension(const char* path) {
    if (!path) return NULL;
    char* dot = strrchr(path, '.');
    if (dot && dot != path) {
        return gp_strdup(dot + 1);
    }
    return gp_strdup("");
}

char* path_absolute(const char* path) {
    if (!path) return NULL;
    char* abs_path = realpath(path, NULL);
    return abs_path ? abs_path : gp_strdup(path);
}

int path_exists(const char* path) {
    return access(path, F_OK) == 0;
}

int path_is_absolute(const char* path) {
    return path && path[0] == '/';
}

// File Operations
FileHandle* file_open(const char* path, const char* mode) {
    if (!path || !mode) return NULL;
    
    FileHandle* handle = malloc(sizeof(FileHandle));
    if (!handle) return NULL;
    
    handle->fp = fopen(path, mode);
    if (!handle->fp) {
        free(handle);
        return NULL;
    }
    
    handle->path = gp_strdup(path);
    handle->mode = gp_strdup(mode);
    handle->is_open = 1;
    handle->position = 0;
    
    // Get file size
    fseek(handle->fp, 0, SEEK_END);
    handle->size = ftell(handle->fp);
    fseek(handle->fp, 0, SEEK_SET);
    
    return handle;
}

int file_close(FileHandle* handle) {
    if (!handle || !handle->is_open) return -1;
    
    int result = fclose(handle->fp);
    handle->is_open = 0;
    
    free(handle->path);
    free(handle->mode);
    free(handle);
    
    return result;
}

char* file_read_all(const char* path) {
    FILE* fp = fopen(path, "r");
    if (!fp) return NULL;
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char* content = malloc(size + 1);
    if (!content) {
        fclose(fp);
        return NULL;
    }
    
    size_t read_size = fread(content, 1, size, fp);
    content[read_size] = '\0';
    
    fclose(fp);
    return content;
}

int file_write_text(const char* path, const char* content) {
    if (!path || !content) return -1;
    
    FILE* fp = fopen(path, "w");
    if (!fp) return -1;
    
    size_t written = fwrite(content, 1, strlen(content), fp);
    fclose(fp);
    
    return written == strlen(content) ? 0 : -1;
}

int file_copy(const char* src, const char* dest) {
    if (!src || !dest) return -1;
    
    FILE* src_fp = fopen(src, "rb");
    if (!src_fp) return -1;
    
    FILE* dest_fp = fopen(dest, "wb");
    if (!dest_fp) {
        fclose(src_fp);
        return -1;
    }
    
    char buffer[4096];
    size_t bytes;
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), src_fp)) > 0) {
        if (fwrite(buffer, 1, bytes, dest_fp) != bytes) {
            fclose(src_fp);
            fclose(dest_fp);
            return -1;
        }
    }
    
    fclose(src_fp);
    fclose(dest_fp);
    return 0;
}

int file_delete(const char* path) {
    return unlink(path);
}

int file_exists(const char* path) {
    return access(path, F_OK) == 0;
}

long file_size(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return st.st_size;
    }
    return -1;
}

// File Information
FileInfo* file_get_info(const char* path) {
    if (!path) return NULL;
    
    struct stat st;
    if (stat(path, &st) != 0) return NULL;
    
    FileInfo* info = malloc(sizeof(FileInfo));
    if (!info) return NULL;
    
    info->path = gp_strdup(path);
    info->name = path_basename(path);
    info->extension = path_extension(path);
    info->directory = path_dirname(path);
    info->size = st.st_size;
    info->created = st.st_ctime;
    info->modified = st.st_mtime;
    info->accessed = st.st_atime;
    info->permissions = st.st_mode & 0777;
    info->is_file = S_ISREG(st.st_mode);
    info->is_directory = S_ISDIR(st.st_mode);
    info->is_symlink = S_ISLNK(st.st_mode);
    info->is_readable = access(path, R_OK) == 0;
    info->is_writable = access(path, W_OK) == 0;
    info->is_executable = access(path, X_OK) == 0;
    
    return info;
}

void file_info_destroy(FileInfo* info) {
    if (!info) return;
    free(info->path);
    free(info->name);
    free(info->extension);
    free(info->directory);
    free(info);
}

// Directory Operations
int dir_create(const char* path) {
    return mkdir(path, 0755);
}

int dir_exists(const char* path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

DirEntry** dir_list(const char* path, int* count) {
    if (!path || !count) return NULL;
    
    DIR* dir = opendir(path);
    if (!dir) return NULL;
    
    // Count entries first
    int entry_count = 0;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            entry_count++;
        }
    }
    
    rewinddir(dir);
    
    DirEntry** entries = malloc(entry_count * sizeof(DirEntry*));
    if (!entries) {
        closedir(dir);
        return NULL;
    }
    
    int i = 0;
    while ((entry = readdir(dir)) != NULL && i < entry_count) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        DirEntry* dir_entry = malloc(sizeof(DirEntry));
        if (!dir_entry) continue;
        
        dir_entry->name = gp_strdup(entry->d_name);
        dir_entry->path = path_join(path, entry->d_name);
        
        struct stat st;
        if (stat(dir_entry->path, &st) == 0) {
            dir_entry->is_file = S_ISREG(st.st_mode);
            dir_entry->is_directory = S_ISDIR(st.st_mode);
            dir_entry->is_symlink = S_ISLNK(st.st_mode);
            dir_entry->size = st.st_size;
            dir_entry->modified = st.st_mtime;
        } else {
            dir_entry->is_file = 0;
            dir_entry->is_directory = 0;
            dir_entry->is_symlink = 0;
            dir_entry->size = 0;
            dir_entry->modified = 0;
        }
        
        entries[i++] = dir_entry;
    }
    
    closedir(dir);
    *count = i;
    return entries;
}

void dir_entries_destroy(DirEntry** entries, int count) {
    if (!entries) return;
    
    for (int i = 0; i < count; i++) {
        if (entries[i]) {
            free(entries[i]->name);
            free(entries[i]->path);
            free(entries[i]);
        }
    }
    free(entries);
}

// Error Handling
char* file_get_error_string(int error_code) {
    return gp_strdup(strerror(error_code));
}

int file_get_last_error(void) {
    return errno;
}
