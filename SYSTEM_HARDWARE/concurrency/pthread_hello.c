#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

#define NUM_THREADS 5

void *printhello(void *threadid)
{
    long tid = (long)threadid;
    printf("Hello world\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    for(t=0; t<NUM_THREADS; t++) {
        printf("In main thread\n");
	rc = pthread_create(&threads[t],NULL,printhello,(void*)t);
	if (rc) {
	    printf("Error\n");
 	    exit(-1);
	}
    }
//    pthread_exit(NULL);
}
