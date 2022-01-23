// Name: Justin Li
// Date: 5/5/21
// Title: Lab5: Stop and Wait for an Unreliable Channel
// Description: This program is the Client

#include "lab5.h"

/*The simplest checksum algorithm is the so-called longitudinal parity check, 
which breaks the data into "bytes" with a fixed number 8 of bits, and then 
computes the exclusive or (XOR) of all those bytes. 
The result is appended to the message as an extra byte*/
// getChecksum()
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

// printPacket()
void printPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

// clientSend()
void clientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet) {
	int r;
	while(1) {
		r = rand() % 5;
		if (r == 0 && packet.header.len != 0) {
			packet.header.cksum = -1;
		}
		else {
			packet.header.cksum = getChecksum(packet);
		}

        // send the packet
        printf("Client sending packet\n");
		sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // receive an Ack from the server
        Packet recvpacket;
		recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, address, &addrlen);

        // print what was received
        printf("Client received ACK %d, checksum %d - \n", recvpacket.header.seq_ack, recvpacket.header.cksum);
        
		// validate the ACK
        if (recvpacket.header.cksum != getChecksum(recvpacket)) {
            // bad checksum, resend packet
            printf("Client: Bad checksum, expected checksum was: %d\n", getChecksum(recvpacket));
        } 
		else if (recvpacket.header.seq_ack != packet.header.seq_ack) {
            // incorrect sequence number, resend packet
            printf("Client: Bad seqnum, expected sequence number was: %d\n", packet.header.seq_ack);
        } 
		else {
            // good ACK, we're done
            printf("Client: Good ACK\n");
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip> <port> <infile>\n", argv[0]);
        return 1;
    }

	// adding randomness
	srand(time(0));

    // create and configure the socket [name the socket sockfd] 
    int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	    perror("Failed to setup endpoint socket.\n");
	    exit(1);	
    }

    // initialize server address structure using argv[1] and argv[2]
	struct sockaddr_in servAddr;
	socklen_t addr_len = sizeof(servAddr);
	struct hostent *host;
	host = (struct hostent *) gethostbyname(argv[1]);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[2]));
	servAddr.sin_addr = *((struct in_addr *)host->h_addr);
    

    // open file using argv[3]
	FILE *fp;
	fp = fopen(argv[2], "r");

    // send file contents to server
    int seq=0;
    Packet pack;
	while(fread(pack.data, 1, sizeof(pack.data), fp) > 0); // Reads from file then sends to server
		pack.header.seq_ack = seq;
    	pack.header.len = sizeof(pack.data);
		clientSend(sockfd, (struct sockaddr *)&servAddr, addr_len, pack);
    	seq = (seq + 1) % 2;
    }

    // send zero-length packet to server to end connection
    Packet final;
    final.header.seq_ack = seq;
    final.header.len = 0;
    final.header.cksum = getChecksum(final);
    clientSend(sockfd, (struct sockaddr *)&servAddr, addr_len, final);
    
	// clean up
	//-close the sockets and file
	fclose(fp);
    close(sockfd);
    return 0;
}
