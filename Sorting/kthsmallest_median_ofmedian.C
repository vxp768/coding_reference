#include<iostream>
#include<algorithm>
#include<climits>
using namespace std;
void swap (int *a, int *b)
{
  int temp = *b;
  *b = *a;
  *a = temp;
}
int partition (int arr[], int l, int r, int medOfMed)
{
   int i;
   //search for median and move it to end to use it as pivot
   for (i=l; i<=r; i++) {
     if(arr[i] == medOfMed)
        break;
   }
   swap(&arr[i], &arr[r]);

   i = l;
   for (int j=l; j<r; j++) {
      if (arr[j] <= medOfMed){
          swap(&arr[j], &arr[i]);
          i++;
      }
   }
   swap(&arr[i], &arr[r]);
   return i;
}
int findMedian (int arr[], int n)
{
  sort(arr, arr+n);
  return arr[n/2];
}
int kthsmallest(int arr[], int l, int r, int k)
{
  if (k>0 && k< r-l+1) {
     int n = r-l+1;

     //divide array in to group of size 5...calculate median of each group and store 
     //in median[] array 
     int i, median[(n+4)/5]; //e.g. to make sure there 3 groups for 11 elems
     for (i=0; i<n/5; i++)
          median[i] = findMedian(arr+l+(i*5), 5);

     if (i*5 < n) { //last group of elems
         median[i] = findMedian(arr+l+(i*5), n%5);
         i++;
     }
     //find median of medians using same function as number of groups
     // of 5 elem could also be large
     int medOfMed = (i==1) ? median[i-1] : kthsmallest(median, 0, i-1, i/2);

     int pos = partition(arr, l, r, medOfMed);

     if (pos-l == k-1)
         return arr[pos];
     if (pos-l > k-1)
         return kthsmallest(arr, l, pos-1, k);
     return kthsmallest(arr, pos+1, r, k-pos+l-1);
  }
  return INT_MAX;
}
int main()
{
int arr[] = {12, 3, 5, 7, 4, 19, 26};
    int n = sizeof(arr)/sizeof(arr[0]), k = 3;
    cout << "K'th: k="<<k<<" smallest element is "
         << kthsmallest(arr, 0, n-1, k)<<endl;
    return 0;
}
