#include<stdio.h>

union a {

    struct b{
      int a;
      double b;
      char c;
    }bb;

    float f;
 
};

union f {
 int a;
 unsigned short c;
 double d;
 long double e;
};

int main()
{

  printf("Size of union a is: %lu\n",sizeof(union a));
  printf("Size of union f is: %lu\n",sizeof(union f));

return 0;
}
