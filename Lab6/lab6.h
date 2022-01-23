#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define PLOSTMSG 2
typedef struct {
    int seq_ack;
    int len;
    int cksum;
} Header;

typedef struct {
    Header header;
    char data[10];
} Packet;