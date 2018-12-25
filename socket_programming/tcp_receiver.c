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
    int client_fd, portno, n;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];

    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(1);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        printf("ERROR, no such host as %s\n", hostname);
        exit(0);
    }
   
    /* build the server's Internet address */
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(portno);

    /* connect: create a connection with the server */
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
      printf("ERROR connecting");

    /* get message line from the user */
    printf("Please enter msg: ");
    bzero(buf, BUFSIZE);
    fgets(buf, BUFSIZE, stdin);

    /* send the message line to the server */
    n = write(client_fd, buf, strlen(buf));
    if (n < 0) 
      printf("ERROR writing to socket");

    /* print the server's reply */
    bzero(buf, BUFSIZE);
    n = read(client_fd, buf, BUFSIZE);
    if (n < 0) 
      printf("ERROR reading from socket");
    printf("Echo from server: %s", buf);
    close(client_fd);
    return 0;
}
