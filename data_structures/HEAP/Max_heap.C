#include<iostream>
#include<climits>
using namespace std;
class MaxHeap {
   int heap_size;
   int capacity;
   int *heap_arr;
public:
   MaxHeap(int arr[], int size, int cap);
   int parent (int i) {return (i-1)/2;}
   int left_child (int i) {return (2*i +1);}
   int right_child (int i) {return (2*i +2);}
   void max_heapify (int i);
   int extract_max();
};
void swap (int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}
MaxHeap::MaxHeap (int arr[], int size, int cap)
{
   heap_arr = arr;
   heap_size = size;
   capacity  = size;
   int i = (heap_size -1)/2;
   while (i>=0) {
       max_heapify(i);
       i--;
   }
}
void MaxHeap::max_heapify (int i)
{
   int left  = left_child(i);
   int right = right_child(i);
   int max   = i; 
   if (left < heap_size && heap_arr[left] > heap_arr[max]) {
       max = left;
   }
   if (right < heap_size && heap_arr[right] > heap_arr[max]) {
       max = right;
   }
   if (max != i) {
       swap(&heap_arr[i], &heap_arr[max]);
       max_heapify(max);
   }
}
int MaxHeap::extract_max ()
{
   if (heap_size == 0)
       return INT_MAX;
   int root = heap_arr[0];
   if (heap_size > 1) {
       heap_arr[0] = heap_arr[heap_size-1];
       max_heapify(0);
   }
   heap_size--;
   return root;
}
int Klargest (int arr[], int size, int k)
{
  if (k > size)
     return -1;
  MaxHeap H(arr, size, size);
  for (int i=0; i<k; i++) {
      cout<<H.extract_max()<<" ";
  }
  cout<<endl;
return 0;
}
int Ksmallest (int arr[], int size, int k)
{
  MaxHeap H(arr, k, size);
  for (int i=k; i<size; i++) {
     if (arr[i] < arr[0]) {
         swap(&arr[i], &arr[0]);
         H.max_heapify(0);
     } 
  }
  //in reverse order...can be sorted
  for (int i=0; i<k; i++)
    cout<<arr[i]<<" ";
  cout<<endl;
}
int main()
{
  int arr[] = {15, 19, 2, 8, 6, 7, 1, 10};
  int size = sizeof(arr)/sizeof(arr[0]);
  //Klargest(arr, size, 4);
  Ksmallest(arr, size, 4);
}
