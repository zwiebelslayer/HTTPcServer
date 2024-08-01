
#include <stdio.h>
#include "server.h"
#include "http_server.h"
#include "ht.h"

int main() {
    initialize_winsock();

    SOCKET server_socket = create_socket();
    bind_socket(server_socket);
    listen_for_connections(server_socket);


    printf("Server is running on port %d\n", PORT);
    /*
     * Define routes and their handler with the according params!
     */
    http_route_dispatch route_dispatch[1];
    route_dispatch[0].route = "/";
    route_dispatch[0].callback_function = serve_html_file;
    route_dispatch[0].callback_function_params = "html_src_dir/test.html";


    struct ht* route_dispatch_hashtable = ht_create();
    char* res = ht_set(route_dispatch_hashtable, (route_dispatch[0].route), &(route_dispatch[0]));


    while (1) {
        SOCKET client_socket = accept_connection(server_socket);
        if (client_socket != INVALID_SOCKET) {
            handle_client(client_socket, route_dispatch_hashtable);
            closesocket(client_socket);
        }
    }

    cleanup(server_socket);
    return 0;
}


