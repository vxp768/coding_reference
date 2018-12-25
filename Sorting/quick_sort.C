#include<stdio.h>

int arr[10]={3,2,0,6,4,8,7,1,9,5};

int divide (int arr[], int low, int high)
{
    int left = low;
    int right = high;
    int pivot = arr[low];
    while (left < right) {
        while (arr[left] <= pivot)
               left++;
        while (arr[right] > pivot)
               right--;
        if (left < right) {
            int temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
        }
    }
    arr[low] = arr[right];
    arr[right] = pivot;
    return right;
}
void quick_sort(int arr[], int low, int high)
{
   int pivot;
printf("Sorted: left:%d high:%d\n",low, high);
 for(int i=0; i<10;i++)
   printf("%d ",arr[i]);
 printf("\n");

   if (high > low) {
       pivot = divide(arr, low, high);
       quick_sort(arr, low, pivot-1);
       quick_sort(arr, pivot+1, high);
   }
 }
int main()
{
 for(int i=0; i<10;i++)
   printf("%d ",arr[i]);
 printf("\n");
 quick_sort(arr,0,9);
 for(int i=0; i<10;i++)
   printf("%d ",arr[i]);
 printf("\n");
}
