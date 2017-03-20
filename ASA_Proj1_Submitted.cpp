// ASA Project 1
// 2016/2017 - 2st Semestre
// Instituto Superior Tecnico
// U.C. Análise e Síntese de Algoritmos
// Francisco Barros, nº 85069
// Rafael Ribeiro, nº 84758

//Must be compiled with "g++ -std=c++11 ..."

#include <stdio.h>
#include <iostream>
#include <list>
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
int sortedNumber = 0;

class Graph {
	private:
		bool _incoherency;			/* default is set to false */
		bool _insuficiency;			/* default is set to false */
		int _edges;
		int _vertices;
		int *_color;
		int *_topology;
		list<int> *_adjacencyList;

	public:
		Graph(int vertices, int edges);
		~Graph();
		bool hasPath(int source, int destination);
		bool uniqueSolution();
		bool isIncoherent()			{ return _incoherency; }
		bool isInsuficient()		{ return _insuficiency; }
		void toString();
		void newEdge(int source, int destination);
		void DFS();
		void DFSVisit(int sourceVertex);
		
};

/*------------------------------------------------------------------------------
//	MAIN FUNCTIONS
------------------------------------------------------------------------------*/
Graph::Graph(int vertices, int edges) {
	this->_incoherency = false;
	this->_insuficiency = false;
	this->_edges = edges;
	this->_vertices = vertices;
	this->_color = new int[vertices];
	this->_topology = new int[vertices];
	this->_adjacencyList = new list<int>[vertices];
	
	for (int i = 0; i < vertices; i++) {
		this->_color[i] = WHITE;
	}
}

Graph::~Graph() {
	delete[] this->_color;
	delete[] this->_topology;
	delete[] this->_adjacencyList;
}

void Graph::newEdge(int source, int	destination) {
	_adjacencyList[source - 1].push_front(destination);
	if (source == destination) {
		this->_incoherency = true;
	}
}

void Graph::DFS() {
	int i;
	int vertices = this->_vertices;
	for (i = 0; i < vertices; i++)
		if (this->_color[i] == WHITE)
			DFSVisit(i);
}

void Graph::DFSVisit(int sourceVertex) {
	int index;
	list<int> li = _adjacencyList[sourceVertex];

	this->_color[sourceVertex] = GREY;
		
	for (list<int>::const_iterator ci = li.begin(); ci != li.end(); ++ci) {
		index = *ci - 1;
		if (this->_color[index] == GREY) {
			this->_incoherency = true;
		} else if (this->_color[index] == WHITE) {
			DFSVisit(index);
		}
	}
	this->_color[sourceVertex] = BLACK;
	this->_topology[this->_vertices - sortedNumber++ - 1] = sourceVertex + 1;
}

bool Graph::uniqueSolution() {
	int i;
	int destinationVertex;
	int sourceVertex;
	int requiredEdges = this->_vertices - 1;
	int countedEdges = 0;
	
	for (i = 0; i < requiredEdges; i++) {
		sourceVertex = _topology[i] - 1;
		destinationVertex = _topology[i + 1];

		if (hasPath(sourceVertex, destinationVertex)) {
			countedEdges++;
		}

		if (countedEdges != (i + 1) ) {
			_insuficiency = true;
			return false;
		}
	}
	return true;
}

/*------------------------------------------------------------------------------
//	AUXILIAR FUNCTIONS
------------------------------------------------------------------------------*/

bool Graph::hasPath(int source, int destination) {
	list<int>::const_iterator l;
	for(l = this->_adjacencyList[source].begin(); l != this->_adjacencyList[source].end(); ++l)
		if (*l == destination)
			return true;
	return false;
}
void Graph::toString() {
	int i;
	for (i = 0; i < this->_vertices - 1; i++)
		cout << this->_topology[i] << " ";
	cout << this->_topology[i] << endl;
}

/*------------------------------------------------------------------------------
--------------------------------------------------------------------------------

//	MAIN

--------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
int main() {
	int i;
	int edges, vertices;
	int source, destination;
	
	scanf("%d %d", &vertices, &edges);
	Graph graph(vertices, edges);
	
	for (i = 0; i < edges; i++) {
		scanf("%d %d", &source, &destination);
		graph.newEdge(source, destination);
	}

	if (graph.isIncoherent()) {
		cout << "Incoerente" << endl;
		return 0;
	}
	
	graph.DFS();
	
	if (graph.isIncoherent()) {
		cout << "Incoerente" << endl;
		return 0;
	}

	graph.uniqueSolution();

	if (graph.isInsuficient()) cout << "Insuficiente" << endl;
	else if (!graph.isInsuficient() && !graph.isIncoherent()) graph.toString();
	return 0;
}
