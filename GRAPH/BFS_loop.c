#include<stdio.h>
#include<stdlib.h>

typedef struct Edge_ {
    int vertex;
    struct Edge_ *next;
}Edge;

void AddEdge(Edge **head, int vertex)
{
  Edge *temp = (Edge*)malloc(sizeof(Edge));
  Edge *parse;
  temp->next = NULL;
  temp->vertex = vertex;
  if (*head == NULL){
      *head = temp;
      return;
  }
  parse = *head;
  while (parse->next != NULL) {
      parse = parse->next;
  }
  parse->next = temp;
}

/*Using only loop...better version is using queue*/

void BreadthFirstSearch(Edge *AdjList[], int V, int *parent, int *level, int startvertex)
{
 /* start from the given vertex and find the vertices on next level */
 Edge *temp; 
 int par, lvl, flag=1;
 lvl = 0;
 level[startvertex]=lvl; 

 while(flag){
     flag=0; 
     for (int i=0; i<V; i++) {
         if(level[i] == lvl) {
             flag = 1;
 	     temp = AdjList[i];
 	     par = i;
             while(temp != NULL) {
                     if (level[temp->vertex] != -1 ) {
 			 temp = temp->next;
 			 continue;
		     }
                     level[temp->vertex] = lvl + 1;
 		     parent[temp->vertex] = par;
 		     temp = temp->next;
             }
         }
     }
     lvl++;
 }
}

int main()
{
    int start_vertex;
    int v1, v2,V,E;
    printf("Enter number of vertices\n");
    scanf("%d",&V);
    Edge *AdjList[V];
    int Parent[V], Level[V];
    printf("Enter number of Edges\n");
    scanf("%d",&E);

    /*Initialize all the pointers in adjlist to NULL */
    for (int i=0; i<V; i++) {
        AdjList[i] = NULL;
        Parent[i] = -1;
        Level[i] = -1;
    }
    /* Add all the edges*/
    printf("Add all the edges\n");
    for (int j=0; j<E; j++) {
        scanf("%d%d",&v1,&v2);
        if (v1>=V || v2>=V) {
                printf("Vertex out of bound\n");
                return (1);
        }
        AddEdge(&AdjList[v1],v2);
        /*If undirected*/
        AddEdge(&AdjList[v2],v1);
    } 
    /* Enter start vertex */
    printf("Enter start vertex: ");
    scanf("%d",&start_vertex);
    printf("\n");
    BreadthFirstSearch(AdjList,V,Parent,Level,start_vertex);
return 0;
}
