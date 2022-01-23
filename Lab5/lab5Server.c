// Name: Justin Li
// Date: 5/5/21
// Title: Lab5: Stop and Wait for an Unreliable Channel
// Description: This program is the Server

#include "lab5.h"

/*The simplest checksum algorithm is the so-called longitudinal parity check, 
which breaks the data into "bytes" with a fixed number 8 of bits, and then 
computes the exclusive or (XOR) of all those bytes. 
The result is appended to the message as an extra byte*/
// getChecksum()
int getChecksum(Packet packet) {
    packet.header.cksum = rand() % 2;
    int checksum = rand() % 2;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;
    while (ptr < end) {
        checksum ^= *ptr++;
    }
    return checksum;
}

// printPacket()
void printPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

// server sending ACK to the client
void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    Packet packet;	
    packet.header.seq_ack = seqnum;
    packet.header.len = sizeof(packet.data);
	packet.header.cksum = getChecksum(packet);
    // send packet
	sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
    printf("\t Server sending ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
}

// server receiving packet
Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum) {
    Packet packet;
    while(1) {
        // receive packets from the client
		recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        
        // print received packet
        printPacket(packet);
	    
        // validate the packet   
        if (packet.header.cksum != getChecksum(packet)) {
            printf("\t Server: Bad checksum, expected checksum was: %d\n", getChecksum(packet));
            serverSend(sockfd, address, *addrlen, !seqnum);
        } 
		else if (packet.header.seq_ack != seqnum) {
            printf("\t Server: Bad seqnum, expected sequence number was: %d\n", seqnum);
			serverSend(sockfd, address, *addrlen, !seqnum);
        } 
		else {
            printf("\t Server: Good packet\n");
			serverSend(sockfd, address, *addrlen, seqnum);
            break;
        }
    }
    return packet;
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <outfile>\n", argv[0]);
        exit(1);
    }

    // create and configure the socket
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	    perror("failure to setup an endpoint socket\n");	
	    exit(1);
    }

    // initialize the server address structure using argv[1]
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1]));
	servAddr.sin_addr.s_addr = INADDR_ANY;

	// bind the socket
	if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0) {
		perror("Failure to bind server address to the endpoint socket");
		exit(1);
	}

    // open file using argv[2]
	FILE *fp;
	fp = fopen(argv[2], "w");
	
    // get file contents from client and write it to the file
    int seqnum = 0;
    Packet packet;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    do {
		packet = serverReceive(sockfd, (struct sockaddr *)&clientaddr, &clientaddr_len, seqnum);
		fwrite(packet.data, 1, sizeof(packet.data), fp);
    	seqnum = (seqnum + 1) % 2;
    } while (packet.header.len != 0);

	fclose(fp);
    close(sockfd);
    return 0;
}
