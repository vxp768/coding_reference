#include<stdio.h>

//sizeof returns long unsigned int
int main(void)
{

    printf("Sizeof int:           %lu\n",sizeof(int));
    printf("Sizeof unsigned int:  %lu\n",sizeof(unsigned int));
    printf("Sizeof long int:  %lu\n",sizeof(long int));
    printf("Sizeof long unsigned int:  %lu\n",sizeof(long unsigned int));

    printf("Sizeof unsigned long long: %lu\n",sizeof(unsigned long long));
    printf("Sizeof double: %lu\n",sizeof(double));
    printf("Sizeof long double: %lu\n",sizeof(long double));

    printf("Sizeof float: %lu\n",sizeof(float));
return 0;
}
