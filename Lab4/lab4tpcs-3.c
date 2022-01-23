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
#include <pthread.h>

#define NTHREADS 5 // 5 threads
#define CONN 5 // 5 connections

void *threadF(void *arg); // thread function
pthread_t threads[NTHREADS]; // thread ID array
char fName[20]; // a file name of 20 bytes

struct d { 		// structure to hold thread data
   int con;		// connection
   int fP;		// file pointer
   int tID;		// thread id
};

int main (int argc, char *argv[]){
    if (argc != 3){
		  printf ("Usage: %s <port number> <src_file> \n", argv[0]);
		  exit(1);
	  }
    int		sockfd;             // Declare socket file descriptor
    int     k, connfd[CONN];    // connection array
	FILE *fp;
    
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
    if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0) {
      perror("Failure to bind server address to the endpoint socket");
      exit(1);
    } 

    // Server listening to the socket endpoint, and can queue 5 client requests
	printf("Server waiting for client at port 5000 \n");
	if (listen(sockfd, CONN) == -1) { // return -1 if connection is unsuccessful
		perror("Failure to listen");
		exit(1);
	}

	k = 0; //connection index
	strcpy(fName, argv[2]);
    int sin_size = sizeof(struct sockaddr_in); //size of sockaddr_in
    
	// Server loops all the time accepting conncections when they are made, then passing connfd to a thread
	while(1) {
        // Server accepting to establish a connection with a connecting client, if successful, returns a connection descriptor
		if ((connfd[k] = accept(sockfd, (struct sockaddr *)&clienAddr, (socklen_t *)&sin_size)) < 0) {
          perror("Failure to accept connection to the client");
          exit(1);
        }
        //Connection established, server begins to read and write to the connecting client
        printf("Connection Established with client: IP %s and Port %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));
        pthread_attr_t attr; 
      	pthread_attr_init(&attr);
        struct d *data = (struct d *) malloc(sizeof(struct d)); // Data object to pass to thread
	      data->con = connfd[k];
		  data->fP = fp;
		  data->tID = k;   
	      pthread_create(&threads[k], NULL, threadF, (void *)data); // Thread to handle connfd
		  k++;
		close(connfd[k]);
    }
    close(sockfd);
    return 0;
}

// Thread function handling client connection
void *threadF(void *arg){
	char buf[1024], copy[20];
	struct d *data = (struct d *) arg;
	int rb;
	sprintf(copy, "%s-%d", fName, data->tID);
	printf("Opening file %s\n", copy); // Open file for writing
	data->fP = fopen(copy, "w");
	if (data->fP == NULL) {
		perror("Failure to open the file");
		exit(0);
	}
	while ((rb = recv(data->con, buf, sizeof(buf), 0)) > 0) {
		fwrite(buf, 1, rb, data->fP);
	}
	fclose(data->fP);
	pthread_exit(0);
}
