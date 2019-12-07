#include <stdio.h>
#include <stdlib.h>

#define NOT_EXIST -1

#define VERTEX1 0
#define VERTEX2 1
#define END_VERTICES 2

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
	EDGE_BACK = -1,
	EDGE_TREE = 0,
	EDGE_FRESH = 1
}EdgeLabel;

typedef struct List
{
	struct List *next;
	EdgeIndex edgeIndex;
}List;

typedef struct Vertex
{
	struct List *head;
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
}Graph;

void init(Graph *graph);
ErrorCode initEdges(Graph *graph);
ErrorCode DFS(Graph *graph, VertexID S);
void freeGraph(Graph *graph);


int main()
{
	Graph *graph = (Graph*)malloc(sizeof(Graph));
	VertexID S = 0;

	init(graph);
	scanf("%d", &S);
	if (initEdges(graph) == NOT_EXIST) return 0;

	if (DFS(graph, S) == NOT_EXIST) return 0;

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
		graph->vertices[i].head = (List*)malloc(sizeof(List));
		graph->vertices[i].head->next = NULL;
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
	List *node = NULL;
	List *p = NULL;

	(graph->edgesNowIndex)++;  // 끝 인덱스 1을 증가시킨 위치에 새로운 간선 지정 후 각 정점 인덱스(v, w) 저장
	graph->edges[graph->edgesNowIndex].vertexIndices[VERTEX1] = v;
	graph->edges[graph->edgesNowIndex].vertexIndices[VERTEX2] = w;

	for (int i = 0; i < END_VERTICES; i++)
	{
		VertexIndex vTmp;

		node = (List*)malloc(sizeof(List));  // 새 노드 생성 후 새로운 간선의 인덱스를 저장
		node->edgeIndex = graph->edgesNowIndex;
		node->next = NULL;
		
		vTmp = graph->edges[graph->edgesNowIndex].vertexIndices[i];  // vTmp = 각 정점
		p = graph->vertices[vTmp].head;  // vTmp의 인접리스트 헤더에 대해

		while ((p->next != NULL) &&  // p의 다음 노드가 NULL이 아니고
			(getVertexID(graph, opposite(graph, vTmp, node->edgeIndex)) > 
				getVertexID(graph, opposite(graph, vTmp, p->next->edgeIndex))))
			// 'vTmp의 새 간선에 대한 반대 끝점의 ID'  >  '각 노드가 가리키는 간선에 대한 반대 끝점의 ID'  인 경우 순회
		{
			p = p->next;
		}
		node->next = p->next;  // p->next 위치에 삽입
		p->next = node;
	}
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


void rDFS(Graph *graph, VertexIndex v)
{
	List *p = NULL;

	graph->vertices[v].label = VERTEX_VISITED;  // 해당 정점(v)을 방문(VISITED)하였음을 표시
	// 특화 부분
	printf("%d\n", getVertexID(graph, v));
	//

	p = graph->vertices[v].head->next;
	while (p != NULL) // 모든 부착간선을 순회
	{
		if (graph->edges[p->edgeIndex].label == EDGE_FRESH)  // 해당 간선을 방문하지 않은 경우(FRESH)
		{
			VertexIndex w;

			w = opposite(graph, v, p->edgeIndex);  // w = v의 반대 끝점
			if (graph->vertices[w].label == VERTEX_FRESH)  // w를 방문하지 않은 경우
			{
				graph->edges[p->edgeIndex].label = EDGE_TREE;  // TREE 표시
				rDFS(graph, w);  // w에 의한 재귀
			}
			else  // w를 이미 방문한 경우
			{
				graph->edges[p->edgeIndex].label = EDGE_BACK;  // BACK 표시
			}
		}
		p = p->next;
	}
}

ErrorCode DFS(Graph *graph, VertexID S)
{
	VertexIndex v;

	v = getVertexIndex(graph, S);
	if (v == NOT_EXIST) return NOT_EXIST;

	for (int i = 0; i < graph->verticesSize; i++)  // 모든 정점에 대해
	{
		if (graph->vertices[i].label == VERTEX_FRESH) rDFS(graph, v);  // 방문되지 않은 정점(FRESH)이라면 rDFS
	}
}


void freeList(Graph *graph)
{
	for (int i = 0; i < graph->verticesSize; i++)
	{
		List *p = graph->vertices[i].head;  // 모든 정점의 인접리스트 헤더에 대해
		while (p != NULL)
		{
			List *tmp = p->next;  // tmp에 다음 노드 저장 후
			free(p);  // p를 free
			p = tmp;  // 다음 노드로 이동
		}
	}
}

void freeGraph(Graph *graph)
{
	freeList(graph);
	free(graph->vertices);
	free(graph->edges);
	free(graph);
}