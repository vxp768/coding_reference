#include<stdio.h>

int arr[10]={3,2,0,6,4,8,7,1,9,5};

/*
*/

void selection_sort (int *a)
{
    int i,j,min,temp;
    for(i=0; i<10; i++) {
        min=i;
        /*find min and swap*/
        for(j=i; j<10; j++) {
            if(a[j] < a[min]) {
		min=j;
            }
        }
        temp = a[i];
        a[i] = a[min];
        a[min] = temp;


        printf("Sorted \n");
        for (int k=0; k<10; k++){
 	 printf("%d ",a[k]);
        }
 	printf("\n");
    }
}

int main()
{
    selection_sort(arr);
    for (int i=0; i<10; i++) {
         printf("%d ",arr[i]);
    }
    printf("\n");
return 0;
}
