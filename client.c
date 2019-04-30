#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }

int main (int argc, char *argv[]) {
  if (argc < 2) on_error("Usage: %s [port]\n", argv[0]);

  int port = atoi(argv[1]);

  int client_fd, err;
  struct sockaddr_in client;
  char buf[BUFFER_SIZE];

  client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd < 0) on_error("Could not create socket\n");

  client.sin_family = AF_INET;
  client.sin_port = htons(port);
  client.sin_addr.s_addr = htonl(INADDR_ANY);
  
  err = connect(client_fd, (struct sockaddr *) &client, sizeof(client));
  if (err < 0) on_error("Could not connect to socket\n");

  printf("Client connected to %d\n", port);
  
  err = send(client_fd, "hello", 6, 0);
  
  int read = recv(client_fd, buf, BUFFER_SIZE, 0);
  if (read < 0) on_error("Client read failed\n");
  
  printf("received: %s\n", buf);
  
  err = shutdown(client_fd, SHUT_RDWR);
  if (err < 0) on_error("Could not shutdown connection to socket\n");
  
  return 0;
}
