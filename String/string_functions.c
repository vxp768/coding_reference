#include<stdlib.h>
#include<stdio.h>

/*
sprintf ...appends NULL
snprintf...appends NULL...NULL is included in size passed

strcmp(str1,str2)
strncmp(str1,str2,size)

strcpy(dest,src) ...does not check size of src so can overrun dest buffer and corrupt memory
                 strcpy keeps copying till NULL appears which may never appear
              Manually put '\0' at end of dest

strncpy(dest,src,size)... was initially introduced into the C library to deal with fixed-length 
                          name fields in structures such as directory entries. 
            Such fields are not used in the same way as strings: the trailing null is unnecessary for a maximum-length field, 
            and setting trailing bytes for shorter names to null assures efficient field-wise comparisons.
                         It doesn't put a nul-terminator on the destination if it is completely filled
     	         	 It always completely fills the destination, with nuls if necessary  

            if in size len(dest) -1 is passed then NULL termination is guaranteed

SAFER strcpy is strncat

strcat(dest,src) //will look for NULL-terminator in dest and treat that as end of dest 
 		   appends src to dest overwriting terminating NULL byte at end of dest and adds terminating NULL
                   src should also be NULL terminated
                   dest should be large enough to hold bigger string

strncat(dest, src, size)  size = sizeof(dest) -1 or sizeof(dest)-strlen(dest)-1 ...same as strcat
                          dest should have NULL char for strcat and strncat to work
 			NULL terminator not appended if dest buffer is full so -1 above will guarantee NULL terminator
strdup(str) ...returns pointer to copy of str
strndup(str,size)...returns ptr to copy of str where size char from str was copied and NULL terminated

strchr(str, c)...returns pointer to first occurence of character c in string str
strrchr(str,c)...returns pointer to last occurence of character c in string str
              terminating NULL is considered part of str for both strchr and strrchr 

strstr(str1, str2) ... returns pointer to first occurence of str2 in str1

strtok(char* str, const char *delimit) Each call to strtok() returns a pointer to a null-terminated string containing the next token.
            str is passed only first time then onwards str should be NULL
strtok_r(char *str, const char *delim, char **saveptr)
            str is passed only first time then onwards str should be NULL but saveptr should be unchanged

*/

int main()
{
    char dest[100];
    int d = 10; 
    int len;
    char *src = "again";

  /* here if the num of char in src is greater than size of dest or if src is not NULL terminated
     then there could be buffer overflow  */

    len = sprintf(dest,"hello world %d %s",d,src);
    printf("sprintf: %s  \t len:%d\n",dest,len);

    snprintf(dest, sizeof(dest), "hello world %d %s", d, src);
    printf("snprintf: %s  \t len:%d\n",dest,len);


return 0;
}
