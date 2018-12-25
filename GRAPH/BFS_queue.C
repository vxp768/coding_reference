#include<iostream>
#include<list>

using namespace std;

class Graph {
    int V;
    list<int> *adj;
  public:
    Graph(int V);
    void addEdge(int v, int w);
    void BFS(int s);
};
Graph::Graph(int V)
{
    this->V = V;
    this->adj = new list<int>[V];
}
void Graph::addEdge(int v, int w)
{
   this->adj[v].push_back(w);
}
void Graph::BFS(int s)
{
    bool *visited = new bool[V];
    list<int>::iterator it;
    list<int> queue;
    int popped_vertex;

    for(int i = 0; i<V; i++)
       visited[i] = false;
    visited[s] = true;
    queue.push_back(s);

    while(!queue.empty()){
        popped_vertex = queue.front();
        cout <<popped_vertex <<" ";
        queue.pop_front();
        for(it = adj[popped_vertex].begin(); it != adj[popped_vertex].end(); ++it) {
            if(!visited[*it]) {
                visited[*it] = true;
		queue.push_back(*it);
            }
        }
    }
}
int main()
{
return 0;
}
