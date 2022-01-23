// Name: Justin Li
// Date: 4/28/21
// Title: Lab4: TCP/IP Socket Programming
// Description: This program is the Server 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>

int main (int argc, char *argv[]){
    if (argc != 3){
		  printf ("Usage: %s <port number> <src_file> \n",argv[0]);
		  exit(1);
	  }
    int		sockfd, connfd, rb, sin_size;     // Declare socket file descriptor and connection
    char	buf[1024];			// Declare a buffer of size 1k bytes
	FILE	*fp;
    
    // Declare server address to which to bind for receiving messages and client address to fill in sending address
    struct	sockaddr_in servAddr, clienAddr; 
    
    // Set server address/port
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(5000); 
    
    // Open a TCP socket, if successful, returns a descriptor
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
      perror("Failure to setup an endpoint socket");
      exit(1);
    }
    
    // Set address/port of server endpoint for socket descriptor
    if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0){
      perror("Failure to bind server address to the endpoint socket");
      exit(1);
    } 

    // Server listening to the socket endpoint
	printf("Server waiting for client at port 5000 \n");
	if (listen(sockfd, 1) == -1) { // return -1 if connection is unsuccessful
		perror("Failure to listen");
		exit(1);
	}

	// Open file for writing
	printf("opening file\n");
	fp = fopen("lab4-copy.txt", "w");
	sin_size = sizeof(struct sockaddr_in); //size of sockaddr_in
    
    // Server accepting to establish a connection with a connecting client, if successful, returns a connection descriptor
    if ((connfd = accept(sockfd, (struct sockaddr *)&clienAddr, (socklen_t *)&sin_size)) < 0){
        perror("Failure to accept connection to the client");
        exit(1);
    }
    
	// Connection established, server begins to read and write to the connecting client
    printf("Connection Established with client: IP %s and Port %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));
	while ((rb = recv(connfd, buf, sizeof(buf), 0)) > 0) {
		fwrite(buf, 1, rb, fp);
	}
	
	close(connfd);
    fclose(fp);
    close(sockfd);
    return 0;
}
