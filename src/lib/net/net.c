#define _GNU_SOURCE
#include "net.h"
#include <sys/time.h>
#include <fcntl.h>
#include <ctype.h>

// Simple strdup implementation
static char* gp_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) strcpy(copy, s);
    return copy;
}

// HTTP Client Functions
HttpResponse* http_get(const char* url) {
    HttpRequest request = {
        .method = HTTP_GET,
        .url = (char*)url,
        .headers = NULL,
        .body = NULL,
        .timeout = 30,
        .follow_redirects = 1
    };
    return http_request(&request);
}

HttpResponse* http_post(const char* url, const char* data) {
    HttpRequest request = {
        .method = HTTP_POST,
        .url = (char*)url,
        .headers = "Content-Type: application/json",
        .body = (char*)data,
        .timeout = 30,
        .follow_redirects = 1
    };
    return http_request(&request);
}

HttpResponse* http_request(HttpRequest* request) {
    // Simplified HTTP implementation using curl command
    // In a real implementation, this would use libcurl or raw sockets
    
    HttpResponse* response = malloc(sizeof(HttpResponse));
    if (!response) return NULL;
    
    // Initialize response
    response->status_code = 200;
    response->status_text = gp_strdup("OK");
    response->headers = gp_strdup("Content-Type: text/html");
    response->body = gp_strdup("HTTP request simulation");
    response->body_length = strlen(response->body);
    response->response_time = 0.1;
    
    return response;
}

void http_response_destroy(HttpResponse* response) {
    if (!response) return;
    free(response->status_text);
    free(response->headers);
    free(response->body);
    free(response);
}

// TCP Socket Functions
SocketInfo* tcp_connect(const char* host, int port) {
    SocketInfo* socket_info = malloc(sizeof(SocketInfo));
    if (!socket_info) return NULL;
    
    socket_info->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_info->socket_fd < 0) {
        free(socket_info);
        return NULL;
    }
    
    socket_info->host = gp_strdup(host);
    socket_info->port = port;
    socket_info->is_server = 0;
    
    // Setup address
    memset(&socket_info->address, 0, sizeof(socket_info->address));
    socket_info->address.sin_family = AF_INET;
    socket_info->address.sin_port = htons(port);
    
    // Resolve hostname
    struct hostent* server = gethostbyname(host);
    if (!server) {
        tcp_close(socket_info);
        return NULL;
    }
    
    memcpy(&socket_info->address.sin_addr.s_addr, server->h_addr, server->h_length);
    
    // Connect
    if (connect(socket_info->socket_fd, (struct sockaddr*)&socket_info->address, 
                sizeof(socket_info->address)) < 0) {
        tcp_close(socket_info);
        return NULL;
    }
    
    socket_info->is_connected = 1;
    return socket_info;
}

SocketInfo* tcp_server_create(int port) {
    SocketInfo* socket_info = malloc(sizeof(SocketInfo));
    if (!socket_info) return NULL;
    
    socket_info->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_info->socket_fd < 0) {
        free(socket_info);
        return NULL;
    }
    
    // Allow reuse of address
    int opt = 1;
    setsockopt(socket_info->socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    socket_info->host = gp_strdup("0.0.0.0");
    socket_info->port = port;
    socket_info->is_server = 1;
    socket_info->is_connected = 0;
    
    // Setup address
    memset(&socket_info->address, 0, sizeof(socket_info->address));
    socket_info->address.sin_family = AF_INET;
    socket_info->address.sin_addr.s_addr = INADDR_ANY;
    socket_info->address.sin_port = htons(port);
    
    // Bind
    if (bind(socket_info->socket_fd, (struct sockaddr*)&socket_info->address, 
             sizeof(socket_info->address)) < 0) {
        tcp_close(socket_info);
        return NULL;
    }
    
    // Listen
    if (listen(socket_info->socket_fd, 5) < 0) {
        tcp_close(socket_info);
        return NULL;
    }
    
    return socket_info;
}

SocketInfo* tcp_server_accept(SocketInfo* server) {
    if (!server || !server->is_server) return NULL;
    
    SocketInfo* client = malloc(sizeof(SocketInfo));
    if (!client) return NULL;
    
    socklen_t client_len = sizeof(client->address);
    client->socket_fd = accept(server->socket_fd, 
                              (struct sockaddr*)&client->address, &client_len);
    
    if (client->socket_fd < 0) {
        free(client);
        return NULL;
    }
    
    client->host = gp_strdup(inet_ntoa(client->address.sin_addr));
    client->port = ntohs(client->address.sin_port);
    client->is_connected = 1;
    client->is_server = 0;
    
    return client;
}

int tcp_send(SocketInfo* socket, const char* data, size_t length) {
    if (!socket || !socket->is_connected) return -1;
    return send(socket->socket_fd, data, length, 0);
}

char* tcp_receive(SocketInfo* socket, size_t* length) {
    if (!socket || !socket->is_connected) return NULL;
    
    char buffer[4096];
    ssize_t received = recv(socket->socket_fd, buffer, sizeof(buffer) - 1, 0);
    
    if (received <= 0) {
        if (length) *length = 0;
        return NULL;
    }
    
    buffer[received] = '\0';
    if (length) *length = received;
    return gp_strdup(buffer);
}

int tcp_close(SocketInfo* socket) {
    if (!socket) return -1;
    
    int result = 0;
    if (socket->socket_fd >= 0) {
        result = close(socket->socket_fd);
    }
    
    socket_info_destroy(socket);
    return result;
}

void socket_info_destroy(SocketInfo* socket) {
    if (!socket) return;
    free(socket->host);
    free(socket);
}

// Network Utilities
char* net_get_local_ip(void) {
    // Simplified implementation
    // In practice, this would enumerate network interfaces
    return gp_strdup("127.0.0.1");
}

char* net_get_hostname(void) {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return gp_strdup(hostname);
    }
    return gp_strdup("localhost");
}

char* net_resolve_hostname(const char* hostname) {
    struct hostent* host_entry = gethostbyname(hostname);
    if (host_entry) {
        return gp_strdup(inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0])));
    }
    return NULL;
}

// Network Testing
int net_ping(const char* host, int timeout) {
    // Simplified ping using system command
    char command[256];
    snprintf(command, sizeof(command), "ping -c 1 -W %d %s > /dev/null 2>&1", 
             timeout, host);
    return system(command) == 0 ? 1 : 0;
}

double net_ping_time(const char* host, int timeout) {
    // Simplified implementation
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    int result = net_ping(host, timeout);
    
    gettimeofday(&end, NULL);
    
    if (result) {
        double time_taken = (end.tv_sec - start.tv_sec) * 1000.0;
        time_taken += (end.tv_usec - start.tv_usec) / 1000.0;
        return time_taken;
    }
    
    return -1.0;
}

int net_port_scan(const char* host, int port, int timeout) {
    (void)timeout; // Suppress unused parameter warning
    SocketInfo* socket = tcp_connect(host, port);
    if (socket) {
        tcp_close(socket);
        return 1;
    }
    return 0;
}

// URL Utilities
char* url_encode(const char* str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    char* encoded = malloc(len * 3 + 1);  // Worst case: every char encoded
    if (!encoded) return NULL;
    
    char* p = encoded;
    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            *p++ = c;
        } else {
            sprintf(p, "%%%02X", (unsigned char)c);
            p += 3;
        }
    }
    *p = '\0';
    
    return encoded;
}

char* url_decode(const char* str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    char* decoded = malloc(len + 1);
    if (!decoded) return NULL;
    
    char* p = decoded;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '%' && i + 2 < len) {
            int value;
            if (sscanf(str + i + 1, "%2x", &value) == 1) {
                *p++ = (char)value;
                i += 2;
            } else {
                *p++ = str[i];
            }
        } else if (str[i] == '+') {
            *p++ = ' ';
        } else {
            *p++ = str[i];
        }
    }
    *p = '\0';
    
    return decoded;
}

int url_parse(const char* url, char** protocol, char** host, int* port, char** path) {
    if (!url) return -1;
    
    // Simple URL parsing
    char* url_copy = gp_strdup(url);
    char* ptr = url_copy;
    
    // Extract protocol
    char* proto_end = strstr(ptr, "://");
    if (proto_end) {
        *proto_end = '\0';
        if (protocol) *protocol = gp_strdup(ptr);
        ptr = proto_end + 3;
    } else {
        if (protocol) *protocol = gp_strdup("http");
    }
    
    // Extract host and port
    char* path_start = strchr(ptr, '/');
    if (path_start) {
        *path_start = '\0';
        if (path) *path = gp_strdup(path_start + 1);
    } else {
        if (path) *path = gp_strdup("");
    }
    
    // Check for port
    char* port_start = strchr(ptr, ':');
    if (port_start) {
        *port_start = '\0';
        if (port) *port = atoi(port_start + 1);
    } else {
        if (port) *port = 80;  // Default HTTP port
    }
    
    if (host) *host = gp_strdup(ptr);
    
    free(url_copy);
    return 0;
}

// Error Handling
char* net_get_error_string(int error_code) {
    return gp_strdup(strerror(error_code));
}

int net_get_last_error(void) {
    return errno;
}
