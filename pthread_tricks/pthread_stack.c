#include <pthread.h>

#define PAGE_SIZE 4096
#define STK_SIZE (2 * PAGE_SIZE)

void *stack;
pthread_t thread;
pthread_attr_t attr;

void *dowork(void *arg)
{
 // int data[2*STK_SIZE]; //will seg fault for this size as it exceeds thread stack
 int data[STK_SIZE];
 int i = 0;
 //for(i = 0; i < 2*STK_SIZE; i++) {
 for(i = 0; i < STK_SIZE; i++) {
   data[i] = i;
 }
}

int main(int argc, char **argv)
{

 // allocates memory of SIZE bytes and address of allocated memory will be multiple of PAGE_SIZE
  posix_memalign(&stack,PAGE_SIZE,STK_SIZE);
  pthread_attr_init(&attr);

// Set the stack size for thread
/*
    A thread's stack size is fixed at the time of thread creation.  
    Only the main thread can dynamically grow its stack.
*/
  pthread_attr_setstack(&attr,&stack,STK_SIZE);
  pthread_create(&thread,&attr,dowork,NULL);
  pthread_exit(0);
}
