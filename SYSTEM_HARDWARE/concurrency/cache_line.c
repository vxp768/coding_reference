#include <time.h>
#include<stdio.h>
#include<stdlib.h>


int main(int argc, char *argv[])
{
	const int length = 512*1024*1024;
	int *arr = (int*)malloc(length*sizeof(int));
	int i;

 	for(i=0; i<length; i++) 
		arr[i] = i;

	clock_t t;

        t = clock();
	for(i=0; i< length; i = i+30)
		arr[i] = arr[i] * 3;
	t = clock() - t;
	printf("loop 2 used:%f seconds\n",(float)t/CLOCKS_PER_SEC);

        t = clock();
	for(i=0; i< length; i++)
		arr[i] = arr[i] * 3;
	t = clock() - t;
	printf("loop 1 used:%f seconds\n",(float)t/CLOCKS_PER_SEC);

}
