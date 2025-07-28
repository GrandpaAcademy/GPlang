#ifndef GPLANG_STDLIB_H
#define GPLANG_STDLIB_H

// GPLANG Standard Library
// This header includes all standard library modules

// Core system libraries
#include "os/os.h"
#include "net/net.h"
#include "fs/fs.h"
#include "json/json.h"

// Extended libraries
#include "math/math.h"
#include "string/string.h"
#include "crypto/crypto.h"
#include "time/time.h"
#include "collections/collections.h"

// GPLANG Standard Library Version
#define GPLANG_STDLIB_VERSION_MAJOR 1
#define GPLANG_STDLIB_VERSION_MINOR 0
#define GPLANG_STDLIB_VERSION_PATCH 0
#define GPLANG_STDLIB_VERSION "1.0.0"

// Library initialization and cleanup
int gplang_stdlib_init(void);
void gplang_stdlib_cleanup(void);

// Library information
const char* gplang_stdlib_version(void);
const char* gplang_stdlib_build_date(void);
const char* gplang_stdlib_build_time(void);

// Module availability checks
int gplang_has_os_module(void);
int gplang_has_net_module(void);
int gplang_has_fs_module(void);
int gplang_has_json_module(void);
int gplang_has_math_module(void);
int gplang_has_string_module(void);
int gplang_has_crypto_module(void);
int gplang_has_time_module(void);
int gplang_has_collections_module(void);

// GPLANG Runtime Integration
// These functions bridge GPLANG language constructs to C library calls

// OS Module Integration
typedef struct {
    char* (*get_name)(void);
    char* (*get_version)(void);
    char* (*get_username)(void);
    char* (*get_home_dir)(void);
    char* (*getenv)(const char* name);
    int (*setenv)(const char* name, const char* value);
    int (*execute)(const char* command);
    int (*sleep)(int seconds);
    void (*exit)(int code);
} GPLangOSModule;

// Network Module Integration
typedef struct {
    HttpResponse* (*http_get)(const char* url);
    HttpResponse* (*http_post)(const char* url, const char* data);
    SocketInfo* (*tcp_connect)(const char* host, int port);
    int (*tcp_send)(SocketInfo* socket, const char* data, size_t length);
    char* (*tcp_receive)(SocketInfo* socket, size_t* length);
    int (*tcp_close)(SocketInfo* socket);
    char* (*get_local_ip)(void);
    int (*ping)(const char* host, int timeout);
} GPLangNetModule;

// File System Module Integration
typedef struct {
    char* (*read_text)(const char* path);
    int (*write_text)(const char* path, const char* content);
    int (*copy)(const char* src, const char* dest);
    int (*delete)(const char* path);
    int (*exists)(const char* path);
    long (*size)(const char* path);
    FileInfo* (*get_info)(const char* path);
    DirEntry** (*list_dir)(const char* path, int* count);
    char* (*join_path)(const char* path1, const char* path2);
    char* (*dirname)(const char* path);
    char* (*basename)(const char* path);
} GPLangFSModule;

// JSON Module Integration
typedef struct {
    JsonValue* (*parse)(const char* json_string);
    char* (*stringify)(JsonValue* value);
    JsonValue* (*create_object)(void);
    JsonValue* (*create_array)(void);
    int (*object_set_string)(JsonValue* object, const char* key, const char* value);
    int (*object_set_number)(JsonValue* object, const char* key, double value);
    int (*object_set_bool)(JsonValue* object, const char* key, bool value);
    char* (*object_get_string)(JsonValue* object, const char* key, const char* default_value);
    double (*object_get_number)(JsonValue* object, const char* key, double default_value);
    bool (*object_get_bool)(JsonValue* object, const char* key, bool default_value);
    int (*array_append_string)(JsonValue* array, const char* value);
    int (*array_append_number)(JsonValue* array, double value);
    JsonValue* (*array_get)(JsonValue* array, int index);
    int (*array_size)(JsonValue* array);
    void (*destroy)(JsonValue* value);
} GPLangJsonModule;

// Global module instances
extern GPLangOSModule gp_os;
extern GPLangNetModule gp_net;
extern GPLangFSModule gp_fs;
extern GPLangJsonModule gp_json;

// Extended module instances (simplified access)
extern bool gp_math_available;
extern bool gp_string_available;
extern bool gp_crypto_available;
extern bool gp_time_available;
extern bool gp_collections_available;

// GPLANG Language Bindings
// These functions are called directly from GPLANG code

// OS bindings
char* gp_os_name(void);
char* gp_os_username(void);
char* gp_os_home(void);
char* gp_os_getenv(const char* name);
int gp_os_setenv(const char* name, const char* value);
int gp_os_execute(const char* command);
int gp_os_sleep(int seconds);
void gp_os_exit(int code);

// Network bindings
void* gp_net_http_get(const char* url);
void* gp_net_http_post(const char* url, const char* data);
char* gp_net_http_response_body(void* response);
int gp_net_http_response_status(void* response);
void gp_net_http_response_free(void* response);
void* gp_net_tcp_connect(const char* host, int port);
int gp_net_tcp_send(void* socket, const char* data);
char* gp_net_tcp_receive(void* socket);
void gp_net_tcp_close(void* socket);
char* gp_net_local_ip(void);
int gp_net_ping(const char* host);

// File System bindings
char* gp_fs_read(const char* path);
int gp_fs_write(const char* path, const char* content);
int gp_fs_copy(const char* src, const char* dest);
int gp_fs_delete(const char* path);
int gp_fs_exists(const char* path);
long gp_fs_size(const char* path);
char* gp_fs_join(const char* path1, const char* path2);
char* gp_fs_dirname(const char* path);
char* gp_fs_basename(const char* path);
void* gp_fs_list_dir(const char* path);
int gp_fs_dir_count(void* dir_list);
char* gp_fs_dir_name(void* dir_list, int index);
void gp_fs_dir_free(void* dir_list);

// JSON bindings
void* gp_json_parse(const char* json_string);
char* gp_json_stringify(void* json_value);
void* gp_json_object(void);
void* gp_json_array(void);
int gp_json_object_set_string(void* object, const char* key, const char* value);
int gp_json_object_set_number(void* object, const char* key, double value);
int gp_json_object_set_bool(void* object, const char* key, int value);
char* gp_json_object_get_string(void* object, const char* key);
double gp_json_object_get_number(void* object, const char* key);
int gp_json_object_get_bool(void* object, const char* key);
int gp_json_array_append_string(void* array, const char* value);
int gp_json_array_append_number(void* array, double value);
void* gp_json_array_get(void* array, int index);
int gp_json_array_size(void* array);
void gp_json_free(void* json_value);

// Error handling
int gp_get_last_error(void);
char* gp_get_error_string(void);

#endif // GPLANG_STDLIB_H
