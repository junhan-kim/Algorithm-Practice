#include <stdio.h>
#include <stdlib.h>

#define NOT_EXIST -1

#define VERTEX1 0
#define VERTEX2 1
#define END_VERTICES 2

#define TRUE 1
#define FALSE 0

typedef int VertexID;
typedef int VertexIndex;
typedef int EdgeIndex;
typedef int ErrorCode;

typedef enum VertexLabel
{
	VERTEX_VISITED = 0,
	VERTEX_FRESH = 1
}VertexLabel;

typedef enum EdgeLabel
{
	EDGE_CROSS = -1,
	EDGE_TREE = 0,
	EDGE_FRESH = 1
}EdgeLabel;


typedef struct List
{
	struct List *next;
	VertexIndex v;
}List;

typedef struct Vertex
{
	int id;
	VertexLabel label;
}Vertex;

typedef struct Edge
{
	VertexIndex vertexIndices[2];
	EdgeLabel label;
}Edge;

typedef struct Graph
{
	Vertex *vertices;
	Edge *edges;
	int verticesSize;  // 정점 배열의 전체 크기
	int edgesSize;  // 간선 배열의 전체 크기
	int edgesNowIndex;  // 간선 배열의 끝 인덱스
	EdgeIndex **adjacencyMatrix;  // 인접 행렬
}Graph;

void init(Graph *graph);
ErrorCode initEdges(Graph *graph);
ErrorCode BFS(Graph *graph, VertexID S);
void freeGraph(Graph *graph);


int main()
{
	Graph *graph = (Graph*)malloc(sizeof(Graph));
	VertexID S = 0;

	init(graph);
	scanf("%d", &S);
	if (initEdges(graph) == NOT_EXIST) return 0;

	if (BFS(graph, S) == NOT_EXIST) return 0;

	freeGraph(graph);  // free
	return 0;
}


// 초기화
void init(Graph *graph)
{
	scanf("%d", &(graph->verticesSize));
	graph->vertices = (Vertex*)malloc(sizeof(Vertex)*graph->verticesSize);
	for (int i = 0; i < graph->verticesSize; i++)
	{
		graph->vertices[i].id = i + 1;  // id를 1 ~ verticesSize로 지정
		graph->vertices[i].label = VERTEX_FRESH;
	}

	scanf("%d", &(graph->edgesSize));
	graph->edges = (Edge*)malloc(sizeof(Edge)*graph->edgesSize);
	graph->edgesNowIndex = NOT_EXIST;
	for (int i = 0; i < graph->edgesSize; i++)
	{
		for (int j = 0; j < END_VERTICES; j++)
		{
			graph->edges[i].vertexIndices[j] = NOT_EXIST;
		}
		graph->edges[i].label = EDGE_FRESH;
	}

	// verticesSize * verticesSize 크기의 인접행렬 생성
	graph->adjacencyMatrix = (EdgeIndex**)malloc(sizeof(EdgeIndex*)*graph->verticesSize);  
	for (int i = 0; i < graph->verticesSize; i++)
	{
		graph->adjacencyMatrix[i] = (EdgeIndex*)malloc(sizeof(EdgeIndex)*graph->verticesSize);
	}
	for (int i = 0; i < graph->verticesSize; i++)
	{
		for (int j = 0; j < graph->verticesSize; j++)
		{
			graph->adjacencyMatrix[i][j] = NOT_EXIST;
		}
	}
}

// 인덱스 -> id 변환
extern inline VertexIndex getVertexIndex(Graph *graph, VertexID id)
{
	if ((id < 1) || (id > graph->verticesSize)) return NOT_EXIST;
	else return id - 1;
}

// id -> 인덱스 변환
extern inline VertexID getVertexID(Graph *graph, VertexIndex v)
{
	if ((v < 0) || (v > graph->verticesSize - 1)) return NOT_EXIST;
	else return v + 1;
}

// 정점 인덱스와 다른 값을 가진 인덱스(반대쪽 인덱스) 반환
VertexIndex opposite(Graph *graph, VertexIndex v, EdgeIndex e)
{
	for (int i = 0; i < END_VERTICES; i++)
	{
		if (graph->edges[e].vertexIndices[i] != v)
		{
			return graph->edges[e].vertexIndices[i];
		}
	}
}

void insertEdge(Graph *graph, VertexIndex v, VertexIndex w)
{
	(graph->edgesNowIndex)++;  // 끝 인덱스 1을 증가시킨 위치에 새로운 간선 지정 후 각 정점 인덱스(v, w) 저장
	graph->edges[graph->edgesNowIndex].vertexIndices[VERTEX1] = v;
	graph->edges[graph->edgesNowIndex].vertexIndices[VERTEX2] = w;

	graph->adjacencyMatrix[v][w] = graph->edgesNowIndex;  // 인접행렬에 간선 인덱스 저장
	graph->adjacencyMatrix[w][v] = graph->edgesNowIndex;
}

// 간선 초기화
ErrorCode initEdges(Graph *graph)
{
	VertexID id1 = NOT_EXIST;
	VertexID id2 = NOT_EXIST;
	VertexIndex v = NOT_EXIST;
	VertexIndex w = NOT_EXIST;

	for (int i = 0; i < graph->edgesSize; i++)
	{
		scanf("%d %d", &id1, &id2);
		v = getVertexIndex(graph, id1);
		if (v == NOT_EXIST) return NOT_EXIST;
		w = getVertexIndex(graph, id2);
		if (w == NOT_EXIST) return NOT_EXIST;

		insertEdge(graph, v, w);
	}
}


// 리스트가 비었는가
int isEmpty(List *p)
{
	if (p->next == NULL) return TRUE;
	else return FALSE;
}

void addLast(List *p, VertexIndex v)
{
	while (p->next != NULL)  // p의 다음 노드가 NULL이 아닐 때까지 순회
	{
		p = p->next;
	}
	p->next = (List*)malloc(sizeof(List));  // 다음 노드에 새로운 노드 만듬
	p->next->v = v;
	p->next->next = NULL;
}

void freeList(List *p)
{
	while (p != NULL)
	{
		List *tmp = p->next;  // tmp에 다음 노드 저장 후
		free(p);  // p를 free
		p = tmp;  // 다음 노드로 이동
	}
}

void rBFS(Graph *graph, VertexIndex v)
{
	List **L = (List**)malloc(sizeof(List*)*graph->verticesSize);  // 정점 갯수만큼 리스트 배열 생성
	for (int i = 0; i < graph->verticesSize; i++)
	{
		L[i] = (List*)malloc(sizeof(List)); // 헤더 생성
		L[i]->v = NOT_EXIST;
		L[i]->next = NULL;
	}

	addLast(L[0], v);  // 처음 리스트에 시작 위치 인덱스 추가

	graph->vertices[v].label = VERTEX_VISITED;  // 해당 정점(v)을 방문(VISITED)하였음을 표시
	//특화 부분
	printf("%d\n", getVertexID(graph, v));
	//

	int i = 0;
	while (!isEmpty(L[i]))  // 해당 레벨의 리스트가 비어있지 않다면
	{
		List *p = L[i]->next;  // 헤더 다음 노드부터 시작
		while (p != NULL)  // 그 노드가 NULL이 아니라면
		{
			v = p->v;  // 그 노드의 정점 인덱스를 취함
			for (int k = 0; k < graph->verticesSize; k++)  // 그 인덱스와 모든 정점 인덱스 중
			{
				if (graph->adjacencyMatrix[v][k] != NOT_EXIST)  // 간선이 연결되어있는 경우
				{
					EdgeIndex edgeIndex = graph->adjacencyMatrix[v][k];  // 그 간선 인덱스를 취함
					if (graph->edges[edgeIndex].label == EDGE_FRESH)  // 해당 간선을 방문하지 않은 경우(FRESH)
					{
						VertexIndex w;

						w = opposite(graph, v, edgeIndex);  // w = v의 반대 끝점
						if (graph->vertices[w].label == VERTEX_FRESH)  // w를 방문하지 않은 경우
						{
							graph->edges[edgeIndex].label = EDGE_TREE;  // TREE 표시
							graph->vertices[w].label = VERTEX_VISITED;  // w에는 VISITED 표시
							// 특화 부분
							printf("%d\n", getVertexID(graph, w));
							//
							addLast(L[i + 1], w);  // 다음 레벨 리스트에 w를 추가
						}
						else  // w를 이미 방문한 경우
						{
							graph->edges[edgeIndex].label = EDGE_CROSS;  // CROSS 표시
						}
					}
				}
			}
			p = p->next;  // 계속하여 해당 레벨 리스트의 다음 노드로 이동
		}
		freeList(L[i]);  // 해당 레벨 리스트를 free
		i++;  // 계속하여 다음 레벨 리스트로 이동
	}
	free(L);
}

ErrorCode BFS(Graph *graph, VertexID S)
{
	VertexIndex v;

	v = getVertexIndex(graph, S);
	if (v == NOT_EXIST) return NOT_EXIST;

	for (int i = 0; i < graph->verticesSize; i++)  // 모든 정점에 대해
	{
		if (graph->vertices[i].label == VERTEX_FRESH) rBFS(graph, v);  // 방문되지 않은 정점(FRESH)이라면 rBFS
	}
}


void freeAdjacencyMatrix(Graph *graph)  // 인접행렬 free
{
	for (int i = 0; i < graph->verticesSize; i++)
	{
		free(graph->adjacencyMatrix[i]);
	}	
}

void freeGraph(Graph *graph)
{
	freeAdjacencyMatrix(graph);
	free(graph->adjacencyMatrix);
	free(graph->vertices);
	free(graph->edges);
	free(graph);
}