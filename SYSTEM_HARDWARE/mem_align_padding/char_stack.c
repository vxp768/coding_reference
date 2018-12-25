#include<stdio.h>

void func(char a, char b)
{
    printf("addr diff %0x\n",(&a - &b)); //stack grows downward
}

int main()
{
  char a, b;

  func(a,b);
return 0;
}
