//
// Create a socket to serve the http server on
//
#include <winsock2.h>
#include "server.h"



void initialize_winsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        exit(1);
    }
}

SOCKET create_socket() {
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed: %ld\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    return server_socket;
}

void bind_socket(SOCKET server_socket) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    int result = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (result == SOCKET_ERROR) {
        printf("Bind failed: %ld\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
}

void listen_for_connections(SOCKET server_socket) {
    int result = listen(server_socket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        printf("Listen failed: %ld\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
}

SOCKET accept_connection(SOCKET server_socket) {
    /* accept() blocks until it has something to "accept" */
    SOCKET client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed: %ld\n", WSAGetLastError());
    }
    return client_socket;
}



void cleanup(SOCKET server_socket) {
    closesocket(server_socket);
    WSACleanup();
}