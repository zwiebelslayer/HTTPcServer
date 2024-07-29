//
// Simple HTTP Server
//

#ifndef HTTPCSERVER_HTTP_SERVER_H
#define HTTPCSERVER_HTTP_SERVER_H
#include <winsock2.h>

#define MAX_HTTP_RECEIVE_SIZE 8192   // every request which has more than this number of bytes is discarded! completely arbitrary number
#define  MAX_REQUEST_ROUTE_BUFFER_SIZE 254

void handle_client(SOCKET client_socket);

struct http_request parse_incoming_request(char* buffer);
#endif //HTTPCSERVER_HTTP_SERVER_H
