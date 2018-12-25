#include<stdio.h>
#include<stdlib.h>
#include "minheap.h"
#include<limits.h>

//#define INT_MAX 64000

typedef struct AdjNode_ {
    int vertex;
    int edge_weight;
    struct AdjNode_ *next;
} AdjNode;

typedef struct AdjList_ {
   AdjNode *head; 
} AdjList;

typedef struct Graph_ {
    int V;
    AdjList *adj;
} Graph;

void addedge(Graph *G, int src, int dest, int weight)
{
 /*Assuming undirected graphs.
   bidirectional edge between any two vertex
  */
 AdjNode *adjnode = (AdjNode*)malloc(sizeof(AdjNode));
 adjnode->vertex = dest;
 adjnode->edge_weight = weight;
 adjnode->next = G->adj[src].head;
 G->adj[src].head = adjnode;

 adjnode = (AdjNode*)malloc(sizeof(AdjNode));
 adjnode->vertex = src;
 adjnode->edge_weight = weight;
 adjnode->next = G->adj[dest].head;
 G->adj[dest].head = adjnode;
}

Graph *create_graph(int num_vertex)
{
    Graph *G = (Graph*)malloc(sizeof(Graph));
    G->V = num_vertex;
    G->adj = (AdjList*)malloc(G->V * sizeof(AdjList));
    for (int i=0; i<num_vertex; i++){
	G->adj[i].head = NULL;
    }
return G;    
}

void printDist(int dist[], int num_vertex)
{
    printf("Dist from Source vertex\n");
    for (int i=0; i<num_vertex; i++){
	printf("Dest vertex :%d %d \n",i,dist[i]);
    }
}

void dijkstra(Graph *G, int src)
{
  int num_vertex = G->V;
  int dist[num_vertex];
  MinHeap *heap = create_minheap(num_vertex);
  heap->size = num_vertex; 

  for(int i=0; i<num_vertex; i++) {
      dist[i] = INT_MAX;
      heap->array[i] = newHeapNode(i,dist[i]);
      heap->pos[i]   = i; 
  }
  dist[src] = 0;
  decrease_key(heap, src, dist[src]);

/*
  Each vertex should be visited once. Root of the min heap will have the vertex with min distance.
  Extract min(vertex) from the heap...relax all outgoing edges from this vertex 
*/

  while(heap->size!=0){
      minheapnode *root = extract_min(heap);
      int u = root->v;
      printf("Extracted from minheap vertex:%d \n",u);

      /*Loop through the list of adj for vertex G->adj[root->v] */
      AdjNode *adj_node = G->adj[u].head;

      while(adj_node != NULL ){
          int v = adj_node->vertex;    
          //isInMinHeap ensures that a vertex is processed once
          printf("update dist for :%d dist[u]:%d dist[v]:%d\n",v,dist[u],dist[v]);
          if (isInMinHeap(heap,v) && dist[u] != INT_MAX && 
                                     dist[v] > dist[u] + adj_node->edge_weight){
              dist[v] = dist[u] + adj_node->edge_weight;
              printf("Updated distance for v:%d dist:%d\n",v,dist[v]);
              decrease_key(heap,v,dist[v]);
          }
          adj_node = adj_node->next;
      }
  }
  printDist(dist,num_vertex);
}

void printGraph(Graph *G)
{
    int num_vertex = G->V;
    for (int i=0; i<num_vertex; i++) {
        AdjNode *node = G->adj[i].head;
        printf("Edges from vertex:   V:%d-->   ",i);
        while(node != NULL){
            printf("V:%d W:%d    ",node->vertex,node->edge_weight);
 	    node = node->next;
        }
        printf("\n");
    }
}

int main()
{
    Graph *G = create_graph(9);
    addedge(G, 0, 1, 4);
    addedge(G, 0, 7, 8);
    addedge(G, 1, 2, 8);
    addedge(G, 1, 7, 11);
    addedge(G, 2, 3, 7);
    addedge(G, 2, 8, 2);
    addedge(G, 2, 5, 4);
    addedge(G, 3, 4, 9);
    addedge(G, 3, 5, 14);
    addedge(G, 4, 5, 10);
    addedge(G, 5, 6, 2);
    addedge(G, 6, 7, 1);
    addedge(G, 6, 8, 6);
    addedge(G, 7, 8, 7);
    //printGraph(G);   
    dijkstra(G, 0);
 
    return 0;
}
