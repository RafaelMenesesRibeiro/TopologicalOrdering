/*
// FUNCTIONALITY
// ASA Project 1
// 2016/2017 - 2st Semestre
// Instituto Superior Tecnico
// U.C. Análise e Síntese de Algoritmos
// Francisco Barros, nº 85069
// Rafael Ribeiro, nº 84758
*/

#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------------------------------------
//	CONSTANTS
------------------------------------------------------------------------------*/
#define IGNORE_INDEX 0
#define WHITE 0
#define GREY 1
#define BLACK 2

/*------------------------------------------------------------------------------
//	STRUCTURES
------------------------------------------------------------------------------*/
//Defines the adjancent lists for each vertex in the graph
typedef struct node {
	int index;
	struct node *next;
}Node;

/*------------------------------------------------------------------------------
// FUNCTIONS DECLARIATIONS
------------------------------------------------------------------------------*/
int graphCheckIncoherent();
int graphCheckInsufficient();
Node* newNode(int index, Node* next);
void graphCreate(int V);
void graphCreateEdge(int u, int v);
void graphDFS();
void graphDFSVisit(int u);
void graphCheckSource();

int isCyclic();
int isCyclicUtil(int i, int visited[], int recStack[]);

void initSearchArrays();
void graphPrint();
void listPrint(int list, Node* head);

/*------------------------------------------------------------------------------
//	GLOBAL VARIABLES
------------------------------------------------------------------------------*/
Node *Graph = NULL;
int graphVertices = 0; //number of vertices
int graphEdges = 0; //number of edges
int time = 0; //counter for DFS


int* color; //array that stores the color in DFS
int* disc; //array that stores the discovery time of a vertex
int* father; //array that stores the father vertex of a vertex
int* topolSorted; //array that stores the topologicaly sorted vertices
int sortedNumber = 0; //counter that stores the number of sorted vertices

int insufficient = 0; //flag that stores if input is insufficient


/*------------------------------------------------------------------------------
// SOURCE CODE
------------------------------------------------------------------------------*/
int main(int argc, char** argv) {
	int u, v = 0;
	//Gets the graphVertices and graphEdges
	scanf("%d %d", &graphVertices, &graphEdges);
	//Create the graph with the input specifications
	graphCreate(graphVertices);
	//Adds the edges given by input	
	for (int i = 0; i < graphEdges; i++) {
		scanf("%d %d", &u, &v);
		graphCreateEdge(u, v);
	}

	//Prints the graph
	graphPrint();
	//Checks how many heads exist
	graphCheckSource();
	//DFS the graph
	graphDFS();

	//Prints project output
	if (graphCheckIncoherent()) printf("INCOERENTE\n");
	else if (graphCheckInsufficient()) printf("INSUFICIENTE\n");
	return 0;
}

/*------------------------------------------------------------------------------
// FUNCTIONS CODE
------------------------------------------------------------------------------*/
int graphCheckIncoherent() {
	if (insufficient) return 1;
	return 0;
}

int graphCheckInsufficient() {
	//Checks if the minimum number of connections is respected.
	if (graphEdges < graphVertices - 1)
		return 1;
	return 0;
}

Node* newNode(int index, Node* next) {
	Node* n = malloc(sizeof(Node));
	n->index = index;
	n->next = next;
	return n;
}
void graphCreate(int V) {
	Graph = malloc(V * sizeof(Node));
	for (int i = 0; i < V; i++)
		Graph[i] = (Node){IGNORE_INDEX, NULL};
}
void graphCreateEdge(int u, int v) {
	Node* head = &Graph[u - 1];
	head->next = newNode(v, head->next);
	if (u == v)
		insufficient = 1;
}
void graphDFS() {
	initSearchArrays();
	time = 0;
	for (int i = 0; i < graphVertices; i++) {
		if (color[i] == WHITE)
			graphDFSVisit(i + 1);
	}
	
	printf("Topological Order\n");
	if (!insufficient)
		for (int i = 0; i < graphVertices; i++)
			printf("%d\t", topolSorted[i]);
	printf("\n");
}
void graphDFSVisit(int u) {
	color[u - 1] = GREY;
	disc[u - 1] = ++time;
	for (Node* head = (&Graph[u - 1])->next; head != NULL; head = head->next) {
		if (color[head->index -1] == GREY) {
			insufficient = 1;
			return;
		}
		else if (color[head->index - 1] == WHITE) {
			father[head->index - 1] = u;
			graphDFSVisit(head->index);
		}
	}
	color[u - 1] = BLACK;
	disc[u - 1] = ++time;
	topolSorted[graphVertices - sortedNumber++ - 1] = u;
}

void initSearchArrays() {
	color = malloc(graphVertices * sizeof(int));
	disc = malloc(graphVertices * sizeof(int));
	father = malloc(graphVertices * sizeof(int));
	topolSorted = malloc(graphVertices * sizeof(int));
	for (int i = 0; i < graphVertices; i++) {
		color[i] = WHITE;
		father[i] = -1;
		disc[i] = -1;
	}
}
void graphPrint() {
	for (int i = 0; i < graphVertices; i++)
		listPrint(i + 1, &Graph[i]);
}
void listPrint(int list, Node* head){
	for (Node *temp = head->next; temp != NULL; temp = temp->next)
		printf("Lista %d: %d\n", list, temp->index);
}

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

/*
void allTopo() {
	int flag = 0;
	int res[graphVertices];
	for (int i = 0; i <graphVertices; i++) res[i] = 0;
	int counter = 0;
	for (int i = 0; i < graphVertices; i++) {
		if (indegree[i] == 0 && !visited[i]) {
			//Reduces indegree of adjacent vertices
			Node *tempHead = (&Graph[i])->next;
			for (tempHead; tempHead != NULL; tempHead = tempHead->next)
				indegree[tempHead->index - 1] -= 1;
			//Adds to result
			res[graphVertices - counter -1] = i + 1;
			counter++;
			visited[i] = 1;
			allTopo(res, visited);
			visited[i] = 0;
			res[graphVertices - counter -2] = 0;
			for (tempHead; tempHead != NULL; tempHead = tempHead->next)
				indegree[tempHead->index - 1] += 1;
			flag = 1;
		}
	}
	if (!flag)
		for (int i = 0; i < graphVertices; i++)
			printf(" %d \n", res[i]);
}
*/