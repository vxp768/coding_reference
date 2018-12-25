#include<stdio.h>
#include<stdlib.h>

typedef struct ArrayStack_ {
    int top;
    int capacity;
    int *array;
} ArrayStack;

ArrayStack *create(int capacity)
{
    ArrayStack *S = (ArrayStack*)malloc(sizeof(ArrayStack));
    S->top = -1;
    S->capacity = capacity;
    S->array = (int*)malloc(sizeof(int) * capacity);
    return S;
}

int is_empty(ArrayStack *S)
{
    if (S->top == -1)
        return 1;
    else
        return 0;
}
int is_full(ArrayStack *S)
{
    return (S->top == S->capacity -1);
}

void push(ArrayStack *S, int data)
{
    if(is_full(S)) {
	printf("Stack is full\n");
    } else {
        S->top++;
        S->array[S->top] = data; 
    }
}

int pop(ArrayStack *S)
{
    if (is_empty(S)){
  	printf("Stack is empty\n");
        return -1;
    } else {
        return S->array[S->top--];
    }
}

void print_stack(ArrayStack *S)
{
  if (is_empty(S))
       return;
    for(int i=0; i<=S->top; i++)
         printf("%d\t",S->array[i]);
    printf("\n");
}

int main()
{
    ArrayStack *S=NULL;
    S = create(10);
    push(S,1);
    push(S,2);
    push(S,5);
    print_stack(S);
return 0;
}
