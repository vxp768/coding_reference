#include<stdio.h>

int A[] = {-2, -3, 4, -1, -2, 1, 5, -4};
int max(int a, int b)
{
  if (a > b)
     return a;
  else
     return b;
}
int main()
{
 int size = sizeof(A)/sizeof(A[0]);
 int start = 0, end = 0, s = 0;
 int max_so_far = A[0],  max_subarr = A[0];

 for (int k=1; k<size; k++) {
     if (max_subarr + A[k] <= A[k]){
         max_subarr = A[k];
         s = k;
     } else {
         max_subarr = max_subarr + A[k];
     }
     if (max_subarr > max_so_far) {
	max_so_far = max_subarr;
        start = s;
        end = k;
     }
 } 
 printf("Max subarr:%d\n",max_so_far);
 printf("start:%d end:%d\n",start,end);
return 0;
}
