#include <stdio.h>
#include <stdlib.h>

#define NOT_EXIST - 1

typedef int VertexIndex;
typedef int EdgeIndex;
typedef int ID;

typedef struct Edge
{
	int weight;
	VertexIndex orig; // 원점
	VertexIndex dest; // 도착점
}Edge;

typedef struct Vertex
{
	int id;
	int d;
	// 간선리스트 구조이므로 인접리스트가 존재하지 않는다.
}Vertex;

typedef struct Graph
{
	Vertex *vertices;
	Edge *edges;
	int verticesSize;
	int edgesSize;
	int edgeNowIndex;
	int startIndex;
}Graph;

void BellmanFordShortestPaths(Graph *graph);


extern inline int getMaxValue()
{
	return 50 * 1000; // INT_MAX를 최대값으로 하게 되면 더 했을때 오버플로우가 생기기 때문에
	                  // 최대 가중치 * 최대 간선수 * 2 보다 충분히 큰 값을 최대값으로 하였음.
	                  // 2를 곱한 이유는 가이드라인에 음의 가중치에 대한 제한이 정확히 나와있지 않았지만
	                  // 유추해보았을때 아마 양의 가중치 제한인 20의 음의 값인 -20이라고 예상해볼 수 있고
	                  // 그렇다고 가정하면 최대값 - 20 * 1000이 예외 상황에서 나올 수 있는 최대 값이기 때문에
	                  // 거기에 여유분을 둔 50 * 1000을 최대값으로 하였음.
	                  // 올바른 상황에서 나올 수 있는 최대값이 20 * 1000 = 20000 이기 때문에 
	                  // 예외 상황에서 나오는 50 * 1000 - 20 * 1000 = 30000 과 10000의 여유를 두고 겹치지 않음
}

void init(Graph *graph)
{
	int N = 0;
	int M = 0;
	int S = 0;

	scanf("%d %d %d", &N, &M, &S);

	graph->verticesSize = N;
	graph->edgesSize = M;
	graph->startIndex = S - 1;

	graph->vertices = (Vertex*)malloc(sizeof(Vertex)*N);
	graph->edges = (Edge*)malloc(sizeof(Edge)*M);

	for (int i = 0; i < graph->verticesSize; i++)
	{
		graph->vertices[i].id = i + 1; // id는 1~N
		graph->vertices[i].d = getMaxValue(); // 무한대 = int 최대값
	}

	for (int i = 0; i < graph->edgesSize; i++)
	{
		graph->edges[i].weight = 0;
		graph->edges[i].orig = NOT_EXIST;
		graph->edges[i].dest = NOT_EXIST;
	}

	graph->edgeNowIndex = NOT_EXIST;
}

void insertEdge(Graph *graph, ID a, ID b, int weight) // a=원점, b=도착점
{
	graph->edgeNowIndex++; // 인덱스 1 증가시켜 간선 추가
	graph->edges[graph->edgeNowIndex].orig = a - 1; // 따로 id->index 변환함수 안넣음. 어차피 id - 1 = 인덱스라서
	graph->edges[graph->edgeNowIndex].dest = b - 1;
	graph->edges[graph->edgeNowIndex].weight = weight;
}


// main -------------------------------------------
int main()
{
	Graph *graph = (Graph*)malloc(sizeof(Graph));

	init(graph);
	for (int i = 0; i < graph->edgesSize; i++)
	{
		int a = 0, b = 0, weight = 0;
		scanf("%d %d %d", &a, &b, &weight);
		insertEdge(graph, a, b, weight);
	}

	BellmanFordShortestPaths(graph);

	for (int i = 0; i < graph->verticesSize; i++) // 정점을 오름차순으로 순회하며
	{
		if ((i != graph->startIndex) && (graph->vertices[i].d < 20000))
			// 자기 자신은 제외, d가 최대값(20 * 1000) 보다 작은 경우에만
		{
			printf("%d %d\n", i + 1, graph->vertices[i].d);
		}
	}

	free(graph->vertices);
	free(graph->edges);
	free(graph);

	return 0;
}


int min(int a, int b)
{
	if (a < b) return a;
	else return b;
}

void BellmanFordShortestPaths(Graph *graph)
{
	int s = graph->startIndex; // 입력받은 시작 인덱스
	graph->vertices[s].d = 0; // 시작인덱스 d를 0으로 하여, 시작인덱스부터 시작하게끔

	for (int i = 1; i < graph->verticesSize; i++) 
		// 이 알고리즘은 i라운드마다 시작점으로부터 i개의 간선을 사용하는 최단경로를 찾는다.
		// 즉, n-1번(간선 갯수)이상의 간선을 사용할 수 없으므로 n-1번만 수행
	{
		for (EdgeIndex e = 0; e < graph->edgesSize; e++) // 모든 간선에 대해
		{
			VertexIndex u = graph->edges[e].orig;
			VertexIndex z = graph->edges[e].dest;
			graph->vertices[z].d = min(graph->vertices[z].d, graph->vertices[u].d + graph->edges[e].weight);
			// 원점의 거리와 간선 무게 + 도착점 거리 중 최소인 값을 도착점의 거리로 갱신.
		}
	}
}