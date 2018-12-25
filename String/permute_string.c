#include<string.h>
#include<stdio.h>
#include<stdlib.h>

void swap(char *a, char *b)
{
    char temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
//ABC...Time complexity O(n*n!) as there are n! permutations and n for printing n char
void permute (char *str, int left, int right)
{
	if (left == right) {
		printf("%s\n",str);
	} else {
		for (int i = left; i<= right; i++) {
			swap(str + left, str + i);   //Swap A and B
			permute(str, left+1, right);
			swap(str + left, str + i);   //SWAP BACK to make sure we swap A and C
		}
	}
}

int main()
{
    char str[] = "ABDC";
//char *str will not work as its not allowed to modify the string as memory is allocated    
//char *str = "ABC";
    int len = strlen(str);
    permute(str, 0, len-1);
return 0;
}
