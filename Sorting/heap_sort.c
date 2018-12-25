#include<stdio.h>

void swap(int *a, int *b) 
{
  int temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

void printarr(int *arr, int size)
{
  for (int i=0; i<size; i++){
	printf("%d ",arr[i]);
  }
  printf("\n");
}
/* Heapify a subtree rooted with node i which is an index in arr[]*/
void heapify(int *arr, int size, int i)
{
    int largest = i;
    int left = (2*i)+1; //left child of node at index i
    int right = (2*i)+2; //right child of node at index i

    if (left < size && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < size && arr[right] > arr[largest]) {
	largest = right;
    }
    if (largest != i) {
        swap(&arr[largest], &arr[i]);
        //Heapify the affected subtree
        heapify(arr, size, largest);
    }
}

void heap_sort(int *arr, int size)
{
  /* Build heap and rearrange array(heapify) */ 
  for (int i= size/2 -1; i>=0; i--) {
      heapify(arr, size, i);
  }
  printarr(arr,size);
  //Extract element from heap 
  for (int i=size-1; i>=0; i--){
 	//Move root to end
	swap(&arr[0], &arr[i]);
 	//heapify reduced heap
	heapify(arr,i,0);
  }

}

int main()
{
  int arr[] = {3,6,7,1,4,2,5,9,0,8};
  /* if sizeof is called in same scope as array declaration 
     then it will return size of array (not global scope)
     otherwise it will return just size of pointer*/
  int size = sizeof(arr) / sizeof(arr[0]);
  printf("Before sorting\n");
  printarr(arr,size);
  heap_sort(arr, size);

  printarr(arr,size);
return 0;
}
