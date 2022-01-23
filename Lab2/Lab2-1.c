// Name: Justin Li
// Date: 4/14/21
// Title: Lab2 Step1: Multithreading in C and File transfer
// Description: This program copies files (binary and text) using functions

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char *fName = argv[1];
	char *fName2 = argv[2];
	FILE *fp_src, *fp_dst;
	char buf[1000];
	fp_src = fopen(fName, "r");  // open "src1.dat" for reading
	fp_dst = fopen(fName2, "w"); // open "dst1.dat" for reading
	while (fread(buf, 1, sizeof(buf), fp_src)) { // reads from fp_src
		fwrite(buf, 1, sizeof(buf), fp_dst); // writes to fp_dst
	}
	fclose(fp_src);
	fclose(fp_dst); // closes the file pointers

	return 0;
}

