// ASA Project 1
// 2016/2017 - 2st Semestre
// Instituto Superior Tecnico
// U.C. Análise e Síntese de Algoritmos
// Francisco Barros, nº 85069
// Rafael Ribeiro, nº 84758

//Must be compiled with "g++ -std=c++11 ..."

#include <stdio.h>
#include <iostream>
#include <forward_list>
#include <string>

/*------------------------------------------------------------------------------
//	CONSTANTS
------------------------------------------------------------------------------*/
#define WHITE 0
#define GREY 1
#define BLACK 2

using namespace std;

/*------------------------------------------------------------------------------
//	GLOBAL VARIABLES
------------------------------------------------------------------------------*/
int timeCurrent = 0;

class Graph {
	private:
		int graphEdges;
		int graphVertices;
		int *color;
		int *parent;
		int *discoveryTime;
		int *finishTime;
		forward_list<int> *adjacencyList;
	
	public:
		Graph(int vertices, int edges);
		void graphCreateEdge(int source, int destination);
		void graphInitAuxiliar();
		void graphDFS(int sourceVertex);
		void graphDFSVisit(int sourceVertex);
		int graphCheckSources();
		
		int getGraphEdges()		{ return graphEdges; }
		int getGraphVertices()	{ return graphVertices; }
		int *getFinish()		{ return finishTime; }
		int *getDisc()			{ return discoveryTime; }
		void toString();
};

Graph::Graph(int vertices, int edges) {
	this->graphEdges = edges;
	this->graphVertices = vertices;
	this->adjacencyList = new forward_list<int>[vertices];
}

void Graph::graphCreateEdge(int source, int	destination) {
	adjacencyList[source - 1].push_front(destination);
}

void Graph::graphDFS(int sourceVertex) {
	graphInitAuxiliar();
	timeCurrent = 0;
	for (int i = 0; i < this->graphVertices; i++)
		if (this->color[i] == WHITE)
			graphDFSVisit(i);
}

void Graph::graphDFSVisit(int sourceVertex) {
	this->color[sourceVertex] = GREY;
	this->discoveryTime[sourceVertex] = timeCurrent++;
	
	//forward_list<int>::iterator l;
		
	forward_list<int> li = adjacencyList[sourceVertex];
	
	for (forward_list<int>::const_iterator ci = li.begin(); ci != li.end(); ++ci) {

//	for (l = this->adjacencyList[sourceVertex].begin(); l != this->adjacencyList[sourceVertex].end(); l++) {
		int index = *ci - 1;
		if (this->color[index] == WHITE) {
			this->parent[index] = sourceVertex + 1;
			graphDFSVisit(index);
		}
	}
	this->color[sourceVertex] = BLACK;
	this->finishTime[sourceVertex] = timeCurrent++;
}

int Graph::graphCheckSources() { /* insert code here */ }

void Graph::graphInitAuxiliar() {
	int vertices = this->graphVertices;
	this->color = new int[vertices];
	this->parent = new int[vertices];
	this->discoveryTime = new int[vertices];
	this->finishTime = new int[vertices];

	for (int i = 0; i < vertices; i++) {
		this->color[i] = WHITE;
		this->parent[i] = -1;
	}
}

void Graph::toString() {
	forward_list<int>::iterator l;
	for (int i = 0; i < this->graphVertices; i++)
		for(l = this->adjacencyList[i].begin(); l != this->adjacencyList[i].end(); l++)
			cout << "List " << i + 1 << ": " << *l << endl;
}

int main() {
	int i;
	int edges, vertices;
	int source;
	int destination;
	int sourceVertex;

	scanf("%d %d", &vertices, &edges);
	Graph graph(vertices, edges);
	
	for (i = 0; i < edges; i++) {
		scanf("%d %d", &source, &destination);
		graph.graphCreateEdge(source, destination);
	}
	graph.toString();
	graph.graphCheckSources();
	
	graph.graphDFS(0);
	

	int* arrai = graph.getFinish();
	for (int i = 0; i < graph.getGraphVertices(); i++)
		cout << "discovery time do i " << i+1 << " : " << arrai[i] << endl;

	return 0;
}