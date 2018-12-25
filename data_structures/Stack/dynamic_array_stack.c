#include<stdio.h>
#include<stdlib.h>

typedef struct DynamicArrayStack_ {
    int top;
    int capacity;
    int *array;
} DynamicArrayStack;

void DoubleStackSize(DynamicArrayStack *);

DynamicArrayStack *create(int capacity)
{
    DynamicArrayStack *S = (DynamicArrayStack*)malloc(sizeof(DynamicArrayStack));
    S->top = -1;
    S->capacity = capacity;
    S->array = (int*)malloc(capacity * sizeof(int));
    return S;
}
int is_full(DynamicArrayStack *S)
{
  return (S->top == S->capacity -1);
}
int is_empty(DynamicArrayStack *S)
{
  return (S->top == -1);
}

void push(DynamicArrayStack *S, int data)
{
   if (is_full(S)) {
       printf("Stack is full\n");
       DoubleStackSize(S);
   }
   S->array[++S->top] = data;
}

int pop(DynamicArrayStack *S) 
{
    if(is_empty(S)) {
     printf("Stack is empty\n");
     return -1;
    } else {
       return S->array[S->top--];
    }
}

void DoubleStackSize(DynamicArrayStack *S)
{
    int *array_double;
    array_double = (int*)malloc(sizeof(int)*(S->capacity*2));
    S->capacity = S->capacity*2;
    for (int i=0; i<=S->top; i++) {
        array_double[i] = S->array[i];
    }
    free(S->array);
    S->array = array_double;
}

void display(DynamicArrayStack *S)
{
    for(int i=0; i<=S->top; i++) 
       printf("%d\t",S->array[i]);
    printf("\n");
}

int main()
{
  DynamicArrayStack *S=create(1);
  push(S,1);
  display(S);
  push(S,3);
  display(S);
  push(S,5);
  display(S);

  push(S,7);
  push(S,9);
  push(S,4);
  push(S,2);
  display(S);
}
