#include<stdio.h>

/*

struct hi  
{
   long a;  
   int b;  
   long c;  
};  

 num= (unsigned int) & (((struct hi *)0)->b); 

This gives you the offset in bytes of the b field inside the hi struct

((struct hi *)0) is a pointer to a hi struct, starting at address 0.

(((struct hi *)0)->b) is the b field of the above struct

& (((struct hi *)0)->b) is the address of the above field. Because the hi struct is located at address 0, this is the offset of b within the struct.

(unsigned int) & (((struct hi *)0)->b) is a conversion of that from the address type to unsigned int, so that it can be used as a number.

You're not actually dereferencing a NULL pointer. You're just doing pointer arithmetic.

Accessing (((struct hi *)0)->b) will give you a segmentation fault because you're trying to access a forbidden memory location.

Using & (((struct hi *)0)->b) does not give you segmentation fault because you're only taking the address 
of that forbidden memory location, but you're not trying to access said location.

*/


struct test {
int *a;
int b;
int *c;
};

struct test2 {
int d;
int e;
};

int main()
{
long unsigned int offset=0;
printf("Size of int: %lu\n",sizeof(int));
printf("Size of struct test: %lu \n",sizeof(struct test));

 offset = ((long unsigned int)&(((struct test *)NULL)->a));
printf("offset a %lu\n",offset);

 offset = ((long unsigned int)&(((struct test *)0)->b));
printf("offset b: %lu\n",offset);

 offset = ((long unsigned int)&(((struct test *)0)->c));
printf("offset c %lu\n",offset);


 offset = ((long unsigned int)&(((struct test2 *)0)->d));
printf("test2 offset d %lu\n",offset);

 offset = ((long unsigned int)&(((struct test2 *)0)->e));
printf("test2 offset e %lu\n",offset);

return 0;
}
