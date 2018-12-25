#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

typedef struct {
    int src;
    int dest;
    int weight;
} Edge;

typedef struct {
    int V;
    int E;
    Edge *edge;
} Graph;

Graph *creategraph(int V, int E)
{
    Graph *G = (Graph*)malloc(sizeof(Graph));
    G->V = V;
    G->E = E;
    G->edge = (Edge*)malloc(E * sizeof(Edge));
return G;
}

void bellmanford(Graph *G, int src)
{
    int V = G->V;
    int dist[V];
    int u,v,w;
    for(int k=0; k<V; k++){
        dist[k] = INT_MAX; 
    }
    dist[src] = 0;

    for(int i=0; i < G->V; i++) {
        for(int j=0; j<G->E; j++) {
            u = G->edge[j].src;
            v = G->edge[j].dest;
	    w = G->edge[j].weight;
            if(dist[u] != INT_MAX && dist[v] > dist[u]+w) {
	        dist[v] = dist[u] + w;
            }
        } 
    }

    for (int k=0; k<G->E; k++){
	u = G->edge[k].src;
	v = G->edge[k].dest;
	w = G->edge[k].weight;
        if(dist[u] != INT_MAX && dist[v] > dist[u] + w){
	    printf("Negative cycle exists\n");
        }
    }
    printf("Dist form src vertex\n");
    for (int i=0; i<G->V; i++) {
        printf("V:%d dist:%d\n",i,dist[i]);
    } 
}

int main()
{
    int num_vertex=5, num_edge=8;
    Graph *graph = creategraph(num_vertex,num_edge);
  
    graph->edge[0].src = 0;
    graph->edge[0].dest = 1;
    graph->edge[0].weight = -1;
 
    graph->edge[1].src = 0;
    graph->edge[1].dest = 2;
    graph->edge[1].weight = 4;
 
    graph->edge[2].src = 1;
    graph->edge[2].dest = 2;
    graph->edge[2].weight = 3;
 
    graph->edge[3].src = 1;
    graph->edge[3].dest = 3;
    graph->edge[3].weight = 2;
 
    graph->edge[4].src = 1;
    graph->edge[4].dest = 4;
    graph->edge[4].weight = 2;
 
    graph->edge[5].src = 3;
    graph->edge[5].dest = 2;
    graph->edge[5].weight = 5;
 
    graph->edge[6].src = 3;
    graph->edge[6].dest = 1;
    graph->edge[6].weight = 1;
 
    graph->edge[7].src = 4;
    graph->edge[7].dest = 3;
    graph->edge[7].weight = -3;
bellmanford(graph,0);
return 0;
}
