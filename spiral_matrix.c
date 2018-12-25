#include<stdio.h>

#define ROW 4
#define COL 4

/*int A[3][3] = {{1,2,3},
               {4,5,6},
               {7,8,9}};
*/

int A[4][4] = {{1,2,3,4},
               {5,6,7,8},
               {9,10,11,12},
	       {13,14,15,16}};

int main(void)
{
 int i=0, j=0;
 int r1=0,r2=ROW-1;
 int c1=0,c2=COL-1;
 
 while (r1 <= r2 && c1<= c2) {
     ///AHEAD
     while(j<=c2){
         printf("%d ",A[i][j]);
         j++;
     }
     j=j-1;
     ///DOWN
     i++;
     while(i<=r2) {
         printf("%d ",A[i][j]);
         i++;
     }
     i--;
     ///BACK
     j--;
     while(j>=c1) {
         printf("%d ",A[i][j]);
         j--;
     }
     j++;
     ///UP
     i--;
     while(i>r1) {
         printf("%d ",A[i][j]);
         i--;
     }
     //after this its like orig matrix with one less row and col
     r1 = r1 + 1;
     r2 = r2 - 1;
     c1 = c1 + 1;
     c2 = c2 - 1;
     i = r1;
     j = c1;
 }
printf("\n");
return 0;
}
