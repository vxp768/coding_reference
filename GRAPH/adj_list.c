#include<stdio.h>
#include<stdlib.h>

int V=5;
int E=5;

typedef struct Edge_ {
    int vertex;
    struct Edge_ *next;
}Edge;

void AddEdge (Edge **head, int vertex)
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

void PrintAdjList(Edge **adjlist)
{
    Edge *temp = NULL;
    for (int i=0; i<V; i++) {
        printf("AdjList for vertex:%d -->",i); 
        temp = adjlist[i];
        while(temp!=NULL){
            printf(" %d ",temp->vertex);
	    temp = temp->next;
        }
        printf("\n");
    }
}

int main()
{
    int v1, v2;
    printf("Enter number of vertices\n");
    scanf("%d",&V);
    Edge *AdjList[V];
    printf("Enter number of Edges\n");
    scanf("%d",&E);

    /*Initialize all the pointers in adjlist to NULL */
    for (int i=0; i<V; i++) {
	AdjList[i] = NULL;
    }

    /* Add all the edges*/
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
    PrintAdjList(AdjList);
return 0;
}
