include<stdio.h>

int findPivot (int arr[], int low, int high)
{
  int mid = (low + high)/2;
  if (high < low)
      return -1;
  if (high == low)
      return low;

  /*check if mid is pivot*/
  if ((mid < high) && (arr[mid] > arr[mid+1]))
      return mid;
  if ((mid > low)  && (arr[mid] < arr[mid-1]))
      return (mid - 1); 

  if (arr[low] < arr[mid])
      return findPivot(arr, mid+1, high);
  else
      return findPivot(arr, low, mid-1);
}

int main ()
{
 int arr[] = {5, 6, 7, 8, 9, 10, 1, 2, 3};
 int pivot;
 int size = sizeof(arr)/sizeof(arr[0]);
 pivot = findPivot(arr, 0, size-1);
 printf("Pivot elem:%d\n",pivot);

return 0;
}
