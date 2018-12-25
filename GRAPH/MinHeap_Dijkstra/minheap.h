

#define LEFT_CHILD(i)  ((2*i) + 1)
#define RIGHT_CHILD(i) ((2*i) + 2)

typedef struct minheapnode_ {
    int v;
    int dist; //used as key in heap
}minheapnode;

typedef struct MinHeap_ {
    int size; //gives number of valid data in heap
    int capacity;
    int *pos; //hold position of any vertex in heap array
    minheapnode **array; //to hold data....minheapnode
}MinHeap;


MinHeap *create_minheap(int capacity);
minheapnode *newHeapNode (int vertex, int dist);
void min_heapify(MinHeap *heap, int idx);
minheapnode *extract_min(MinHeap *heap);
void decrease_key(MinHeap *heap, int v, int dist);
int isInMinHeap(MinHeap *heap, int v);
