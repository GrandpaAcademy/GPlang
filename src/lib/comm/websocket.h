#ifndef GPLANG_WEBSOCKET_H
#define GPLANG_WEBSOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

// WebSocket connection states
typedef enum {
    GP_WS_CONNECTING,
    GP_WS_OPEN,
    GP_WS_CLOSING,
    GP_WS_CLOSED,
    GP_WS_ERROR
} GPWebSocketState;

// WebSocket frame types
typedef enum {
    GP_WS_FRAME_CONTINUATION = 0x0,
    GP_WS_FRAME_TEXT = 0x1,
    GP_WS_FRAME_BINARY = 0x2,
    GP_WS_FRAME_CLOSE = 0x8,
    GP_WS_FRAME_PING = 0x9,
    GP_WS_FRAME_PONG = 0xA
} GPWebSocketFrameType;

// WebSocket close codes
typedef enum {
    GP_WS_CLOSE_NORMAL = 1000,
    GP_WS_CLOSE_GOING_AWAY = 1001,
    GP_WS_CLOSE_PROTOCOL_ERROR = 1002,
    GP_WS_CLOSE_UNSUPPORTED_DATA = 1003,
    GP_WS_CLOSE_NO_STATUS = 1005,
    GP_WS_CLOSE_ABNORMAL = 1006,
    GP_WS_CLOSE_INVALID_PAYLOAD = 1007,
    GP_WS_CLOSE_POLICY_VIOLATION = 1008,
    GP_WS_CLOSE_MESSAGE_TOO_BIG = 1009,
    GP_WS_CLOSE_EXTENSION_REQUIRED = 1010,
    GP_WS_CLOSE_INTERNAL_ERROR = 1011,
    GP_WS_CLOSE_SERVICE_RESTART = 1012,
    GP_WS_CLOSE_TRY_AGAIN_LATER = 1013,
    GP_WS_CLOSE_TLS_HANDSHAKE = 1015
} GPWebSocketCloseCode;

// WebSocket message structure
typedef struct {
    GPWebSocketFrameType type;
    uint8_t* data;
    size_t length;
    bool is_final;
    time_t timestamp;
} GPWebSocketMessage;

// WebSocket configuration
typedef struct {
    char* subprotocols;
    char* extensions;
    char** headers;
    int header_count;
    int ping_interval;
    int pong_timeout;
    int max_message_size;
    int max_frame_size;
    bool auto_reconnect;
    int reconnect_delay;
    int max_reconnect_attempts;
    bool compression_enabled;
    bool mask_client_frames;
} GPWebSocketConfig;

// Forward declaration
typedef struct GPWebSocket GPWebSocket;

// WebSocket event callbacks
typedef void (*GPWebSocketOnOpen)(GPWebSocket* ws, void* user_data);
typedef void (*GPWebSocketOnMessage)(GPWebSocket* ws, const GPWebSocketMessage* message, void* user_data);
typedef void (*GPWebSocketOnClose)(GPWebSocket* ws, GPWebSocketCloseCode code, const char* reason, void* user_data);
typedef void (*GPWebSocketOnError)(GPWebSocket* ws, const char* error, void* user_data);
typedef void (*GPWebSocketOnPing)(GPWebSocket* ws, const uint8_t* data, size_t length, void* user_data);
typedef void (*GPWebSocketOnPong)(GPWebSocket* ws, const uint8_t* data, size_t length, void* user_data);

// WebSocket structure
struct GPWebSocket {
    char* url;
    GPWebSocketState state;
    GPWebSocketConfig config;
    int socket_fd;
    pthread_t thread;
    pthread_mutex_t send_mutex;
    pthread_mutex_t state_mutex;
    bool is_server;
    bool is_secure;
    
    // Event callbacks
    GPWebSocketOnOpen on_open;
    GPWebSocketOnMessage on_message;
    GPWebSocketOnClose on_close;
    GPWebSocketOnError on_error;
    GPWebSocketOnPing on_ping;
    GPWebSocketOnPong on_pong;
    void* user_data;
    
    // Connection info
    char* host;
    int port;
    char* path;
    char* origin;
    char* sec_websocket_key;
    char* sec_websocket_accept;
    
    // Statistics
    uint64_t bytes_sent;
    uint64_t bytes_received;
    uint64_t messages_sent;
    uint64_t messages_received;
    time_t connected_at;
    time_t last_ping;
    time_t last_pong;
    
    // Fragmentation support
    uint8_t* fragment_buffer;
    size_t fragment_length;
    GPWebSocketFrameType fragment_type;
    
    // Compression context
    void* compression_context;
};

// WebSocket client functions
GPWebSocket* gp_websocket_create(const char* url);
void gp_websocket_destroy(GPWebSocket* ws);
int gp_websocket_connect(GPWebSocket* ws);
int gp_websocket_disconnect(GPWebSocket* ws, GPWebSocketCloseCode code, const char* reason);
GPWebSocketState gp_websocket_get_state(const GPWebSocket* ws);

// Configuration functions
void gp_websocket_set_config(GPWebSocket* ws, const GPWebSocketConfig* config);
GPWebSocketConfig gp_websocket_get_default_config(void);
void gp_websocket_set_subprotocol(GPWebSocket* ws, const char* subprotocol);
void gp_websocket_set_origin(GPWebSocket* ws, const char* origin);
void gp_websocket_add_header(GPWebSocket* ws, const char* name, const char* value);
void gp_websocket_enable_compression(GPWebSocket* ws, bool enable);
void gp_websocket_set_ping_interval(GPWebSocket* ws, int seconds);
void gp_websocket_set_auto_reconnect(GPWebSocket* ws, bool enable, int delay, int max_attempts);

// Event callback functions
void gp_websocket_set_on_open(GPWebSocket* ws, GPWebSocketOnOpen callback, void* user_data);
void gp_websocket_set_on_message(GPWebSocket* ws, GPWebSocketOnMessage callback, void* user_data);
void gp_websocket_set_on_close(GPWebSocket* ws, GPWebSocketOnClose callback, void* user_data);
void gp_websocket_set_on_error(GPWebSocket* ws, GPWebSocketOnError callback, void* user_data);
void gp_websocket_set_on_ping(GPWebSocket* ws, GPWebSocketOnPing callback, void* user_data);
void gp_websocket_set_on_pong(GPWebSocket* ws, GPWebSocketOnPong callback, void* user_data);

// Message sending functions
int gp_websocket_send_text(GPWebSocket* ws, const char* text);
int gp_websocket_send_binary(GPWebSocket* ws, const uint8_t* data, size_t length);
int gp_websocket_send_message(GPWebSocket* ws, const GPWebSocketMessage* message);
int gp_websocket_send_ping(GPWebSocket* ws, const uint8_t* data, size_t length);
int gp_websocket_send_pong(GPWebSocket* ws, const uint8_t* data, size_t length);

// Fragmented message support
int gp_websocket_send_text_fragmented(GPWebSocket* ws, const char* text, size_t chunk_size);
int gp_websocket_send_binary_fragmented(GPWebSocket* ws, const uint8_t* data, size_t length, size_t chunk_size);

// Message creation and destruction
GPWebSocketMessage* gp_websocket_message_create(GPWebSocketFrameType type, const uint8_t* data, size_t length);
GPWebSocketMessage* gp_websocket_message_create_text(const char* text);
GPWebSocketMessage* gp_websocket_message_create_binary(const uint8_t* data, size_t length);
void gp_websocket_message_destroy(GPWebSocketMessage* message);
char* gp_websocket_message_to_text(const GPWebSocketMessage* message);

// WebSocket server functions
typedef struct GPWebSocketServer GPWebSocketServer;
typedef void (*GPWebSocketServerOnConnection)(GPWebSocketServer* server, GPWebSocket* client, void* user_data);

struct GPWebSocketServer {
    int port;
    int socket_fd;
    pthread_t thread;
    bool is_running;
    GPWebSocketConfig default_config;
    GPWebSocketServerOnConnection on_connection;
    void* user_data;
    GPWebSocket** clients;
    int client_count;
    int max_clients;
    pthread_mutex_t clients_mutex;
};

GPWebSocketServer* gp_websocket_server_create(int port);
void gp_websocket_server_destroy(GPWebSocketServer* server);
int gp_websocket_server_start(GPWebSocketServer* server);
int gp_websocket_server_stop(GPWebSocketServer* server);
void gp_websocket_server_set_on_connection(GPWebSocketServer* server, GPWebSocketServerOnConnection callback, void* user_data);
void gp_websocket_server_set_max_clients(GPWebSocketServer* server, int max_clients);
int gp_websocket_server_broadcast_text(GPWebSocketServer* server, const char* text);
int gp_websocket_server_broadcast_binary(GPWebSocketServer* server, const uint8_t* data, size_t length);
GPWebSocket** gp_websocket_server_get_clients(GPWebSocketServer* server, int* count);

// Statistics and monitoring
typedef struct {
    uint64_t total_connections;
    uint64_t active_connections;
    uint64_t total_messages_sent;
    uint64_t total_messages_received;
    uint64_t total_bytes_sent;
    uint64_t total_bytes_received;
    double average_message_size;
    double connection_uptime;
    int reconnection_count;
} GPWebSocketStats;

GPWebSocketStats gp_websocket_get_stats(const GPWebSocket* ws);
void gp_websocket_reset_stats(GPWebSocket* ws);

// Utility functions
bool gp_websocket_is_valid_url(const char* url);
char* gp_websocket_generate_key(void);
char* gp_websocket_calculate_accept(const char* key);
bool gp_websocket_validate_accept(const char* key, const char* accept);
char* gp_websocket_encode_close_frame(GPWebSocketCloseCode code, const char* reason);
void gp_websocket_decode_close_frame(const uint8_t* data, size_t length, GPWebSocketCloseCode* code, char** reason);

// Frame handling (low-level)
typedef struct {
    bool fin;
    bool rsv1;
    bool rsv2;
    bool rsv3;
    uint8_t opcode;
    bool mask;
    uint64_t payload_length;
    uint8_t masking_key[4];
    uint8_t* payload;
} GPWebSocketFrame;

GPWebSocketFrame* gp_websocket_frame_create(void);
void gp_websocket_frame_destroy(GPWebSocketFrame* frame);
int gp_websocket_frame_encode(const GPWebSocketFrame* frame, uint8_t** output, size_t* output_length);
int gp_websocket_frame_decode(const uint8_t* data, size_t length, GPWebSocketFrame* frame);
void gp_websocket_frame_mask(uint8_t* data, size_t length, const uint8_t* mask);

// Extension support
typedef struct {
    char* name;
    char* parameters;
    bool (*negotiate)(const char* client_request, char** server_response);
    int (*compress)(const uint8_t* input, size_t input_length, uint8_t** output, size_t* output_length);
    int (*decompress)(const uint8_t* input, size_t input_length, uint8_t** output, size_t* output_length);
} GPWebSocketExtension;

void gp_websocket_register_extension(const GPWebSocketExtension* extension);
void gp_websocket_enable_extension(GPWebSocket* ws, const char* extension_name);
char* gp_websocket_negotiate_extensions(const char* client_extensions);

// Error handling
typedef enum {
    GP_WS_ERROR_NONE,
    GP_WS_ERROR_INVALID_URL,
    GP_WS_ERROR_CONNECTION_FAILED,
    GP_WS_ERROR_HANDSHAKE_FAILED,
    GP_WS_ERROR_PROTOCOL_ERROR,
    GP_WS_ERROR_MESSAGE_TOO_LARGE,
    GP_WS_ERROR_INVALID_UTF8,
    GP_WS_ERROR_COMPRESSION_ERROR,
    GP_WS_ERROR_NETWORK_ERROR,
    GP_WS_ERROR_TIMEOUT
} GPWebSocketErrorCode;

typedef struct {
    GPWebSocketErrorCode code;
    char* message;
    int system_error;
} GPWebSocketError;

GPWebSocketError* gp_websocket_get_last_error(void);
void gp_websocket_clear_error(void);
const char* gp_websocket_error_string(GPWebSocketErrorCode code);

#endif // GPLANG_WEBSOCKET_H
