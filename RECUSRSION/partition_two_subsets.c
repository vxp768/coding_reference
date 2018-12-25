#include <stdio.h>
#include <stdlib.h>
/*
 * 2^n possible subsets ...permutation possible = n!
 * this is why subset problem is different from permutation problem.
 */

int min(int a, int b)
{
  if (a < b )
    return a;
  else 
    return b;
}
int findMinRec(int arr[], int i, int sumCalculated, int sumTotal)
{
	if (i==0)
		return abs((sumTotal-sumCalculated) - sumCalculated);
	return min(findMinRec(arr, i-1, sumCalculated+arr[i-1], sumTotal),
			findMinRec(arr, i-1, sumCalculated, sumTotal));
}
int findMin(int arr[], int n)
{
	int sumTotal = 0;
	for (int i=0; i<n; i++)
		sumTotal += arr[i];
	return findMinRec(arr, n, 0, sumTotal);
}
int main()
{
	int arr[] = {3, 1, 4, 2, 2, 1};
	int n = sizeof(arr)/sizeof(arr[0]);
	printf("The minimum difference between two sets is %d ",findMin(arr,n));
	return 0;
}
