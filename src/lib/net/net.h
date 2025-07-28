#ifndef GPLANG_NET_H
#define GPLANG_NET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

// HTTP Methods
typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_PATCH,
    HTTP_HEAD,
    HTTP_OPTIONS
} HttpMethod;

// HTTP Response
typedef struct {
    int status_code;
    char* status_text;
    char* headers;
    char* body;
    size_t body_length;
    double response_time;
} HttpResponse;

// HTTP Request
typedef struct {
    HttpMethod method;
    char* url;
    char* headers;
    char* body;
    int timeout;
    int follow_redirects;
} HttpRequest;

// Socket Information
typedef struct {
    int socket_fd;
    char* host;
    int port;
    int is_connected;
    int is_server;
    struct sockaddr_in address;
} SocketInfo;

// Network Interface
typedef struct {
    char* name;           // Interface name (eth0, wlan0, etc.)
    char* ip_address;     // IP address
    char* netmask;        // Network mask
    char* broadcast;      // Broadcast address
    char* mac_address;    // MAC address
    int is_up;            // Interface is up
    int is_loopback;      // Is loopback interface
    long bytes_sent;      // Bytes sent
    long bytes_received;  // Bytes received
} NetworkInterface;

// DNS Record
typedef struct {
    char* hostname;
    char* ip_address;
    int record_type;      // A, AAAA, CNAME, etc.
    int ttl;              // Time to live
} DnsRecord;

// Function declarations

// HTTP Client
HttpResponse* http_get(const char* url);
HttpResponse* http_post(const char* url, const char* data);
HttpResponse* http_put(const char* url, const char* data);
HttpResponse* http_delete(const char* url);
HttpResponse* http_request(HttpRequest* request);
HttpResponse* http_request_with_headers(const char* url, HttpMethod method, 
                                       const char* headers, const char* body);
void http_response_destroy(HttpResponse* response);
void http_request_destroy(HttpRequest* request);

// HTTP Server (Basic)
int http_server_create(int port);
int http_server_listen(int server_fd, void (*handler)(int client_fd));
int http_server_stop(int server_fd);
char* http_parse_request(const char* request, char** method, char** path, char** headers);
void http_send_response(int client_fd, int status_code, const char* headers, const char* body);

// TCP Sockets
SocketInfo* tcp_connect(const char* host, int port);
SocketInfo* tcp_server_create(int port);
SocketInfo* tcp_server_accept(SocketInfo* server);
int tcp_send(SocketInfo* socket, const char* data, size_t length);
char* tcp_receive(SocketInfo* socket, size_t* length);
int tcp_close(SocketInfo* socket);
void socket_info_destroy(SocketInfo* socket);

// UDP Sockets
SocketInfo* udp_create(void);
SocketInfo* udp_bind(int port);
int udp_send_to(SocketInfo* socket, const char* host, int port, const char* data, size_t length);
char* udp_receive_from(SocketInfo* socket, char** from_host, int* from_port, size_t* length);
int udp_close(SocketInfo* socket);

// Network Utilities
char* net_get_local_ip(void);
char* net_get_public_ip(void);
char* net_get_hostname(void);
char* net_resolve_hostname(const char* hostname);
DnsRecord** net_dns_lookup(const char* hostname, int* count);
void dns_records_destroy(DnsRecord** records, int count);

// Network Interfaces
NetworkInterface** net_get_interfaces(int* count);
NetworkInterface* net_get_interface(const char* name);
void network_interfaces_destroy(NetworkInterface** interfaces, int count);
void network_interface_destroy(NetworkInterface* interface);

// Network Testing
int net_ping(const char* host, int timeout);
double net_ping_time(const char* host, int timeout);
int net_port_scan(const char* host, int port, int timeout);
int net_is_connected(void);
double net_download_speed_test(const char* url);
double net_upload_speed_test(const char* url, const char* data);

// URL Utilities
char* url_encode(const char* str);
char* url_decode(const char* str);
int url_parse(const char* url, char** protocol, char** host, int* port, char** path);
char* url_join(const char* base, const char* path);

// Network Security
int net_is_ssl_supported(void);
HttpResponse* https_get(const char* url);
HttpResponse* https_post(const char* url, const char* data);

// Error Handling
char* net_get_error_string(int error_code);
int net_get_last_error(void);

#endif // GPLANG_NET_H
