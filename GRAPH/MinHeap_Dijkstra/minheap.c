#include<stdio.h>
#include<stdlib.h>
#include "minheap.h"

MinHeap *create_minheap(int capacity)
{
    MinHeap *minheap = (MinHeap*)malloc(sizeof(MinHeap));
    minheap->size = 0;
    minheap->capacity = capacity;
    minheap->pos   = (int*)malloc(capacity * sizeof(int));
    minheap->array = (minheapnode**)malloc(capacity * sizeof(minheapnode*));

return minheap;
}

minheapnode *newHeapNode (int vertex, int dist)
{
    minheapnode *node = (minheapnode*)malloc(sizeof(minheapnode));
    node->v = vertex;
    node->dist = dist;
return node;
}

void swapHeapNodes(minheapnode **n1, minheapnode **n2)
{
    minheapnode *temp;
    temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

/*Heapify around index idx */
void min_heapify(MinHeap *heap, int idx)
{
    int left  = LEFT_CHILD(idx);
    int right = RIGHT_CHILD(idx);
    int min   = idx;

    //find min among root and its child....also make sure child exists
    if (left < heap->size && heap->array[left]->dist < heap->array[idx]->dist)
        min = left;
    if (right < heap->size && heap->array[right]->dist < heap->array[idx]->dist)
        min = right;

    //Swap nodes if idx did not have min data
    if (min != idx){
        //swap positions
        printf("Swap positions for vertex idx:%d min:%d\n",heap->array[idx]->v, heap->array[min]->v);
        heap->pos[heap->array[idx]->v] = min;
        heap->pos[heap->array[min]->v] = idx;
        swapHeapNodes(&heap->array[idx], &heap->array[min]);    

        /*after swapping min's index is for idx so need to call minheapify for min*/
        min_heapify(heap,min);
    }
}

minheapnode *extract_min(MinHeap *heap)
{
    if(heap->size == 0)
       return NULL;
    /* swap root with last node and return root */
    minheapnode *root = heap->array[0];

    minheapnode *last_node = heap->array[heap->size-1]; 
    heap->array[0] = last_node;

    heap->pos[root->v] = heap->size - 1;
    heap->pos[last_node->v] = 0;

    heap->size--;
    min_heapify(heap,0);

return root; 
}

/* Decrease dist value of vertex */
void decrease_key(MinHeap *heap, int v, int dist)
{
    int arr_loc = heap->pos[v];
    printf("Decrease key for vertex:%d dist:%d\n",v,dist);
    //find position of vertex and update its dist value
    minheapnode *node = heap->array[arr_loc];
    node->dist = dist;

    /*Heapify the tree upwards of nodes as key has decreased for a node
      parrent of arr_loc is (arr_loc-1)/2*/
    while(arr_loc && node->dist < heap->array[(arr_loc-1)/2]->dist) {
        printf("compare:%d with its root:%d \n",arr_loc,(arr_loc-1)/2);
        heap->pos[heap->array[arr_loc]->v] = (arr_loc-1)/2;
        heap->pos[heap->array[(arr_loc-1)/2]->v] = arr_loc;
        swapHeapNodes(&heap->array[arr_loc], &heap->array[(arr_loc-1)/2]);
        //change location to parent index...need to keep comparing with root till no more swap is required
        arr_loc = (arr_loc-1)/2;
    }
}

int isInMinHeap(MinHeap *heap, int v)
{
    if(heap->pos[v] < heap->size) //pos of v is changed on extraction so this comparison can be used
       return 1;
    return 0;
}
