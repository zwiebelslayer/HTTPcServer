//
// Create  socket to serve the HTTP Server on!
//

#ifndef HTTPCSERVER_SERVER_H
#define HTTPCSERVER_SERVER_H

#include <winsock2.h>



#define PORT 8080


// Function prototypes
void initialize_winsock();

SOCKET create_socket();

void bind_socket(SOCKET server_socket);

void listen_for_connections(SOCKET server_socket);

SOCKET accept_connection(SOCKET server_socket);

void cleanup(SOCKET server_socket);

#endif //HTTPCSERVER_SERVER_H
