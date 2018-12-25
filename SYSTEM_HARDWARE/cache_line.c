#include<stdio.h>

//#define arraySize (1024*1024*1024)
#define arraySize (1024)

int arr[arraySize];

int main(void)
{
    int i=0;

/*    for (i=0; i<arraySize; i++) {
       arr[i] = i;
    }*/
    for (i=0; i<arraySize; i += 256) {
       arr[i] = i;
    }
return 0;
}
