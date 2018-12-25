#include<stdio.h>

typedef struct Node_ {
    int data;
    struct Node_  *next;
} Node;

Node *create()
{
}

void push(Node **top, int data)
{
    Node *node;
    node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = *top;
    *top = node;
}

int pop(Node **top)
{
    Node *node;
    int data;
    if(is_empty(top)) {
       printf("Stack empty\n");
    } else {
       node = *top;
       *top = *top->next;
       data = node->data;
       return data;
    }
}

int main()
{
   Node *top=NULL;

   push(&top,1);

return 0;
}
