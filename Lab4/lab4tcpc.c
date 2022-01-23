// Name: Justin Li
// Date: 4/28/21
// Title: Lab4: TCP/IP Socket Programming
// Description: This program is the Client

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h> 


int main (int argc, char *argv[]) {
    int		sockfd = 0, n = 0;
    char	buf[1024];
    struct	sockaddr_in servAddr; 

    if (argc != 4){
	    printf ("Usage: %s <port no>  <ip of server> <src_file> \n", argv[0]);
	    exit(1);
    } 
    //Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Failure to setup an endpoint socket");
        exit(1);
    } 

    //Set server address by filling sevAddr fields
	servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(5000);
    struct hostent *host = (struct hostent *)gethostbyname("localhost");
    servAddr.sin_addr = *((struct in_addr *)host->h_addr);

    //Connect to server
    if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr)) < 0){
		perror("Failure to connect to the server");
        exit(1);
    } 
    
	//Create and Open file
	FILE *fp;
	fp = fopen("lab4.txt", "r");
	if (fp == NULL) {
		perror("Failure to open the file");
		exit(0);
	}

    int count=1;
    //read from file and write to server
	while ((count = fread(buf, 1, sizeof(buf), fp)) > 0) {
		send(sockfd, buf, count, 0);
	}
    
	fclose(fp);
    close(sockfd);
    return 0;
}
