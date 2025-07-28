#ifndef GPLANG_GRAPHQL_H
#define GPLANG_GRAPHQL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// GraphQL Types
typedef enum {
    GP_GQL_SCALAR,
    GP_GQL_OBJECT,
    GP_GQL_INTERFACE,
    GP_GQL_UNION,
    GP_GQL_ENUM,
    GP_GQL_INPUT_OBJECT,
    GP_GQL_LIST,
    GP_GQL_NON_NULL
} GPGraphQLType;

typedef enum {
    GP_GQL_STRING,
    GP_GQL_INT,
    GP_GQL_FLOAT,
    GP_GQL_BOOLEAN,
    GP_GQL_ID
} GPGraphQLScalarType;

// GraphQL Schema structures
typedef struct GPGraphQLField {
    char* name;
    char* type;
    char* description;
    bool is_required;
    char** arguments;
    int argument_count;
    struct GPGraphQLField* next;
} GPGraphQLField;

typedef struct {
    char* name;
    char* description;
    GPGraphQLField* fields;
    char** interfaces;
    int interface_count;
} GPGraphQLObjectType;

typedef struct {
    char* name;
    char* description;
    char** values;
    int value_count;
} GPGraphQLEnumType;

typedef struct {
    GPGraphQLObjectType* objects;
    int object_count;
    GPGraphQLEnumType* enums;
    int enum_count;
    char* query_type;
    char* mutation_type;
    char* subscription_type;
} GPGraphQLSchema;

// GraphQL Query structures
typedef struct GPGraphQLArgument {
    char* name;
    char* value;
    struct GPGraphQLArgument* next;
} GPGraphQLArgument;

typedef struct GPGraphQLSelection {
    char* name;
    char* alias;
    GPGraphQLArgument* arguments;
    struct GPGraphQLSelection* selections;
    struct GPGraphQLSelection* next;
} GPGraphQLSelection;

typedef struct {
    char* operation_type;  // "query", "mutation", "subscription"
    char* operation_name;
    GPGraphQLSelection* selections;
    char** variables;
    int variable_count;
} GPGraphQLQuery;

// GraphQL Response structures
typedef struct {
    char* data;
    char* errors;
    char* extensions;
} GPGraphQLResponse;

// GraphQL Client
typedef struct {
    char* endpoint;
    char** headers;
    int header_count;
    int timeout;
    bool use_websocket;
} GPGraphQLClient;

// Schema Definition Language (SDL) functions
GPGraphQLSchema* gp_graphql_parse_schema(const char* sdl);
char* gp_graphql_schema_to_sdl(const GPGraphQLSchema* schema);
void gp_graphql_schema_destroy(GPGraphQLSchema* schema);

// Schema building functions
GPGraphQLSchema* gp_graphql_schema_create(void);
void gp_graphql_schema_add_object_type(GPGraphQLSchema* schema, const GPGraphQLObjectType* type);
void gp_graphql_schema_add_enum_type(GPGraphQLSchema* schema, const GPGraphQLEnumType* type);
void gp_graphql_schema_set_query_type(GPGraphQLSchema* schema, const char* type_name);
void gp_graphql_schema_set_mutation_type(GPGraphQLSchema* schema, const char* type_name);
void gp_graphql_schema_set_subscription_type(GPGraphQLSchema* schema, const char* type_name);

// Object type building
GPGraphQLObjectType* gp_graphql_object_type_create(const char* name, const char* description);
void gp_graphql_object_type_add_field(GPGraphQLObjectType* type, const char* name, 
                                      const char* field_type, const char* description, bool required);
void gp_graphql_object_type_add_interface(GPGraphQLObjectType* type, const char* interface_name);
void gp_graphql_object_type_destroy(GPGraphQLObjectType* type);

// Enum type building
GPGraphQLEnumType* gp_graphql_enum_type_create(const char* name, const char* description);
void gp_graphql_enum_type_add_value(GPGraphQLEnumType* type, const char* value);
void gp_graphql_enum_type_destroy(GPGraphQLEnumType* type);

// Query building functions
GPGraphQLQuery* gp_graphql_query_create(const char* operation_type, const char* operation_name);
void gp_graphql_query_add_selection(GPGraphQLQuery* query, const char* field_name, const char* alias);
void gp_graphql_query_add_argument(GPGraphQLQuery* query, const char* field_name, 
                                  const char* arg_name, const char* arg_value);
void gp_graphql_query_add_variable(GPGraphQLQuery* query, const char* variable_name, const char* variable_type);
char* gp_graphql_query_to_string(const GPGraphQLQuery* query);
void gp_graphql_query_destroy(GPGraphQLQuery* query);

// Query parsing functions
GPGraphQLQuery* gp_graphql_parse_query(const char* query_string);
bool gp_graphql_validate_query(const GPGraphQLQuery* query, const GPGraphQLSchema* schema);

// Client functions
GPGraphQLClient* gp_graphql_client_create(const char* endpoint);
void gp_graphql_client_add_header(GPGraphQLClient* client, const char* name, const char* value);
void gp_graphql_client_set_timeout(GPGraphQLClient* client, int timeout_seconds);
void gp_graphql_client_enable_websocket(GPGraphQLClient* client, bool enable);
void gp_graphql_client_destroy(GPGraphQLClient* client);

// Request execution
GPGraphQLResponse* gp_graphql_execute(GPGraphQLClient* client, const GPGraphQLQuery* query, 
                                     const char* variables_json);
GPGraphQLResponse* gp_graphql_execute_string(GPGraphQLClient* client, const char* query_string, 
                                           const char* variables_json);

// Subscription handling
typedef void (*GPGraphQLSubscriptionCallback)(const char* data, void* user_data);
int gp_graphql_subscribe(GPGraphQLClient* client, const GPGraphQLQuery* subscription, 
                        GPGraphQLSubscriptionCallback callback, void* user_data);
void gp_graphql_unsubscribe(GPGraphQLClient* client, int subscription_id);

// Response handling
void gp_graphql_response_destroy(GPGraphQLResponse* response);
char* gp_graphql_response_get_data(const GPGraphQLResponse* response);
char* gp_graphql_response_get_errors(const GPGraphQLResponse* response);
bool gp_graphql_response_has_errors(const GPGraphQLResponse* response);

// Introspection
GPGraphQLQuery* gp_graphql_introspection_query(void);
GPGraphQLSchema* gp_graphql_introspect_schema(GPGraphQLClient* client);

// Utility functions
char* gp_graphql_escape_string(const char* str);
char* gp_graphql_format_variables(const char* json);
bool gp_graphql_is_valid_name(const char* name);
char* gp_graphql_generate_query_id(void);

// Error handling
typedef enum {
    GP_GQL_ERROR_NONE,
    GP_GQL_ERROR_PARSE,
    GP_GQL_ERROR_VALIDATION,
    GP_GQL_ERROR_EXECUTION,
    GP_GQL_ERROR_NETWORK,
    GP_GQL_ERROR_TIMEOUT
} GPGraphQLErrorType;

typedef struct {
    GPGraphQLErrorType type;
    char* message;
    int line;
    int column;
} GPGraphQLError;

GPGraphQLError* gp_graphql_get_last_error(void);
void gp_graphql_clear_error(void);

// Fragment support
typedef struct GPGraphQLFragment {
    char* name;
    char* type_condition;
    GPGraphQLSelection* selections;
    struct GPGraphQLFragment* next;
} GPGraphQLFragment;

void gp_graphql_query_add_fragment(GPGraphQLQuery* query, const GPGraphQLFragment* fragment);
GPGraphQLFragment* gp_graphql_fragment_create(const char* name, const char* type_condition);
void gp_graphql_fragment_add_selection(GPGraphQLFragment* fragment, const char* field_name);
void gp_graphql_fragment_destroy(GPGraphQLFragment* fragment);

// Directive support
typedef struct GPGraphQLDirective {
    char* name;
    GPGraphQLArgument* arguments;
    struct GPGraphQLDirective* next;
} GPGraphQLDirective;

void gp_graphql_selection_add_directive(GPGraphQLSelection* selection, const GPGraphQLDirective* directive);
GPGraphQLDirective* gp_graphql_directive_create(const char* name);
void gp_graphql_directive_add_argument(GPGraphQLDirective* directive, const char* name, const char* value);
void gp_graphql_directive_destroy(GPGraphQLDirective* directive);

// Batch operations
typedef struct {
    GPGraphQLQuery** queries;
    int query_count;
    char* batch_id;
} GPGraphQLBatch;

GPGraphQLBatch* gp_graphql_batch_create(void);
void gp_graphql_batch_add_query(GPGraphQLBatch* batch, const GPGraphQLQuery* query);
GPGraphQLResponse** gp_graphql_execute_batch(GPGraphQLClient* client, const GPGraphQLBatch* batch);
void gp_graphql_batch_destroy(GPGraphQLBatch* batch);

// Caching
typedef struct {
    char* query_hash;
    char* response_data;
    time_t timestamp;
    int ttl_seconds;
} GPGraphQLCacheEntry;

void gp_graphql_enable_caching(GPGraphQLClient* client, int default_ttl);
void gp_graphql_clear_cache(GPGraphQLClient* client);
void gp_graphql_set_cache_ttl(GPGraphQLClient* client, const char* query_hash, int ttl);

// Performance monitoring
typedef struct {
    double query_time;
    double network_time;
    double parse_time;
    size_t response_size;
    int cache_hits;
    int cache_misses;
} GPGraphQLMetrics;

GPGraphQLMetrics* gp_graphql_get_metrics(GPGraphQLClient* client);
void gp_graphql_reset_metrics(GPGraphQLClient* client);

#endif // GPLANG_GRAPHQL_H
