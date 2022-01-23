// Name: Justin Li
// Date: 4/14/21
// Title: Lab2 Step2: Multithreading in C and File transfer
// Description: This program copies files (binary and text) using system calls

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	char *fName = argv[1];
	char *fName2 = argv[2];
	char buf[100];
	int fd_src = open(fName, O_RDONLY); // opens fName for reading
	int fd_dst = open(fName2, O_WRONLY | O_CREAT | O_TRUNC); // opens fName2 for writing
	while (read(fd_src, buf, sizeof(buf))) { // reads from fp_src
		write(fd_dst, buf, sizeof(buf)); // writes to fp_dst
	}
	close(fd_src);
	close(fd_dst); // closes the files

	return 0;
}