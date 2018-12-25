#include<stdio.h>

struct b {
    char c; //3 byte padding as next one is int
    int i;
    unsigned short s; //2byte padding
}; //12 bytes

struct b b_test = {12,24,14};
/*
 Compile with "gcc -S file.c"
test:
        .byte   12
        .zero   3
        .long   24
        .value  14
        .zero   2
        .section        .rodata
        .align 8
*/

struct b_reorder {
    char c; //1 byte padding
    unsigned short s; //char + 1byte padding + 2 byte short
    int i;
}; //8 bytes

struct b_reorder b_reorder_test = {12,24,14};

struct b_packed {
    char c;
    int i;
    unsigned short s;
}__attribute__((packed,aligned(1))); //7  bytes
//}__attribute__((packed)); //7  bytes default aligned param is 1

struct b_packed b_packed_test = {12,24,14};

struct c {
    char c;
    int i;
    unsigned short s;
    unsigned short d;
}; //12  bytes
struct c c_test = {12,24,14,25};

struct c_packed_8 {
    char c;
    int i;
    unsigned short s;
    unsigned short d;
}__attribute__((packed,aligned(8))); //16  bytes
struct c_packed_8 c_packed_8 = {12,24,14,25};

struct s {
 char a;
 double i;
 int d;
};

struct s test = {3,5,2};

int main(int argc, char *argv[])
{
    printf("Size of struct b with padding :%lu\n",sizeof(struct b));
    printf("Size of struct b_reorder with padding :%lu\n",sizeof(struct b_reorder));
    printf("Size of struct b_packed :%lu\n",sizeof(struct b_packed));
    printf("Size of struct c: %lu\n",sizeof(struct c));
    printf("Size of struct c_packed_8 :%lu\n",sizeof(struct c_packed_8));
    printf("Size of struct s: %lu\n",sizeof(struct s));
return 0;
}
