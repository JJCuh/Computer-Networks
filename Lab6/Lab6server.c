/* Justin Li
Date: 5/26/2021
Title: Lab 6 - Stop and Wait for an Unreliable Channel, with Loss
Description: This program runs the server program for an rdt3.0 sender */

#include "lab6.h"

//getChecksum()
int getChecksum(Packet packet) {
    packet.header.cksum = 0;
    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len; 
    while (ptr < end) {
        checksum ^= *ptr++;
    }
    return checksum;
}

//print packet
void printPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

//serverSend()
void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    // Simulating a chance that ACK gets lost
    if (rand() % PLOSTMSG == 0) {
        printf("Dropping ACK\n");
    }
    else {
        Packet packet;
        //prepare and send the ACK
		packet.header.seq_ack = seqnum;
		packet.header.len = sizeof(packet.data);
		packet.header.cksum = getChecksum(packet);
		sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        printf("Sent ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
    }
}

Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum) {                 
    Packet packet;
    while (1) {
        //Receive a packet from the client
		recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // validate the length of the packet
		if (packet.header.len != strlen(packet.data)) {
			printf("Packet lengths mismatch\n");
			break;
		}

        // print what was received
        printf("Received: ");
        printPacket(packet);

        //verify the checksum and the sequence number
        if (packet.header.cksum != getChecksum(packet)) {
            printf("Bad checksum, expected %d\n", getChecksum(packet));
			serverSend(sockfd, address, *addrlen, !seqnum);
        }
		else if (packet.header.seq_ack != seqnum) {
            printf("Bad seqnum, expected %d\n", seqnum);
			serverSend(sockfd, address, *addrlen, !seqnum);
        } 
		else {
            printf("Good packet\n");
			serverSend(sockfd, address, *addrlen, seqnum);
			break;
        }
    }
    printf("\n");
    return packet;
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <outfile>\n", argv[0]);
        exit(1);
    }

    // seed the RNG
    srand((unsigned)time(NULL));

    // create a socket
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Failure to setup an endpoint socket");
		exit(1);
	}

    // initialize the server address structure
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(argv[1]));
	address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket
	if ((bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr))) < 0) {
		perror("Failure to bind server address to the endpoint socket");
		exit(1);
	}

    // open file using argv[2]
	FILE *fp = fopen(argv[2], "wb");
	if (fp == NULL) {
		perror("fopen");
		return 1;
	}

    // get file contents from client and save it to the file
    int seqnum = 0;
    Packet packet;
	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len = sizeof(clientaddr);
    do {
		packet = serverReceive(sockfd, (struct sockaddr *)&clientaddr, &clientaddr_len, seqnum);
		fwrite(packet.data, 1, packet.header.len, fp);
		seqnum = (seqnum + 1) % 2;
    } while (packet.header.len != 0);

    //cleanup
	fclose(fp);
	close(sockfd);

    return 0;
}
