#include <stdio.h>
#include <winsock2.h> 

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[]) {
  WSADATA wsa;
  SOCKET s;
  struct sockaddr_in server;
  int port = 6379;

  printf("\nInitializing Winsock... \n");
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    printf("Failed. Error code: %d", WSAGetLastError());
    return 1;
  }
  printf("Initialized\n");

  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    printf("Could not create socket: %d", WSAGetLastError());
  }
  printf("Socket created. \n");

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);

  if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
    printf("Bind failed, %d", WSAGetLastError());
    return 1;
  }
  printf("Bind done \n");

  if (listen(s, 3) == SOCKET_ERROR) {
    printf("Listen failed %d", WSAGetLastError());
    return 1;
  }
  printf("Waiting for incoming connections... \n");

  while(1) {
    SOCKET client_socket;
    struct sockaddr_in client;
    int client_len = sizeof(client);

    client_socket = accept(s, (struct sockaddr*)&client, &client_len); // accept is blocking by default -- the server waits indefinitely until a connection is received. A responsive alternative is to use non-blocking sockets or multi-threading to process several connections simultaneously.
    if (client_socket == INVALID_SOCKET) {
      printf("Accept failed: %d", WSAGetLastError());
      continue;
    }

    printf("Connection accepted from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    // client interaction

    closesocket(client_socket);
  }

  closesocket(s);
  WSACleanup();

  return 0;
}