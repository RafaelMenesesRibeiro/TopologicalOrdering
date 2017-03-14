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
#include <algorithm>
#include <vector>

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
int sortedNumber = 0;

class Graph {
	private:
		int graphEdges;
		int graphVertices;
		bool isIncoherent = false;
		bool isInsuficient = false;
		int *sorted;
		int *color;
		int *parent;
		int *discoveryTime;
		int *finishTime;
		forward_list<int> *adjacencyList;

	public:
		Graph(int vertices, int edges);
		void graphCreateEdge(int source, int destination);
		bool graphHasEdge(int source, int destination);
		bool graphUniqueSolution();
		void graphDFS();
		void graphDFSVisit(int sourceVertex);
		int graphCheckSources();

		void graphIncoherent() 		{ isIncoherent = true; }
		void graphInitAuxiliar() 	{ isInsuficient = true; }
		bool getIncoherent()		{ return isIncoherent; }
		bool getInsuficient()		{ return isInsuficient; }

		void printFinish();
		void toString();
		int getGraphEdges()		{ return graphEdges; }
		int getGraphVertices()	{ return graphVertices; }		
};

Graph::Graph(int vertices, int edges) {
	this->graphEdges = edges;
	this->graphVertices = vertices;
	this->adjacencyList = new forward_list<int>[vertices];
}

void Graph::graphCreateEdge(int source, int	destination) {
	adjacencyList[source - 1].push_front(destination);
	if (source == destination)
		this->isIncoherent = true;
}

bool Graph::graphHasEdge(int source, int destination) {
	forward_list<int>::const_iterator l;
	for(l = this->adjacencyList[source - 1].begin(); l != this->adjacencyList[source - 1].end(); ++l)
		if (*l == destination)
			return true;
	return false;
}

bool Graph::graphUniqueSolution() {
	int i;
	int sourceVertex, destinationVertex;
	int requiredEdges = this->graphVertices - 1;
	int countedEdges = 0;
	forward_list<int>::const_iterator ci;
	for (i = 0; i < requiredEdges; i++) {
		sourceVertex = sorted[i];
		destinationVertex = sorted[i + 1];

		for ( ci = this->adjacencyList[sourceVertex-1].begin(); ci != this->adjacencyList[sourceVertex-1].end(); ++ci ) {
			if ( *ci == destinationVertex ) {
				countedEdges++;
				break;
			}
		}
		

		if ( countedEdges != (i + 1) ) {
			cout << "nao tem hamiltoniano" << endl;
			return false;
		}
	}
	cout << "tem hamiltoniano" << endl;
	return true;
}

void Graph::graphDFS() {
	graphInitAuxiliar();
	timeCurrent = 0;
	for (int i = 0; i < this->graphVertices; i++)
		if (this->color[i] == WHITE)
			graphDFSVisit(i);

	for (int i = 0; i < this->graphVertices; i++)
		printf("%d\t", this->sorted[i]);
	printf("\n");
}

void Graph::graphDFSVisit(int sourceVertex) {
	this->color[sourceVertex] = GREY;
	this->discoveryTime[sourceVertex] = timeCurrent++;
	forward_list<int> li = adjacencyList[sourceVertex];	
	for (forward_list<int>::const_iterator ci = li.begin(); ci != li.end(); ++ci) {
		int index = *ci - 1;
		if (this->color[index] == GREY) {
			this->graphIncoherent();
			return;
		}
		if (this->color[index] == WHITE) {
			this->parent[index] = sourceVertex + 1;
			graphDFSVisit(index);
		}
	}
	this->color[sourceVertex] = BLACK;
	this->finishTime[sourceVertex] = timeCurrent++;
	this->sorted[this->graphVertices - sortedNumber++ - 1] = sourceVertex + 1;
}

int Graph::graphCheckSources() { /* insert code here */ }

void Graph::graphInitAuxiliar() {
	int vertices = this->graphVertices;
	this->color = new int[vertices];
	this->parent = new int[vertices];
	this->discoveryTime = new int[vertices];
	this->finishTime = new int[vertices];
	this->sorted = new int[vertices];

	for (int i = 0; i < vertices; i++) {
		this->color[i] = WHITE;
		this->parent[i] = -1;
	}
}

void Graph::printFinish() {
	for (int i = 0; i < this->graphVertices; i++)
		cout << "Finnish time of vertex " << i + 1 << ": " << this->finishTime[i] << endl;
}

void Graph::toString() {
	forward_list<int>::iterator l;
	for (int i = 0; i < this->graphVertices; i++)
		for(l = this->adjacencyList[i].begin(); l != this->adjacencyList[i].end(); l++)
			cout << "List " << i + 1 << ": " << *l << endl;
}



int main() {
	int edges, vertices;
	int source, destination;
	scanf("%d %d", &vertices, &edges);
	Graph graph(vertices, edges);

	for (int i = 0; i < edges; i++) {
		scanf("%d %d", &source, &destination);
		graph.graphCreateEdge(source, destination);
	}
	
	graph.graphDFS();
	graph.printFinish();
	graph.graphUniqueSolution();

	if (graph.getInsuficient())
		cout << "Insuficiente" << endl;

	if (graph.getIncoherent())
		cout << "Incoerente" << endl;


	return 0;
}


/*
void graphCheckSource() {
	int isSource[graphVertices];
	for (int i = 0; i < graphVertices; i++)
		isSource[i] = 1;
	
	for (int i = 0; i < graphVertices; i++)
		for (Node *temp = (&Graph[i])->next; temp != NULL; temp = temp->next)
			isSource[temp->index - 1] = 0;

	printf("Graph heads\n");
	for (int i = 0; i < graphVertices; i++)
		printf("%d\t", isSource[i]);
	printf("\n");
}
*/