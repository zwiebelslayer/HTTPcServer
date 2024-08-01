//
// Simple HTTP Server
//

#ifndef HTTPCSERVER_HTTP_SERVER_H
#define HTTPCSERVER_HTTP_SERVER_H
#include <winsock2.h>
#include "ht.h"

#define MAX_HTTP_RECEIVE_SIZE 8192   // every request which has more than this number of bytes is discarded! completely arbitrary number
#define  MAX_REQUEST_ROUTE_BUFFER_SIZE 254

typedef int (*callback_function_type)(void*);

typedef struct {
    const char* route;
    callback_function_type callback_function;
    void* callback_function_params; // e.g. the html filepath to be served

} http_route_dispatch;

void handle_client(SOCKET client_socket,struct ht* ht_route_dispatch);

int serve_html_file(const char* html_file_path);


struct http_request parse_incoming_request(char* buffer);






#endif //HTTPCSERVER_HTTP_SERVER_H
