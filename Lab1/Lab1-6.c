// Name: Justin Li
// Date: 4/7/21
// Title: Lab1 Part6 - Unix/Linux Commands and Overview of C Programming
// Description: This program implements quantitative comparisons between circuit switching and packet switching

#include <stdio.h> 
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) { 
	int linkBandwidth;      // The bandwidth of a network link 
	int userBandwidth;      // The bandwidth required for a given user 
	int nCSusers;           // The number of circuit switching users 
	int nPSusers;           // The number of packet switching users 
	double tPSuser;	        // The percentage of time a packet switching user needs to transmit 
	double pPSusersBusy;    // The probability that a given (specific) packet switching user is busy transmitting
	double pPSusersNotBusy; // The probability that one (specific) packet switching user is not busy transmitting

	/* inputs */
	linkBandwidth = 200;
	userBandwidth = 20;
	tPSuser = 0.10;
	nPSusers = 19;

	/* Circuit switching scenario */
	nCSusers = linkBandwidth / userBandwidth;

	/* Packet switching scenario */
	pPSusersBusy = tPSuser;
	printf("%.2f \n", pPSusersBusy); //a

	pPSusersNotBusy = 1 - pPSusersBusy;
	printf("%.2f \n", pPSusersNotBusy); //b

	printf("%f \n", pow(pPSusersNotBusy, nPSusers - 1)); //c

	printf("%f \n", pPSusersBusy * pow(pPSusersNotBusy, nPSusers - 1)); //d

	printf("%f \n", nPSusers * pPSusersBusy * pow(pPSusersNotBusy, nPSusers - 1)); //e

	printf("%.15f \n", pow(pPSusersBusy, 10) * pow(pPSusersNotBusy, nPSusers - 10)); //f

	// tgamma(n) = (n-1)!
	printf("%.10f \n", tgamma(nPSusers+1)/(tgamma(11)*tgamma(nPSusers - 9)) * pow(pPSusersBusy, 10) * pow(pPSusersNotBusy, nPSusers - 10)); //g
	
	double h;
	int i;
	for (i = 11; i <= nPSusers; i++) {
		h += tgamma(nPSusers + 1) / (tgamma(i+1) * tgamma(nPSusers - i+1)) * pow(pPSusersBusy, i) * pow(pPSusersNotBusy, nPSusers - i);
	}
	printf("%.10f \n", h);  //h

	return 0;
}
