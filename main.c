
#include <stdio.h>
#include "server.h"
#include "http_server.h"

int main() {
    initialize_winsock();

    SOCKET server_socket = create_socket();
    bind_socket(server_socket);
    listen_for_connections(server_socket);

    printf("Server is running on port %d\n", PORT);

    while (1) {
        SOCKET client_socket = accept_connection(server_socket);
        if (client_socket != INVALID_SOCKET) {
            handle_client(client_socket);
            closesocket(client_socket);
        }
    }

    cleanup(server_socket);
    return 0;
}


