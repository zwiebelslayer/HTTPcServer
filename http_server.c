//
// Simple HTTP Server
//

#include "http_server.h"
#include <stdint.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

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
 * Build the response, where the content is a html "formatted" char (this is important for the response headers)
 * @param: response_content: html file content
 * @param: response: return value
 */
void build_response_html(char* response ,char* response_content){
    // TODO: http status code should be changeable!
    char base_res[] = "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n";


    char* content_size_header = (char*) calloc(22 + 10, sizeof(char)); // 22 = strlen(Content-Length:); 10: number and some extra save space

    sprintf(content_size_header, "Content-Length:  %llu\r\n", strlen(response_content));
    // build response headers
    strcat(response, base_res);

    strcat(response, content_size_header);
    strcat(response, "Connection: close\r\n\r\n");
    // append the html content
    strcat(response, response_content);



    free(content_size_header);

}


/*
 * handles an incoming request.
 * "entrypoint" to the http server
 *
 */
void handle_client(SOCKET client_socket, struct ht* ht_route_dispatch) {
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


    char response[99999] = {0};

    http_route_dispatch* route_dispatch_instructions = (http_route_dispatch*) ht_get(ht_route_dispatch,
                                                                                     (const char *) &(incoming_request.request_route));
    //TODO: fix this code to send the response!
    if(route_dispatch_instructions == NULL){
        // ROUTE NOT FOUND -> serve 404
        build_response_html(response, "<html>404 not Found!</html>");
        send(client_socket, response, strlen(response), 0);
    }
    else{
        route_dispatch_instructions->callback_function(client_socket, route_dispatch_instructions->callback_function_params);
        //
    }



    //

}


/*
 * Reads a html file and serves its contents
 */
int serve_html_file(SOCKET client_socket, const char* html_file_path){
    const char* filepath = (const char*)html_file_path;
    FILE* file_handle = fopen(html_file_path, "r");
    if(file_handle == NULL)
    {
        printf("Could not open HTML file check file path!");
        return -1;
    }
    fseek(file_handle, 0, SEEK_END);
    long file_size = ftell(file_handle);
    fseek(file_handle, 0, SEEK_SET);
    char* html_file_contents = calloc((file_size + 1), sizeof(char));
    fread(html_file_contents, file_size, 1, file_handle);
    /* alloc buffer for the file ocntents and the "preamble" http protocol stuff  */
    char* response = calloc(file_size+1024, sizeof(char));
    build_response_html(response, html_file_contents);


    send(client_socket, response, strlen(response), 0);

    free(html_file_contents);
    free(response);

    return 0;
}