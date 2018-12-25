#include<iostream>
#include<climits>
using namespace std;
class Heap{
protected:
   int heap_size;
   int capacity;
   int *heap_arr;
public:
   int parent (int i) {return (i-1)/2;}
   int left_child (int i) {return (2*i +1);}
   int right_child (int i) {return (2*i +2);}

   void decreaseKey();
   void insert();
   void delete_key();
};

class MinHeap: public Heap {
public:
   MinHeap(int arr[], int size, int cap);
   void min_heapify (int i);
   int extract_min();
};

class MaxHeap: public Heap {
public:
   MaxHeap(int arr[], int size, int cap);
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
MinHeap::MinHeap(int arr[], int s, int cap)
{
   heap_size = s;
   capacity  = cap;
   heap_arr  = arr;
   int i     = (heap_size -1)/2;
   while (i>=0) {
       min_heapify(i);
       i--;
   }
}
void MinHeap::min_heapify(int i)
{
   int left = left_child(i);
   int right = right_child(i);
   int min = i;
   if (left  < heap_size && heap_arr[left]  < heap_arr[min]) {
       min = left;
   }
   if (right < heap_size && heap_arr[right] < heap_arr[min]) {
       min = right;
   }
   if (min != i) {
       swap(&heap_arr[i], &heap_arr[min]);
       min_heapify(min);
   }
}
int MinHeap::extract_min()
{
   if (heap_size == 0)
       return INT_MAX; //heap is empty
   //extract min value
   int min = heap_arr[0];
   if (heap_size > 1) {
       heap_arr[0] = heap_arr[heap_size-1];
       min_heapify(0);
   }
   heap_size--;
   return min;
}
int Ksmallest(int arr[], int size, int k)
{
  if (k > size)
     return -1;
  MinHeap H(arr, size, size);
  for (int i=0; i<k; i++) {
      cout<<H.extract_min()<<" ";
  }
  cout<<endl;
return 0;
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
int main()
{
  int arr[] = {15, 19, 2, 8, 6, 7, 1, 10};
  int size = sizeof(arr)/sizeof(arr[0]);
  for (int i=0; i<size; i++)
   cout<<arr[i];
  cout<<endl;
  Klargest(arr, size, 4);
  for (int i=0; i<size; i++)
   cout<<arr[i];
  cout<<endl;
  Ksmallest(arr, size, 4);
}
