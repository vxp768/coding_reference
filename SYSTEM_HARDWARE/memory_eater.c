#include<stdio.h>
#include<stdlib.h>
#include<string.h>

unsigned long long memory_to_eat = 1024 * 10000000; //1000KB

int eaten_memory = 0;
void *memory = NULL;
int data=1;

/*
 Allocate memory in steps of 1KB that is 1024 bytes
*/
int eat_kilobyte()
{
    memory = realloc(memory,(eaten_memory*1024)+1024);
    if (memory == NULL)
    {
        //realloc failed
        return 1;
    } else {
        eaten_memory++;  
        memcpy((memory+eaten_memory),&data,4); 
        return 0;
    }
}

int main(int argc, char *argv[])
{
    int megabyte = 0;
    int megabyte_total=0;
    while(memory_to_eat > 0) {
        memory_to_eat--;
        if (eat_kilobyte()) {
            printf("Failed to allocate memory. Allocated :%d KB\n",eaten_memory);
            while(1);
            return 100;
        } 
        if (megabyte++ >= 1024) {
            megabyte_total++;
            printf("Eaten 1 MB of RAM and total %d MB of RAM\n",megabyte_total);
            megabyte = 0;
        }
    }
    printf("Successfully eaten requested memory\n");
    free(memory);
    return 0;
}

