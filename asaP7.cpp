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
		//Main functions
		Graph(int vertices, int edges);
		~Graph();
		void createEdge(int source, int destination);
		void DFS();
		void DFSVisit(int sourceVertex);
		bool hasUniqueSolution();

		//Auxiliar functions
		bool isIncoherent()	 	{ return _incoherency; }
		bool isInsuficient() 	{ return _insuficiency; }
		void setIncoherent() 	{ _incoherency = true; }
		void setInsuficient() 	{ _insuficiency = true; }
		bool hasPath(int source, int destination);
		friend ostream& operator<<(ostream& os, const Graph &graph);
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
	for (int i = 0; i < vertices; i++) this->_color[i] = WHITE;
}
Graph::~Graph() {
	delete[] this->_color;
	delete[] this->_topology;
	delete[] this->_adjacencyList;
}
void Graph::createEdge(int source, int	destination) {
	//Adds a new edge to the graph.
	_adjacencyList[source - 1].push_front(destination);
	//If it verifies the source and the destination are the same,
	//means it found a cycle, and sets the graph incoherence to true.
	if (source == destination) setIncoherent();
}
void Graph::DFS() {
	//Performs a DFS (Depth First Search) to get a topological order.
	for (int i = 0; i < this->_vertices; i++)
		if (this->_color[i] == WHITE)
			DFSVisit(i);
}
void Graph::DFSVisit(int sourceVertex) {
	int index;
	//Indicates this vertex as been visited (but not all of it's children).
	this->_color[sourceVertex] = GREY;
	list<int> li = _adjacencyList[sourceVertex];
	//Recursevely visits all the children.
	for (list<int>::const_iterator ci = li.begin(); ci != li.end(); ++ci) {
		index = *ci - 1;
		//Checks for back edges (cycles).
		if (this->_color[index] == GREY) setIncoherent();
		else if (this->_color[index] == WHITE) DFSVisit(index);
	}
	//Indicates this vertex as been visited as well as all of it's children.
	this->_color[sourceVertex] = BLACK;
	//Adds the vertex to the topological order array.
	this->_topology[this->_vertices - sortedNumber++ - 1] = sourceVertex + 1;
}
bool Graph::hasUniqueSolution() {
	int countedEdges = 0;
	for (int i = 0; i < this->_vertices - 1; i++) {
		//Checks it there is an edge between each pair in the topological order.
		if (hasPath(this->_topology[i], this->_topology[i + 1])) countedEdges++;
		//If not all two sequencial vertices in the topological order are directly
		//connected, there isn't a unique topological order, wich means the input
		//is insufficient.
		if (countedEdges != (i + 1)) {
			setInsuficient();
			return false;
		}
	}
	return true;
}

/*------------------------------------------------------------------------------
//	AUXILIAR FUNCTIONS
------------------------------------------------------------------------------*/
bool Graph::hasPath(int source, int destination) {
	//Checks if there is an edge from source (array index) to destination (index).
	list<int>::const_iterator l;
	for(l = this->_adjacencyList[source - 1].begin(); l != this->_adjacencyList[source - 1].end(); ++l)
		if (*l == destination) return true;
	return false;
}
ostream& operator<<(ostream& os, const Graph &graph) {
	//Prints the unique topological order (if there is one).
	int i;
	for (i = 0; i < graph._vertices - 1; i++)
		os << graph._topology[i] << " ";
	os << graph._topology[i];
	return os;
}

/*------------------------------------------------------------------------------
--------------------------------------------------------------------------------

//	MAIN

--------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
int main() {
	int edges, vertices;
	int source, destination;
	//Creates the graph structure and adds all the edges.
	scanf("%d %d", &vertices, &edges);
	Graph graph(vertices, edges);
	for (int i = 0; i < edges; i++) {
		scanf("%d %d", &source, &destination);
		graph.createEdge(source, destination);
	}
	//If it already verified incoherency (trough cycles in createEdge), returns
	//imediately.
	if (graph.isIncoherent()) {
		cout << "Incoerente" << endl;
		return 0;
	}

	graph.DFS(); //Performs a DFS (to get a topological solution).

	//If it found the graph to be cyclic in the DFS, returns.
	if (graph.isIncoherent()) {
		cout << "Incoerente" << endl;
		return 0;
	}

	graph.hasUniqueSolution(); //Checks if the graph has a unique topological order.

	if (graph.isInsuficient()) cout << "Insuficiente" << endl; //If it found it doens't, returns.
	else if (!graph.isInsuficient() && !graph.isIncoherent())
		cout << graph << endl; //If it has, prints it.
	return 0;
}
