#include<stdio.h>

int arr[10]={3,2,0,6,4,8,7,1,9,5};

/*
*/

void insertion_sort (int *a)
{
    int i,j,curr;
    for(i=1; i<10; i++) {
       curr = a[i];
       j=i-1; 
       while (j>=0 && a[j] > curr) {
           a[j+1] = a[j]; 
           j--;
       }
       a[j+1] = curr;
       printf("Sorted \n");
       for (int k=0; k<10; k++){
	printf("%d ",a[k]);
       }
	printf("\n");
    }
}

int main()
{
    insertion_sort(arr);
    for (int i=0; i<10; i++) {
         printf("%d ",arr[i]);
    }
    printf("\n");
return 0;
}
