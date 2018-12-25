#include<stdio.h>

int main()
{

  int b[2] = {1,2};
  int c[2][2] = {{ 1,2 },
                 { 3,4 }};
  int arr[2][3][4];

  for (int i=0; i<2; i++) {
      printf("%d\t",b[i]);
  }
  printf("\n");
  printf("b:%0x\n",b);
  printf("*b:%0x\n",*b);
  printf("*(b+1):%0x\n",*(b+1));


  printf("\n");
  for (int i=0; i<2; i++) {
      for (int j=0; j<2; j++) {
          printf("%d\t",c[i][j]);
      }
      printf("\n");
  }
  printf("\n");
  printf("c:%0x\n",c);
  printf("c+1:%0x\n",c+1);
  printf("*c:%0x\n",*c);
  printf("*(*c):%0x\n",*(*c));
  printf("*(*c+1):%0x\n",*(*c+1));
  printf("*(c+1):%0x\n",*(c+1));
  printf("**(c+1):%0x\n",**(c+1));
  printf("*(c+2):%0x\n",*(c+2));

}
