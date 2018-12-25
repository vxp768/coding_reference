#include<stdio.h>

int arr[]= {5,3,8,1,9,0,6,2,4,7};

void bubble_sort()
{
  int temp;
  for (int i=0; i<10; i++) {
     for (int j=0; j<10-1-i; j++) {
        if (arr[j] > arr[j+1]){
           temp = arr[j];
           arr[j] = arr[j+1];
           arr[j+1] = temp;
        }
     }
     printf("Sorted \n");
     for (int k=0; k<10; k++){
       printf("%d ",arr[k]); 
     }
     printf("\n");
  }
}

int main()
{
 printf("Unsorted:\n");
    for (int k=0; k<10; k++){
       printf("%d ",arr[k]); 
     }
 printf("\n");
  
 bubble_sort();
return 0;
}
