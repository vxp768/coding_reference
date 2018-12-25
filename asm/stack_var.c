#include<stdio.h>

void foo(void) {

int arr;
int add1,add2;

add1=4;
arr = 5;
add2= add1+arr;
printf("sum is %d\n",add2);
}

int main(void) {
foo();
}
