#ifndef GPLANG_OS_H
#define GPLANG_OS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

// OS Information
typedef struct {
    char* name;           // OS name (Linux, Windows, macOS)
    char* version;        // OS version
    char* architecture;   // x86_64, arm64, etc.
    char* hostname;       // Computer hostname
    char* username;       // Current user
    char* home_dir;       // User home directory
    char* temp_dir;       // Temporary directory
} OSInfo;

// Process Information
typedef struct {
    int pid;              // Process ID
    int ppid;             // Parent process ID
    char* name;           // Process name
    char* command;        // Full command line
    int status;           // Process status
    double cpu_usage;     // CPU usage percentage
    long memory_usage;    // Memory usage in bytes
} ProcessInfo;

// Environment Variable
typedef struct {
    char* name;
    char* value;
} EnvVar;

// Function declarations

// OS Information
OSInfo* os_get_info(void);
void os_info_destroy(OSInfo* info);
char* os_get_name(void);
char* os_get_version(void);
char* os_get_architecture(void);
char* os_get_hostname(void);
char* os_get_username(void);
char* os_get_home_dir(void);
char* os_get_temp_dir(void);
char* os_get_current_dir(void);

// Environment Variables
char* os_getenv(const char* name);
int os_setenv(const char* name, const char* value);
int os_unsetenv(const char* name);
EnvVar** os_get_all_env(int* count);
void os_env_destroy(EnvVar** env_vars, int count);

// Process Management
int os_execute(const char* command);
int os_execute_async(const char* command);
int os_spawn(const char* program, char* const argv[]);
int os_spawn_async(const char* program, char* const argv[]);
int os_wait_process(int pid);
int os_kill_process(int pid);
int os_get_pid(void);
int os_get_ppid(void);
ProcessInfo* os_get_process_info(int pid);
ProcessInfo** os_get_all_processes(int* count);
void os_process_info_destroy(ProcessInfo* info);
void os_processes_destroy(ProcessInfo** processes, int count);

// System Control
int os_shutdown(void);
int os_restart(void);
int os_sleep(int seconds);
int os_usleep(int microseconds);
void os_exit(int code);

// System Resources
long os_get_total_memory(void);
long os_get_free_memory(void);
long os_get_used_memory(void);
double os_get_cpu_usage(void);
int os_get_cpu_count(void);
double os_get_load_average(void);
long os_get_uptime(void);

// Time and Date
time_t os_get_timestamp(void);
char* os_get_time_string(void);
char* os_get_date_string(void);
char* os_get_datetime_string(void);
struct tm* os_get_local_time(void);
struct tm* os_get_utc_time(void);

// Utility functions
char* os_which(const char* program);
int os_is_admin(void);
int os_is_64bit(void);
char* os_get_error_string(int error_code);

#endif // GPLANG_OS_H
