#include<stdio.h>

int arr[] = {10, 22, 9, 33, 21, 50, 41, 60};



int main()
{
int size = sizeof(arr)/sizeof(arr[0]);
int max_lis = 1;

 lis(arr,size, &max_lis);

retunr 0;
}
