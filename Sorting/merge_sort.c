#include<stdio.h>

int arr[10]={3,2,0,6,4,8,7,1,9,5};

/*
*/

void merge(int *a, int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int L[n1+1], R[n2+1];

    for(i=0; i<n1; i++) { //left to mid
        L[i] = a[left+i];
    }
    for(j=0; j<n2; j++) { //mid+1 to right
        R[j] = a[mid+1+j];
    }
    L[n1] = 645897; //INFINITY
    R[n2] = 645897; //INFINITY
    i=0; j=0; k=left;
    /*while (i<n1 && j<n2 ) {
        if(L[i] <= R[j]) {
          a[k] = L[i];
          i++;
        } else {
	  a[k] = R[j];
	  j++;
        }
        k++;
    } 
    while (i<n1) {
	a[k] = L[i];
        i++;k++;
    }
    while (j<n2) {
        a[k] = R[j];
	j++;k++;
    } */
    for (k=left; k<=right; k++) {
        if (L[i] < R[j]) {
            a[k] = L[i];
            i++;
        } else {
            a[k] = R[j];
            j++;
        }
    }
}

void merge_sort (int *a, int left, int right)
{
   int mid;
   if (left < right) {
       mid = (left+right)/2;
       merge_sort(a, left, mid);
       merge_sort(a, mid+1, right);
       merge(a,left,mid,right);
   }
}

int main()
{
    merge_sort(arr,0,9);
    for (int i=0; i<10; i++) {
         printf("%d ",arr[i]);
    }
    printf("\n");
return 0;
}
