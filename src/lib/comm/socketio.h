#ifndef GPLANG_SOCKETIO_H
#define GPLANG_SOCKETIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

// Socket.IO packet types
typedef enum {
    GP_SIO_CONNECT = 0,
    GP_SIO_DISCONNECT = 1,
    GP_SIO_EVENT = 2,
    GP_SIO_ACK = 3,
    GP_SIO_CONNECT_ERROR = 4,
    GP_SIO_BINARY_EVENT = 5,
    GP_SIO_BINARY_ACK = 6
} GPSocketIOPacketType;

// Engine.IO packet types
typedef enum {
    GP_EIO_OPEN = 0,
    GP_EIO_CLOSE = 1,
    GP_EIO_PING = 2,
    GP_EIO_PONG = 3,
    GP_EIO_MESSAGE = 4,
    GP_EIO_UPGRADE = 5,
    GP_EIO_NOOP = 6
} GPEngineIOPacketType;

// Transport types
typedef enum {
    GP_SIO_TRANSPORT_POLLING,
    GP_SIO_TRANSPORT_WEBSOCKET
} GPSocketIOTransport;

// Connection states
typedef enum {
    GP_SIO_DISCONNECTED,
    GP_SIO_CONNECTING,
    GP_SIO_CONNECTED,
    GP_SIO_DISCONNECTING,
    GP_SIO_RECONNECTING,
    GP_SIO_ERROR
} GPSocketIOState;

// Socket.IO packet structure
typedef struct {
    GPSocketIOPacketType type;
    char* namespace;
    int id;
    char* event;
    char** data;
    int data_count;
    uint8_t** binary_data;
    size_t* binary_lengths;
    int binary_count;
} GPSocketIOPacket;

// Socket.IO configuration
typedef struct {
    char* path;
    char* namespace;
    bool auto_connect;
    bool reconnection;
    int reconnection_attempts;
    int reconnection_delay;
    int reconnection_delay_max;
    double randomization_factor;
    int timeout;
    bool force_new;
    bool multiplex;
    char** transports;
    int transport_count;
    bool upgrade;
    bool remember_upgrade;
    char* query;
    bool force_base64;
    int ping_interval;
    int ping_timeout;
} GPSocketIOConfig;

// Forward declaration
typedef struct GPSocketIO GPSocketIO;

// Event callback types
typedef void (*GPSocketIOOnConnect)(GPSocketIO* sio, void* user_data);
typedef void (*GPSocketIOOnDisconnect)(GPSocketIO* sio, const char* reason, void* user_data);
typedef void (*GPSocketIOOnEvent)(GPSocketIO* sio, const char* event, const char** data, int data_count, void* user_data);
typedef void (*GPSocketIOOnError)(GPSocketIO* sio, const char* error, void* user_data);
typedef void (*GPSocketIOOnReconnect)(GPSocketIO* sio, int attempt, void* user_data);
typedef void (*GPSocketIOOnReconnectError)(GPSocketIO* sio, const char* error, void* user_data);
typedef void (*GPSocketIOOnReconnectFailed)(GPSocketIO* sio, void* user_data);
typedef void (*GPSocketIOOnPing)(GPSocketIO* sio, void* user_data);
typedef void (*GPSocketIOOnPong)(GPSocketIO* sio, int latency, void* user_data);

// Socket.IO client structure
struct GPSocketIO {
    char* url;
    GPSocketIOConfig config;
    GPSocketIOState state;
    GPSocketIOTransport transport;
    void* transport_handle;  // WebSocket or HTTP client
    
    // Session info
    char* session_id;
    char* socket_id;
    int ping_interval;
    int ping_timeout;
    time_t last_ping;
    time_t last_pong;
    
    // Event callbacks
    GPSocketIOOnConnect on_connect;
    GPSocketIOOnDisconnect on_disconnect;
    GPSocketIOOnEvent on_event;
    GPSocketIOOnError on_error;
    GPSocketIOOnReconnect on_reconnect;
    GPSocketIOOnReconnectError on_reconnect_error;
    GPSocketIOOnReconnectFailed on_reconnect_failed;
    GPSocketIOOnPing on_ping;
    GPSocketIOOnPong on_pong;
    void* user_data;
    
    // Event handlers map
    struct {
        char* event_name;
        GPSocketIOOnEvent handler;
        void* user_data;
    }* event_handlers;
    int event_handler_count;
    
    // Acknowledgment callbacks
    struct {
        int id;
        void (*callback)(const char** data, int data_count, void* user_data);
        void* user_data;
        time_t timestamp;
        int timeout;
    }* ack_callbacks;
    int ack_callback_count;
    int next_ack_id;
    
    // Reconnection state
    int reconnection_attempts_made;
    time_t last_reconnect_attempt;
    bool is_reconnecting;
    
    // Statistics
    uint64_t packets_sent;
    uint64_t packets_received;
    uint64_t bytes_sent;
    uint64_t bytes_received;
    time_t connected_at;
    int total_reconnections;
};

// Client creation and management
GPSocketIO* gp_socketio_create(const char* url);
void gp_socketio_destroy(GPSocketIO* sio);
int gp_socketio_connect(GPSocketIO* sio);
int gp_socketio_disconnect(GPSocketIO* sio);
GPSocketIOState gp_socketio_get_state(const GPSocketIO* sio);
bool gp_socketio_is_connected(const GPSocketIO* sio);

// Configuration
void gp_socketio_set_config(GPSocketIO* sio, const GPSocketIOConfig* config);
GPSocketIOConfig gp_socketio_get_default_config(void);
void gp_socketio_set_namespace(GPSocketIO* sio, const char* namespace);
void gp_socketio_set_auto_connect(GPSocketIO* sio, bool auto_connect);
void gp_socketio_set_reconnection(GPSocketIO* sio, bool enable, int attempts, int delay);
void gp_socketio_set_timeout(GPSocketIO* sio, int timeout);
void gp_socketio_set_transports(GPSocketIO* sio, const char** transports, int count);
void gp_socketio_set_query(GPSocketIO* sio, const char* query);

// Event callbacks
void gp_socketio_on_connect(GPSocketIO* sio, GPSocketIOOnConnect callback, void* user_data);
void gp_socketio_on_disconnect(GPSocketIO* sio, GPSocketIOOnDisconnect callback, void* user_data);
void gp_socketio_on_error(GPSocketIO* sio, GPSocketIOOnError callback, void* user_data);
void gp_socketio_on_reconnect(GPSocketIO* sio, GPSocketIOOnReconnect callback, void* user_data);
void gp_socketio_on_reconnect_error(GPSocketIO* sio, GPSocketIOOnReconnectError callback, void* user_data);
void gp_socketio_on_reconnect_failed(GPSocketIO* sio, GPSocketIOOnReconnectFailed callback, void* user_data);
void gp_socketio_on_ping(GPSocketIO* sio, GPSocketIOOnPing callback, void* user_data);
void gp_socketio_on_pong(GPSocketIO* sio, GPSocketIOOnPong callback, void* user_data);

// Event handling
void gp_socketio_on(GPSocketIO* sio, const char* event, GPSocketIOOnEvent callback, void* user_data);
void gp_socketio_off(GPSocketIO* sio, const char* event);
void gp_socketio_once(GPSocketIO* sio, const char* event, GPSocketIOOnEvent callback, void* user_data);

// Message sending
int gp_socketio_emit(GPSocketIO* sio, const char* event, const char** data, int data_count);
int gp_socketio_emit_with_ack(GPSocketIO* sio, const char* event, const char** data, int data_count,
                             void (*ack_callback)(const char** data, int data_count, void* user_data),
                             void* user_data, int timeout);
int gp_socketio_emit_binary(GPSocketIO* sio, const char* event, const uint8_t** binary_data, 
                           const size_t* binary_lengths, int binary_count);

// Convenience functions for common data types
int gp_socketio_emit_string(GPSocketIO* sio, const char* event, const char* data);
int gp_socketio_emit_json(GPSocketIO* sio, const char* event, const char* json);
int gp_socketio_emit_number(GPSocketIO* sio, const char* event, double number);
int gp_socketio_emit_boolean(GPSocketIO* sio, const char* event, bool value);

// Namespace support
GPSocketIO* gp_socketio_of(GPSocketIO* sio, const char* namespace);
void gp_socketio_join_room(GPSocketIO* sio, const char* room);
void gp_socketio_leave_room(GPSocketIO* sio, const char* room);
void gp_socketio_to_room(GPSocketIO* sio, const char* room, const char* event, const char** data, int data_count);

// Packet handling (low-level)
GPSocketIOPacket* gp_socketio_packet_create(GPSocketIOPacketType type);
void gp_socketio_packet_destroy(GPSocketIOPacket* packet);
void gp_socketio_packet_set_namespace(GPSocketIOPacket* packet, const char* namespace);
void gp_socketio_packet_set_id(GPSocketIOPacket* packet, int id);
void gp_socketio_packet_set_event(GPSocketIOPacket* packet, const char* event);
void gp_socketio_packet_add_data(GPSocketIOPacket* packet, const char* data);
void gp_socketio_packet_add_binary_data(GPSocketIOPacket* packet, const uint8_t* data, size_t length);
char* gp_socketio_packet_encode(const GPSocketIOPacket* packet);
GPSocketIOPacket* gp_socketio_packet_decode(const char* encoded);

// Server support
typedef struct GPSocketIOServer GPSocketIOServer;
typedef void (*GPSocketIOServerOnConnection)(GPSocketIOServer* server, GPSocketIO* client, void* user_data);

struct GPSocketIOServer {
    int port;
    char* path;
    bool is_running;
    GPSocketIOConfig default_config;
    GPSocketIOServerOnConnection on_connection;
    void* user_data;
    
    // Connected clients
    GPSocketIO** clients;
    int client_count;
    int max_clients;
    
    // Namespaces
    struct {
        char* name;
        GPSocketIO** clients;
        int client_count;
    }* namespaces;
    int namespace_count;
    
    // Rooms
    struct {
        char* name;
        char* namespace;
        GPSocketIO** clients;
        int client_count;
    }* rooms;
    int room_count;
};

GPSocketIOServer* gp_socketio_server_create(int port);
void gp_socketio_server_destroy(GPSocketIOServer* server);
int gp_socketio_server_start(GPSocketIOServer* server);
int gp_socketio_server_stop(GPSocketIOServer* server);
void gp_socketio_server_on_connection(GPSocketIOServer* server, GPSocketIOServerOnConnection callback, void* user_data);
void gp_socketio_server_set_path(GPSocketIOServer* server, const char* path);
void gp_socketio_server_set_max_clients(GPSocketIOServer* server, int max_clients);

// Server broadcasting
int gp_socketio_server_emit(GPSocketIOServer* server, const char* event, const char** data, int data_count);
int gp_socketio_server_emit_to_namespace(GPSocketIOServer* server, const char* namespace, 
                                        const char* event, const char** data, int data_count);
int gp_socketio_server_emit_to_room(GPSocketIOServer* server, const char* room, 
                                   const char* event, const char** data, int data_count);

// Room management
void gp_socketio_server_create_room(GPSocketIOServer* server, const char* room, const char* namespace);
void gp_socketio_server_delete_room(GPSocketIOServer* server, const char* room);
void gp_socketio_server_join_room(GPSocketIOServer* server, GPSocketIO* client, const char* room);
void gp_socketio_server_leave_room(GPSocketIOServer* server, GPSocketIO* client, const char* room);
char** gp_socketio_server_get_rooms(GPSocketIOServer* server, const char* namespace, int* count);
GPSocketIO** gp_socketio_server_get_clients_in_room(GPSocketIOServer* server, const char* room, int* count);

// Statistics and monitoring
typedef struct {
    uint64_t total_connections;
    uint64_t active_connections;
    uint64_t total_packets_sent;
    uint64_t total_packets_received;
    uint64_t total_bytes_sent;
    uint64_t total_bytes_received;
    int total_reconnections;
    double average_connection_time;
    int active_namespaces;
    int active_rooms;
} GPSocketIOStats;

GPSocketIOStats gp_socketio_get_stats(const GPSocketIO* sio);
GPSocketIOStats gp_socketio_server_get_stats(const GPSocketIOServer* server);
void gp_socketio_reset_stats(GPSocketIO* sio);

// Utility functions
bool gp_socketio_is_valid_event_name(const char* event);
char* gp_socketio_generate_id(void);
int gp_socketio_get_latency(const GPSocketIO* sio);
time_t gp_socketio_get_uptime(const GPSocketIO* sio);

// Middleware support (server-side)
typedef bool (*GPSocketIOMiddleware)(GPSocketIO* client, GPSocketIOPacket* packet, void* user_data);
void gp_socketio_server_use(GPSocketIOServer* server, GPSocketIOMiddleware middleware, void* user_data);

// Error handling
typedef enum {
    GP_SIO_ERROR_NONE,
    GP_SIO_ERROR_INVALID_URL,
    GP_SIO_ERROR_CONNECTION_FAILED,
    GP_SIO_ERROR_HANDSHAKE_FAILED,
    GP_SIO_ERROR_TRANSPORT_ERROR,
    GP_SIO_ERROR_PROTOCOL_ERROR,
    GP_SIO_ERROR_TIMEOUT,
    GP_SIO_ERROR_RECONNECTION_FAILED,
    GP_SIO_ERROR_INVALID_PACKET,
    GP_SIO_ERROR_NAMESPACE_ERROR
} GPSocketIOErrorCode;

typedef struct {
    GPSocketIOErrorCode code;
    char* message;
    char* details;
} GPSocketIOError;

GPSocketIOError* gp_socketio_get_last_error(void);
void gp_socketio_clear_error(void);
const char* gp_socketio_error_string(GPSocketIOErrorCode code);

#endif // GPLANG_SOCKETIO_H
