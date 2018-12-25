#include<stdio.h>
#include<stdlib.h>

typedef struct ArrayQueue_ {
    int head,tail;
    int capacity;
    int *array;
} ArrayQueue;

void create(ArrayQueue **ArrQ,  int capacity)
{
    ArrayQueue *Q = (ArrayQueue*)malloc(sizeof(ArrayQueue));
    Q->head = -1;
    Q->tail = -1;
    Q->capacity = capacity;
    Q->array = (int*)malloc(sizeof(int)*capacity);
    *ArrQ = Q;
}

int is_full(ArrayQueue *Q)
{
    if ((Q->tail+1)%Q->capacity == Q->head) {
       printf("Queue is full\n"); 
       return 1;
    } else {
       return 0;
    }
}

int is_empty(ArrayQueue *Q)
{
    if(Q->tail == -1) {
        return 1;
    }
    return 0;
}
void EnQueue(ArrayQueue *Q, int data)
{
    if(is_full(Q))
       return;
    Q->tail = (++Q->tail)%Q->capacity;
    Q->array[Q->tail] = data;
    if(Q->head == -1) {
        Q->head = Q->tail;
    }
}
int DeQueue(ArrayQueue *Q)
{
    int data;
    if(is_empty(Q))
        return -1;
    data = Q->array[Q->head];
    if (Q->head == Q->tail) {
        Q->head = Q->tail = -1;
    } else {
        Q->head = (++Q->head)%Q->capacity;
    }
    return data;
}

void display(ArrayQueue *Q)
{
    int h = Q->head, t = Q->tail;
    if(h<=t){
       for(int i=h; i<=t; i++)
          printf("%d\t", Q->array[i]);
    } else {
       for(int i=h; i<Q->capacity; i++)
          printf("%d\t", Q->array[i]);
       for(int i=0; i<=t; i++)
          printf("%d\t", Q->array[i]);
    }
    printf("\n");
}

int main()
{
    ArrayQueue *Q=NULL;
    create(&Q,10);
    EnQueue(Q,2);
    EnQueue(Q,3);
    EnQueue(Q,4);
    EnQueue(Q,1);
    display(Q);
    DeQueue(Q);
    display(Q);
}
