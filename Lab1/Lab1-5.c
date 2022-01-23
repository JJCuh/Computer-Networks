// Name: Justin Li
// Date: 4/6/21
// Title: Lab1 Part5 - Unix/Linux Commands and Overview of C Programming
// Description: This program creates 2 sets of threads, and prints them out in order.

#include <stdio.h>      /* printf, stderr */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* fork */
#include <stdlib.h>     /* atoi */
#include <errno.h>      /* errno */
#include <pthread.h>
 
#define NUM_OF_THREADS	100

void *function1(void *tid){
	/* This function prints thread1's id & exits */
	printf("\t Hello from Thread 1, thread id: %d\n", tid);
}

void *function2(void *tid){
	/* This function prints thread2's id & exits */
	printf("Hello from Thread 2, thread id: %d\n", tid);
}	

/* main function with command-line arguments to pass */
int main(int argc, char *argv[]) { 
    int status, i, n = atoi(argv[1]); //n microseconds to input from keyboard for delay
    pthread_t thread1;
    pthread_t thread2;

	for (i=0; i<NUM_OF_THREADS; i++) {
		status = pthread_create(&thread1, NULL, function1, (void *)i);
       		pthread_join(thread1, NULL);
		usleep(n);

		if (status != 0){
			printf("pthread_create returned error code %d\n", status);
			exit(-1);
		}

		status = pthread_create(&thread2, NULL, function2, (void *)i);
		pthread_join(thread2, NULL);
		usleep(n);

		if (status != 0){
			printf("pthread_create returned error code %d\n", status);
			exit(-1);
		}
	}
	return 0;
}
