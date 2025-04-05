#include<iostream>

using namespace std;

const int inf=-1;

class Graph{
private:
    int Vertices;
    int** Edges;

public:
    Graph(int Vertices);
    ~Graph();

    void addEdge(int u,int v,int w);
    void printGraph();
};


Graph::Graph(int Vertices){
    this->Vertices=Vertices;
    Edges = new int*[Vertices];
    for(int i=0;i<Vertices;++i)
    {
        Edges[i]=new int[Vertices];
        for(int j=0;j<Vertices;++j){
            Edges[i][j]=inf;
        }
    }
}

Graph::~Graph(){
    for(int i=0;i<Vertices;++i){
        delete[] Edges[i];
    }
    delete[] Edges;
}

void Graph::addEdge(int u,int v,int w){
    Edges[u][v]=w;
}

void Graph::printGraph(){
    for(int i=0;i<Vertices;++i){
        for(int j=0;j<Vertices;++j){
            std::cout<<Edges[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}

int main()
{
    int Vertices=5;
    Graph G(Vertices);
    G.addEdge(0,1,3);
    G.addEdge(1,0,3);
    G.addEdge(1,2,4);
    G.addEdge(2,1,4);
    G.addEdge(3,4,7);
    G.addEdge(4,3,7);
    G.addEdge(4,2,9);
    G.addEdge(2,4,9);

    cout << "邻接矩阵表示的图：" << endl;
    G.printGraph();

    return 0;

}