#include<stdio.h>

struct s {
 char a;
 double i;
 int d;
};

struct s test = {3,5,2};

int main(){
 printf("struct size: %lu\n",sizeof(struct s));
return 0;
}
