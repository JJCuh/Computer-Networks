// Name: Justin Li
// Date: 4/121/21
// Title: Lab3 Step3: Network commands and HTTP
// Description: This program computes network calculations

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int rtt = 3 + 20 + 26;
	int rtthttp = 47;
	int n, p, o;

	// part a
	int a = rtt + 2 * rtthttp;
	printf("%d \n", a); //143
	
	// part b
	o = 6;
	int b = a + 2 * o * rtthttp;
	printf("%d \n", b); //707

	// part c
	n = 5;
	if (o % n > 0) {
		p = o / n + 1;
	}
	else {
		p = o / n;
	}
	int c = a + 2 * p * rtthttp;
	printf("%d \n", c); //331

	int d = a + p * rtthttp;
	printf("%d \n", d); //237

	return 0;
}

