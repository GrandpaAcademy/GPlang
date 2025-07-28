#include "gplang_stdlib.h"
#include <time.h>

// Global module instances
GPLangOSModule gp_os;
GPLangNetModule gp_net;
GPLangFSModule gp_fs;
GPLangJsonModule gp_json;

// Extended module availability flags
bool gp_math_available = true;
bool gp_string_available = true;
bool gp_crypto_available = true;
bool gp_time_available = true;
bool gp_collections_available = true;

// Global error state
static int last_error = 0;
static char error_buffer[256] = {0};

// Library initialization
int gplang_stdlib_init(void) {
    // Initialize OS module
    gp_os.get_name = os_get_name;
    gp_os.get_version = os_get_version;
    gp_os.get_username = os_get_username;
    gp_os.get_home_dir = os_get_home_dir;
    gp_os.getenv = os_getenv;
    gp_os.setenv = os_setenv;
    gp_os.execute = os_execute;
    gp_os.sleep = os_sleep;
    gp_os.exit = os_exit;
    
    // Initialize Network module
    gp_net.http_get = http_get;
    gp_net.http_post = http_post;
    gp_net.tcp_connect = tcp_connect;
    gp_net.tcp_send = tcp_send;
    gp_net.tcp_receive = tcp_receive;
    gp_net.tcp_close = tcp_close;
    gp_net.get_local_ip = net_get_local_ip;
    gp_net.ping = net_ping;
    
    // Initialize File System module
    gp_fs.read_text = file_read_all;
    gp_fs.write_text = file_write_text;
    gp_fs.copy = file_copy;
    gp_fs.delete = file_delete;
    gp_fs.exists = file_exists;
    gp_fs.size = file_size;
    gp_fs.get_info = file_get_info;
    gp_fs.list_dir = dir_list;
    gp_fs.join_path = path_join;
    gp_fs.dirname = path_dirname;
    gp_fs.basename = path_basename;
    
    // Initialize JSON module (simplified for now)
    gp_json.create_object = json_create_object;
    gp_json.create_array = json_create_array;
    gp_json.object_set_string = json_object_set_string;
    gp_json.object_set_number = json_object_set_number;
    gp_json.object_set_bool = json_object_set_bool;
    gp_json.object_get_string = json_object_get_string;
    gp_json.object_get_number = json_object_get_number;
    gp_json.object_get_bool = json_object_get_bool;
    gp_json.array_append_string = json_array_append_string;
    gp_json.array_append_number = json_array_append_number;
    gp_json.array_get = json_array_get;
    gp_json.array_size = json_array_size;
    gp_json.destroy = json_destroy;
    gp_json.stringify = json_stringify;

    // Initialize extended modules
    if (gp_math_available) {
        // Math module is header-only, no initialization needed
    }

    if (gp_string_available) {
        // String module is header-only, no initialization needed
    }

    if (gp_crypto_available) {
        gp_crypto_init();
    }

    if (gp_time_available) {
        // Time module is header-only, no initialization needed
    }

    if (gp_collections_available) {
        gp_collections_init();
    }

    return 0;
}

void gplang_stdlib_cleanup(void) {
    // Cleanup extended modules
    if (gp_crypto_available) {
        gp_crypto_cleanup();
    }

    if (gp_collections_available) {
        gp_collections_cleanup();
    }

    // Cleanup any global resources
    last_error = 0;
    memset(error_buffer, 0, sizeof(error_buffer));
}

// Library information
const char* gplang_stdlib_version(void) {
    return GPLANG_STDLIB_VERSION;
}

const char* gplang_stdlib_build_date(void) {
    return __DATE__;
}

const char* gplang_stdlib_build_time(void) {
    return __TIME__;
}

// Module availability
int gplang_has_os_module(void) { return 1; }
int gplang_has_net_module(void) { return 1; }
int gplang_has_fs_module(void) { return 1; }
int gplang_has_json_module(void) { return 1; }
int gplang_has_math_module(void) { return gp_math_available ? 1 : 0; }
int gplang_has_string_module(void) { return gp_string_available ? 1 : 0; }
int gplang_has_crypto_module(void) { return gp_crypto_available ? 1 : 0; }
int gplang_has_time_module(void) { return gp_time_available ? 1 : 0; }
int gplang_has_collections_module(void) { return gp_collections_available ? 1 : 0; }

// GPLANG OS Bindings
char* gp_os_name(void) {
    return gp_os.get_name();
}

char* gp_os_username(void) {
    return gp_os.get_username();
}

char* gp_os_home(void) {
    return gp_os.get_home_dir();
}

char* gp_os_getenv(const char* name) {
    return gp_os.getenv(name);
}

int gp_os_setenv(const char* name, const char* value) {
    return gp_os.setenv(name, value);
}

int gp_os_execute(const char* command) {
    return gp_os.execute(command);
}

int gp_os_sleep(int seconds) {
    return gp_os.sleep(seconds);
}

void gp_os_exit(int code) {
    gp_os.exit(code);
}

// GPLANG Network Bindings
void* gp_net_http_get(const char* url) {
    return (void*)gp_net.http_get(url);
}

void* gp_net_http_post(const char* url, const char* data) {
    return (void*)gp_net.http_post(url, data);
}

char* gp_net_http_response_body(void* response) {
    HttpResponse* resp = (HttpResponse*)response;
    return resp ? resp->body : NULL;
}

int gp_net_http_response_status(void* response) {
    HttpResponse* resp = (HttpResponse*)response;
    return resp ? resp->status_code : -1;
}

void gp_net_http_response_free(void* response) {
    if (response) {
        http_response_destroy((HttpResponse*)response);
    }
}

void* gp_net_tcp_connect(const char* host, int port) {
    return (void*)gp_net.tcp_connect(host, port);
}

int gp_net_tcp_send(void* socket, const char* data) {
    SocketInfo* sock = (SocketInfo*)socket;
    return gp_net.tcp_send(sock, data, strlen(data));
}

char* gp_net_tcp_receive(void* socket) {
    SocketInfo* sock = (SocketInfo*)socket;
    size_t length;
    return gp_net.tcp_receive(sock, &length);
}

void gp_net_tcp_close(void* socket) {
    if (socket) {
        gp_net.tcp_close((SocketInfo*)socket);
    }
}

char* gp_net_local_ip(void) {
    return gp_net.get_local_ip();
}

int gp_net_ping(const char* host) {
    return gp_net.ping(host, 5); // 5 second timeout
}

// GPLANG File System Bindings
char* gp_fs_read(const char* path) {
    return gp_fs.read_text(path);
}

int gp_fs_write(const char* path, const char* content) {
    return gp_fs.write_text(path, content);
}

int gp_fs_copy(const char* src, const char* dest) {
    return gp_fs.copy(src, dest);
}

int gp_fs_delete(const char* path) {
    return gp_fs.delete(path);
}

int gp_fs_exists(const char* path) {
    return gp_fs.exists(path);
}

long gp_fs_size(const char* path) {
    return gp_fs.size(path);
}

char* gp_fs_join(const char* path1, const char* path2) {
    return gp_fs.join_path(path1, path2);
}

char* gp_fs_dirname(const char* path) {
    return gp_fs.dirname(path);
}

char* gp_fs_basename(const char* path) {
    return gp_fs.basename(path);
}

void* gp_fs_list_dir(const char* path) {
    int count;
    return (void*)gp_fs.list_dir(path, &count);
}

int gp_fs_dir_count(void* dir_list) {
    // This would need to be implemented properly
    (void)dir_list;
    return 0;
}

char* gp_fs_dir_name(void* dir_list, int index) {
    // This would need to be implemented properly
    (void)dir_list; (void)index;
    return NULL;
}

void gp_fs_dir_free(void* dir_list) {
    // This would need to be implemented properly
    (void)dir_list;
}

// GPLANG JSON Bindings
void* gp_json_parse(const char* json_string) {
    return (void*)gp_json.parse(json_string);
}

char* gp_json_stringify(void* json_value) {
    return gp_json.stringify((JsonValue*)json_value);
}

void* gp_json_object(void) {
    return (void*)gp_json.create_object();
}

void* gp_json_array(void) {
    return (void*)gp_json.create_array();
}

int gp_json_object_set_string(void* object, const char* key, const char* value) {
    return gp_json.object_set_string((JsonValue*)object, key, value);
}

int gp_json_object_set_number(void* object, const char* key, double value) {
    return gp_json.object_set_number((JsonValue*)object, key, value);
}

int gp_json_object_set_bool(void* object, const char* key, int value) {
    return gp_json.object_set_bool((JsonValue*)object, key, value != 0);
}

char* gp_json_object_get_string(void* object, const char* key) {
    return gp_json.object_get_string((JsonValue*)object, key, NULL);
}

double gp_json_object_get_number(void* object, const char* key) {
    return gp_json.object_get_number((JsonValue*)object, key, 0.0);
}

int gp_json_object_get_bool(void* object, const char* key) {
    return gp_json.object_get_bool((JsonValue*)object, key, false) ? 1 : 0;
}

int gp_json_array_append_string(void* array, const char* value) {
    return gp_json.array_append_string((JsonValue*)array, value);
}

int gp_json_array_append_number(void* array, double value) {
    return gp_json.array_append_number((JsonValue*)array, value);
}

void* gp_json_array_get(void* array, int index) {
    return (void*)gp_json.array_get((JsonValue*)array, index);
}

int gp_json_array_size(void* array) {
    return gp_json.array_size((JsonValue*)array);
}

void gp_json_free(void* json_value) {
    if (json_value) {
        gp_json.destroy((JsonValue*)json_value);
    }
}

// Error handling
int gp_get_last_error(void) {
    return last_error;
}

char* gp_get_error_string(void) {
    return error_buffer[0] ? error_buffer : "No error";
}
