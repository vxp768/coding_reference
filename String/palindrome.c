#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char str1[] = "abcdcba";
char str2[] = "asdgh";
char str3[] = "abccba";
char str4[] = "abcba";

void check_palindrome1(char *str1)
{
 int i=0;
 int j=strlen(str1)-1;

 while(str1[i] == str1[j]){
     if(str1[i+1] == '\0') break;
     i++; j--;
 }
 printf("i:%d j:%d\n",i,j);
 if (i == j+ strlen(str1)-1)
   printf("string is palindrome\n");
 else 
   printf("string is not palindrome\n");
}

void check_palindrome2(char *str)
{
   int size = 0;
   while(str[size] != '\0')
        size++;
   int mid = size/2;
   int end = size-1;

   for (int i=0; i<mid; i++){
       if(str[i] != str[end]){
           printf("2nd method Not palindrome\n");
	   return;
       }
       end--;
   }
   printf("2nd method is palindrome\n");
}

int main()
{
check_palindrome1(str1);
check_palindrome1(str2);
check_palindrome2(str3);
check_palindrome2(str4);
}
