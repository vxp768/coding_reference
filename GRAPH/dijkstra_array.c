#include<stdio.h>
#include<stdlib.h>

#define NUM_VERTICES 9
#define INFINITY 63000

int D[NUM_VERTICES];  //for the distance from source to given vertex...every iteration we pick vertex with min distance
int SP[NUM_VERTICES]; //for the set of vertices processed and tracks the shortest path

int pick_vertex_min_distance()
{
   int min_value=INFINITY;
   int index;
   for (int i=0; i<NUM_VERTICES; i++){
      if(D[i] < min_value && SP[i]==-1){ //need to make sure the vertex was not been used earlier
         min_value = D[i];
         index = i;
      }
   }
   printf("Min Vertex returned:%d\n",index);
   return index;
}

void print_min_distance()
{
   printf("Vertex   Distance from Source\n");
   for (int i = 0; i < NUM_VERTICES; i++)
      printf("%d \t\t %d\n", i, D[i]);
}
void dijkstra(int G[NUM_VERTICES][NUM_VERTICES], int s)
{
   int min_v;

   /* */
   for (int i=0; i< NUM_VERTICES; i++) {
        min_v = pick_vertex_min_distance();
        SP[min_v] = 1;  /*to make sure a vertex is chosen only once */

        /*Find outgoing edges from vertex min_v and update the distance*/
        for (int k=0; k<NUM_VERTICES; k++){
            /*For vertex not picked yet and if edge exists update D if better path available*/
            if(SP[k]== -1 && G[min_v][k] && D[min_v]!=INFINITY && D[k] > D[min_v]+G[min_v][k]) {
               D[k]= D[min_v] + G[min_v][k];
               printf("Min Distance updated for D[%d]:%d\n",k,D[k]);
            }
        }
   }
}

int main()
{
        //Adjacency matrix with weights
	int G[NUM_VERTICES][NUM_VERTICES] = {
		{0, 4, 0, 0, 0, 0, 0, 8, 0},
		{4, 0, 8, 0, 0, 0, 0, 11, 0},
		{0, 8, 0, 7, 0, 4, 0, 0, 2},
		{0, 0, 7, 0, 9, 14, 0, 0, 0},
		{0, 0, 0, 9, 0, 10, 0, 0, 0},
		{0, 0, 4, 0, 10, 0, 2, 0, 0},
		{0, 0, 0, 14, 0, 2, 0, 1, 6},
		{8, 11, 0, 0, 0, 0, 1, 0, 7},
		{0, 0, 2, 0, 0, 0, 6, 7, 0} 
	};
   for(int i=0; i<NUM_VERTICES; i++){
      D[i] = INFINITY;
      SP[i] = -1; //to indicate the vertex is invalid...replace with valid vertex number
   }
   D[0] = 0; //assuming the source vertex is "0"
   /* Find shortest path form source to all other vertices */
   dijkstra(G,0);   

  print_min_distance();
return 0;
}
