#define _GNU_SOURCE
#include "os.h"
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <signal.h>

// Simple strdup implementation
static char* gp_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) strcpy(copy, s);
    return copy;
}

// OS Information Functions
OSInfo* os_get_info(void) {
    OSInfo* info = malloc(sizeof(OSInfo));
    if (!info) return NULL;
    
    info->name = os_get_name();
    info->version = os_get_version();
    info->architecture = os_get_architecture();
    info->hostname = os_get_hostname();
    info->username = os_get_username();
    info->home_dir = os_get_home_dir();
    info->temp_dir = os_get_temp_dir();
    
    return info;
}

void os_info_destroy(OSInfo* info) {
    if (!info) return;
    free(info->name);
    free(info->version);
    free(info->architecture);
    free(info->hostname);
    free(info->username);
    free(info->home_dir);
    free(info->temp_dir);
    free(info);
}

char* os_get_name(void) {
    struct utsname sys_info;
    if (uname(&sys_info) == 0) {
        return gp_strdup(sys_info.sysname);
    }
    return gp_strdup("Unknown");
}

char* os_get_version(void) {
    struct utsname sys_info;
    if (uname(&sys_info) == 0) {
        return gp_strdup(sys_info.release);
    }
    return gp_strdup("Unknown");
}

char* os_get_architecture(void) {
    struct utsname sys_info;
    if (uname(&sys_info) == 0) {
        return gp_strdup(sys_info.machine);
    }
    return gp_strdup("Unknown");
}

char* os_get_hostname(void) {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return gp_strdup(hostname);
    }
    return gp_strdup("localhost");
}

char* os_get_username(void) {
    struct passwd* pw = getpwuid(getuid());
    if (pw) {
        return gp_strdup(pw->pw_name);
    }
    char* user = getenv("USER");
    return user ? gp_strdup(user) : gp_strdup("unknown");
}

char* os_get_home_dir(void) {
    char* home = getenv("HOME");
    if (home) {
        return gp_strdup(home);
    }
    struct passwd* pw = getpwuid(getuid());
    if (pw) {
        return gp_strdup(pw->pw_dir);
    }
    return gp_strdup("/tmp");
}

char* os_get_temp_dir(void) {
    char* tmp = getenv("TMPDIR");
    if (tmp) return gp_strdup(tmp);
    
    tmp = getenv("TMP");
    if (tmp) return gp_strdup(tmp);
    
    tmp = getenv("TEMP");
    if (tmp) return gp_strdup(tmp);
    
    return gp_strdup("/tmp");
}

char* os_get_current_dir(void) {
    char* cwd = getcwd(NULL, 0);
    return cwd ? cwd : gp_strdup(".");
}

// Environment Variables
char* os_getenv(const char* name) {
    char* value = getenv(name);
    return value ? gp_strdup(value) : NULL;
}

int os_setenv(const char* name, const char* value) {
    return setenv(name, value, 1);
}

int os_unsetenv(const char* name) {
    return unsetenv(name);
}

// Process Management
int os_execute(const char* command) {
    return system(command);
}

int os_execute_async(const char* command) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execl("/bin/sh", "sh", "-c", command, (char*)NULL);
        exit(1);
    }
    return pid;
}

int os_spawn(const char* program, char* const argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execv(program, argv);
        exit(1);
    }
    
    if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
    
    return -1;
}

int os_spawn_async(const char* program, char* const argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execv(program, argv);
        exit(1);
    }
    return pid;
}

int os_wait_process(int pid) {
    int status;
    if (waitpid(pid, &status, 0) == pid) {
        return WEXITSTATUS(status);
    }
    return -1;
}

int os_kill_process(int pid) {
    return kill(pid, SIGTERM);
}

int os_get_pid(void) {
    return getpid();
}

int os_get_ppid(void) {
    return getppid();
}

// System Control
int os_sleep(int seconds) {
    return sleep(seconds);
}

int os_usleep(int microseconds) {
    return usleep(microseconds);
}

void os_exit(int code) {
    exit(code);
}

// System Resources
long os_get_total_memory(void) {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return info.totalram * info.mem_unit;
    }
    return -1;
}

long os_get_free_memory(void) {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return info.freeram * info.mem_unit;
    }
    return -1;
}

long os_get_used_memory(void) {
    long total = os_get_total_memory();
    long free = os_get_free_memory();
    if (total > 0 && free > 0) {
        return total - free;
    }
    return -1;
}

int os_get_cpu_count(void) {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

long os_get_uptime(void) {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return info.uptime;
    }
    return -1;
}

// Time and Date
time_t os_get_timestamp(void) {
    return time(NULL);
}

char* os_get_time_string(void) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char* time_str = malloc(32);
    if (time_str) {
        strftime(time_str, 32, "%H:%M:%S", tm_info);
    }
    return time_str;
}

char* os_get_date_string(void) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char* date_str = malloc(32);
    if (date_str) {
        strftime(date_str, 32, "%Y-%m-%d", tm_info);
    }
    return date_str;
}

char* os_get_datetime_string(void) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char* datetime_str = malloc(64);
    if (datetime_str) {
        strftime(datetime_str, 64, "%Y-%m-%d %H:%M:%S", tm_info);
    }
    return datetime_str;
}

struct tm* os_get_local_time(void) {
    time_t now = time(NULL);
    return localtime(&now);
}

struct tm* os_get_utc_time(void) {
    time_t now = time(NULL);
    return gmtime(&now);
}

// Utility functions
char* os_which(const char* program) {
    char command[512];
    snprintf(command, sizeof(command), "which %s", program);
    
    FILE* fp = popen(command, "r");
    if (!fp) return NULL;
    
    char path[512];
    if (fgets(path, sizeof(path), fp)) {
        pclose(fp);
        // Remove newline
        char* newline = strchr(path, '\n');
        if (newline) *newline = '\0';
        return gp_strdup(path);
    }
    
    pclose(fp);
    return NULL;
}

int os_is_admin(void) {
    return getuid() == 0;
}

int os_is_64bit(void) {
    return sizeof(void*) == 8;
}

char* os_get_error_string(int error_code) {
    return gp_strdup(strerror(error_code));
}
