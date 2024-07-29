//
// Simple HTTP Server
//

#include "http_server.h"
#include <stdint.h>
#include <winsock2.h>

/* Which http request method */
enum http_request_method{
    GET,
    POST,
    PUT,
    PATCH,
};


/* HTTP1.1 or HTTP2.0 */
enum http_request_protocol_version{
    http_1_1,
    http_2_0
};

struct http_request{

    enum http_request_protocol_version protocol_version;
    enum http_request_method request_method;
    char request_route[MAX_REQUEST_ROUTE_BUFFER_SIZE]; /* e.g. example.com/test/ request route would be /test/ then! */

};

/*
 * Parse the incoming request for version and method
 */
struct http_request parse_incoming_request(char* buffer){

    struct http_request request;
    char* split_request;
    split_request = strtok(buffer,"\r\n");

    /* Extract the first line of the http request to determine the route, request method (get/post) and the protocol version */
    char* temp_buffer = malloc(strlen(split_request));
    memcpy(temp_buffer, split_request, strlen(split_request));
    char* http_internal;
    http_internal = strtok(temp_buffer," ");
    /*
     *
     * second loop is the route
     * third loop is the protocol version
     */
    for(uint8_t i = 0; i<3; i++){
        if( 0 == i){
            // first round is the request method
            if(strncmp(http_internal, "GET", 3) == 0){
                request.request_method = GET;
            }
        }
        if( 1 == i){
            // second round is the request method
            strncpy(request.request_route, http_internal, MAX_REQUEST_ROUTE_BUFFER_SIZE);
        }
        if( 2 == i){
            // third round is the http version
            if(strncmp(http_internal, "HTTP/1.1", 7) == 0){
                request.protocol_version = http_1_1;
            }
            if(strncmp(http_internal, "HTTP/2.0", 7) == 0){
                request.protocol_version = http_2_0;
            }
        }
        http_internal = strtok(NULL," ");
    }
    split_request = strtok(split_request,"\r\n");
    split_request = strtok(NULL,"\r\n"); // the first arg is parsed now left are the headers, which are discarded
    /*
     * TODO: Parse headers here!
     */
    return request;
}


/*
 * handles an incoming request.
 * "entrypoint" to the http server
 *
 */
void handle_client(SOCKET client_socket) {
    char buffer[MAX_HTTP_RECEIVE_SIZE];   // buffer to hold the incoming request
    int bytes_received = recv(client_socket, buffer, MAX_HTTP_RECEIVE_SIZE, 0);
    if (bytes_received == SOCKET_ERROR) {
        printf("Recv failed: %ld\n", WSAGetLastError());
        return;
    }

    if(bytes_received >= MAX_HTTP_RECEIVE_SIZE){
        const char* response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: plain/text\r\n"
                               "Connection: close\r\n\r\n"
                               "Request is too large";

        send(client_socket, response, strlen(response), 0);
    }

    buffer[bytes_received] = '\0';
    printf("Received request:\n%s. With %d number of bytes\n", buffer, bytes_received);

    struct http_request incoming_request = parse_incoming_request(buffer);



    const char *response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "Connection: close\r\n\r\n"
                           "<html><body><h1>Hello, World!</h1></body></html>";

    send(client_socket, response, strlen(response), 0);

}


