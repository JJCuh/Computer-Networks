// Name: Justin Li
// Date: 4/14/21
// Title: Lab2 Step4: Multithreading in C and File transfer
// Description: This program to copies multiple files by creating threads, each of which will be responsible of copying one file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int NTHREADS = 10;
char iName[20], oName[20];

void *go(void *arg) {
	int n = (int)(size_t)arg; 	
	char buf[100];
	FILE *fp, *fp2;
	fp = fopen(iName, "r");
	sprintf(oName, "%s-%d", iName, n);
	fp2 = fopen(oName, "w");
	while (fread(buf, 1, sizeof(buf), fp)) {
		fwrite(buf, 1, sizeof(buf), fp2);
	}
	fclose(fp);
	fclose(fp2);
}

int main (int argc, char *argv[]) {
	strcpy(iName, argv[1]);
	pthread_t threads[NTHREADS];
	int i;
	for (i = 0; i < NTHREADS; i++){
       	pthread_create(&threads[i], NULL, go, (void *)(size_t)i);
	}
    for (i = 0; i < NTHREADS; i++){
        pthread_join(threads[i], NULL);
	}
    	
	return 0;
}
