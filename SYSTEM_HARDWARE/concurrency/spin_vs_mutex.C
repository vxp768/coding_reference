#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<errno.h>
#include<sys/time.h>
#include<list>

#define LOOPS 10000000

using namespace std;

list<int> the_list;

#ifdef USE_SPINLOCK
pthread_spinlock_t spinlock;
#else
pthread_mutex_t mutex;
#endif

pid_t gettid() 
{
 return syscall(__NR_gettid);
}


