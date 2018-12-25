#include <stdio.h>
#include <stdint.h>

#define IS_BIG_ENDIAN (!*(unsigned char *)&(uint16_t){1})

void test_endian( void )
{
    int test_var = 1;
    unsigned char test_endian = *(unsigned char*)&test_var;
    int tmep = *(unsigned char *)&(uint16_t){1};
    int t = *(uint8_t*)(&(uint16_t){1});
    printf("t %d \n",t);
    /*var stored as 
      ON little endian
      BYTE:  1st    0
             0      1

      ON BIG endian
      BYTE:  1st      0 
             1        0           
      
      0th byte is different in both case
      so type casting on char means only starting byte is type casted which is 
      0th byte in case of little endian and 3rd byte in case of big endian
    */
    if(test_endian == 1)
            printf("Little endian\n");
    else
            printf("Big endian\n");
}

int main(void)
{
 test_endian();
}

