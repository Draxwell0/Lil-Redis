#include <stdio.h>
#include <winsock2.h> 

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[]) {
  WSADATA wsa; // Estrutura para armazenar informações da Winsock
  SOCKET s; // Variável para armazenar o identificador do socket
  struct sockaddr_in server; // Estrutura para armazenar informações do servidor
  int port = 6379;

  // Inicialização do Winsock
  printf("\nInitializing Winsock... \n");
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    printf("Failed. Error code: %d", WSAGetLastError());
    return 1;
  }
  printf("Initialized\n");

 // Criação do socket
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    printf("Could not create socket: %d", WSAGetLastError());
  }
  printf("Socket created. \n");

  // Configuração da estrutura do endereço do servidor
  server.sin_family = AF_INET; // Família de endereços: IPv4
  server.sin_addr.s_addr = INADDR_ANY; // Aceitar conexões em qualquer interface
  server.sin_port = htons(port); // Porta em que o servidor irá escutar, convertida corretamente em bytes (endianness)

  // Vinculação do socket ao endereço e porta
  if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
    printf("Bind failed, %d", WSAGetLastError());
    return 1;
  }
  printf("Bind done \n");

  // Iniciando a escuta na porta especificada
  if (listen(s, 3) == SOCKET_ERROR) {
    printf("Listen failed %d", WSAGetLastError());
    return 1;
  }
  printf("Waiting for incoming connections... \n");

  while(1) {
    SOCKET client_socket;
    struct sockaddr_in client;
    int client_len = sizeof(client);

    // aceita uma conexão do cliente
    client_socket = accept(s, (struct sockaddr*)&client, &client_len); // accept é bloqueante por padrão -- o servidor espera indefinidamente até que uam conexão seja recebida. Alternativa responsiva é usar sockets não bloqueantes ou multi-threading para processar varias conexões simultaneamente
    if (client_socket == INVALID_SOCKET) {
      printf("Accept failed: %d", WSAGetLastError());
      continue; // tenta aceitar outra conexão
    }

    printf("Connection accepted from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    // interação com o cliente

    closesocket(client_socket);
  }

  closesocket(s);
  WSACleanup();

  return 0;
}