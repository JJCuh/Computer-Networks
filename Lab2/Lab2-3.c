// Name: Justin Li
// Date: 4/14/21
// Title: Lab2 Step3: Multithreading in C and File transfer
// Description: This program calculates the time it takes to copy files (binary and text) using functions

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
	clock_t start, end;
	double cpu_time_used;
	
	start = clock();
	char *fName = argv[1];
	char *fName2 = argv[2];
	FILE *fp_src, *fp_dst;
	char buf[100];
	fp_src = fopen(fName, "r");  // open "src1.dat" for reading
	fp_dst = fopen(fName2, "w"); // open "dst1.dat" for reading
	while (fread(buf, 1, sizeof(buf), fp_src)) { // reads from fp_src
		fwrite(buf, 1, sizeof(buf), fp_dst); // writes to fp_dst
	}
	fclose(fp_src);
	fclose(fp_dst); // closes the file pointers
	end = clock();

	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("%f \n", cpu_time_used);

	return 0;
}

