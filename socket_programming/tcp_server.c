#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

int main(int argc, char **argv)
{
    int server_fd, client_fd;
    int portno, n;
    struct sockaddr_in server_addr, client_addr;
    char buf[BUFSIZE];
    unsigned int clientlen;

    struct hostent *hostp;

    if (argc != 2) {
       printf("usage: %s <port>\n",argv[0]);
       exit(1);
    }
    portno = atoi(argv[1]);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero((char*)&server_addr, sizeof(server_addr));
    /* this is an Internet address */
  server_addr.sin_family = AF_INET;

  /* let the system figure out our IP address */
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* this is the port we will listen on */
  server_addr.sin_port = htons((unsigned short)portno);

  /* bind: associate the parent socket with a port */
  bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

  /* listen: make this socket ready to accept connection requests */
  listen(server_fd, 5); /* allows 5 requests */
  clientlen = sizeof(client_addr);
  while(1) {
      /* accept: wait for a connection request */
      client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &clientlen);

      /* gethostbyaddr: determine who sent the message */
      hostp = gethostbyaddr((const char*)&client_addr.sin_addr.s_addr, 
		      sizeof(client_addr.sin_addr.s_addr), AF_INET);

      bzero(buf, BUFSIZE);
      n = read(client_fd, buf, BUFSIZE);

      /* 
       * write: echo the input string back to the client 
       */
      n = write(client_fd, buf, strlen(buf));
      close(client_fd);
  }
}
