#include<stdio.h>
#include<string.h>

int main(int argc, char *argv[])
{
 //char str1[] = "1.2.1.1";
 //char str2[] = "1.2.1.2";
 char *saveptr1, *saveptr2;
 char *str1 = argv[1];
 char *str2 = argv[2];
 char *tmp, *tmp1;

 tmp = strtok_r(str1,".",&saveptr1);
 tmp1 = strtok_r(str2,".",&saveptr2);
 while(tmp!=NULL && tmp1!=NULL){
    tmp = strtok_r(NULL,".",&saveptr1);
    tmp1 = strtok_r(NULL,".",&saveptr2);
    if (tmp == NULL && tmp1 != NULL) {
        printf("str2 %s is later\n",str2);
        break;
    } else if (tmp1 == NULL && tmp != NULL) {
        printf("str1 %s is later\n",str1);
        break;
    } else if (tmp == NULL && tmp1 == NULL) {
        printf("Both are same\n");
        break;
    }
    //if (*tmp == *tmp1) {
    if (strcmp(tmp,tmp1)==0) {
        continue;
    } else {
        //if (*tmp < *tmp1) {
        if (strcmp(tmp,tmp1) < 0) {
            printf("str2 %s is later\n",str2);
            break;
        } else {
            printf("str1 %s is later\n",str1);
            break;
        }
    }
 }
return 0;
}
