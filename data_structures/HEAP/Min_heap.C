#include<iostream>
#include<climits>
using namespace std;
class MinHeap {
  int *heap_arr;
  int heap_size;
  int capacity;
public:
  MinHeap(int arr[], int s, int cap);
  int parent(int i) {return (i-1)/2;}
  int left_child(int i) {return (2*i + 1);}
  int right_child(int i) {return (2*i +2);}
  int extract_min();
  void min_heapify(int i);

  void decreaseKey(int i, int val);
  void insert(int i);
  void delete_key(int i);
};
void swap (int *x, int *y)
{
  int temp;
  temp = *x;
  *x = *y;
  *y = temp;
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
void MinHeap::insert (int key)
{
   if (heap_size == capacity) {
       cout<<"heap overflow"<<endl;
       return;
   }
   heap_size++;
   int i = heap_size -1;
   heap_arr[i] = key;
   while (i!=0 && heap_arr[parent(i)] > heap_arr[i]) {
      swap(&heap_arr[parent(i)], &heap_arr[i]);
      i = parent(i); 
   }
}
void MinHeap::delete_key (int i)
{
    decreaseKey(i, INT_MIN);
    extract_min();
}
void MinHeap::decreaseKey (int i, int new_val)
{
   heap_arr[i] = new_val;
   while (i!=0 && heap_arr[parent(i)] > heap_arr[i]) {
        swap(&heap_arr[parent(i)], &heap_arr[i]);
        i = parent(i);
   }
}
int Ksmallest (int arr[], int size, int k)
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
int Klargest(int arr[], int size, int k)
{
  MinHeap H(arr, k, k);
  for (int i=k; i<size; i++) {
      if (arr[i] > arr[0]) {
          swap(&arr[i], &arr[0]);
          H.min_heapify(0);
      }
  }
  for (int i=0; i<k; i++)
    cout<<arr[i]<<" ";
  cout<<endl;
}
int main()
{
  int arr[] = {15, 19, 2, 8, 6, 7, 1, 10};
  int size  = sizeof(arr)/sizeof(arr[0]);
  //Ksmallest(arr, size, 4);
  Klargest(arr, size, 4);
}
