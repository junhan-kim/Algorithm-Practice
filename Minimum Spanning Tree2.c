#include <stdio.h>
#include <stdlib.h>

#define NOT_EXIST - 1

#define VISITED 0
#define FRESH 1
#define TREE 2
#define BACK 3

typedef int VertexIndex;
typedef int EdgeIndex;
typedef int ID;

typedef struct Sack
{
	VertexIndex v;
	struct Sack *next;
}Sack;

typedef struct Edge
{
	int weight;
	VertexIndex endPoints[2];
	int label;
}Edge;

typedef struct Vertex
{
	int id;
	int label;
}Vertex;

typedef struct Graph
{
	Vertex *vertices;
	Edge *edges;
	int verticesSize;
	int edgesSize;
	int edgeNowIndex;
	int **AdjacencyMatrix;
}Graph;

typedef struct QueueStruct
{
	int key; // 간선의 무게
	EdgeIndex elem; // 간선
}QueueStruct;

typedef struct Queue
{
	QueueStruct *Q;
	int f;
	int r;
	int n;
}Queue;

void kruskalMST(Graph *graph);
int dfs(Graph *graph, VertexIndex v);


void init(Graph *graph)
{
	graph->vertices = (Vertex*)malloc(sizeof(Vertex)*graph->verticesSize);
	graph->edges = (Edge*)malloc(sizeof(Edge)*graph->edgesSize);

	for (int i = 0; i < graph->verticesSize; i++)
	{
		graph->vertices[i].id = i + 1; // id는 1~N
		graph->vertices[i].label = FRESH;
	}

	for (int i = 0; i < graph->edgesSize; i++)
	{
		graph->edges[i].weight = 0;
		graph->edges[i].endPoints[0] = NOT_EXIST;
		graph->edges[i].endPoints[1] = NOT_EXIST;
		graph->edges[i].label = FRESH;
	}

	//인접행렬 초기화
	graph->AdjacencyMatrix = (int**)malloc(sizeof(int*)*graph->verticesSize);
	for (int i = 0; i < graph->verticesSize; i++)
	{
		graph->AdjacencyMatrix[i] = (int*)malloc(sizeof(int)*graph->verticesSize);
	}

	for (int i = 0; i < graph->verticesSize; i++)
	{
		for (int j = 0; j < graph->verticesSize; j++)
		{
			graph->AdjacencyMatrix[i][j] = NOT_EXIST;
		}
	}

	graph->edgeNowIndex = NOT_EXIST;
}

void insertEdge(Graph *graph, ID a, ID b, int weight)
{
	graph->edgeNowIndex++; // 인덱스 1 증가시켜 간선 추가
	graph->edges[graph->edgeNowIndex].endPoints[0] = a - 1; // 따로 id->index 변환함수 안넣음. 어차피 id - 1 = 인덱스라서
	graph->edges[graph->edgeNowIndex].endPoints[1] = b - 1;
	graph->edges[graph->edgeNowIndex].weight = weight;

	graph->AdjacencyMatrix[a - 1][b - 1] = graph->edgeNowIndex; // 인접행렬에 간선 인덱스 넣어줌
	graph->AdjacencyMatrix[b - 1][a - 1] = graph->edgeNowIndex;
}




// main -------------------------------------------
int main()
{
	Graph *graph = (Graph*)malloc(sizeof(Graph));

	int N = 0;
	int M = 0;

	scanf("%d %d", &N, &M);
	graph->verticesSize = N;
	graph->edgesSize = M;

	init(graph);
	for (int i = 0; i < graph->edgesSize; i++)
	{
		int a = 0, b = 0, weight = 0;
		scanf("%d", &a);
		scanf("%d", &b);
		scanf("%d", &weight);
		insertEdge(graph, a, b, weight);
	}

	kruskalMST(graph);

	int result = dfs(graph, 0);
	printf("\n%d", result);

	//free
	for (int i = 0; i < graph->verticesSize; i++)
	{
		free(graph->AdjacencyMatrix[i]);
	}
	free(graph->AdjacencyMatrix);
	free(graph->vertices);
	free(graph->edges);
	free(graph);

	return 0;
}


// 큐 구조체 전용 선택 정렬
// 큐라서 f부터 r까지 가면서 n기준으로 wrap around
void selectionSort(QueueStruct *A, Queue *queue)
{
	for (int pass = queue->f; pass < queue->r; pass = (pass + 1) % queue->n)
	{
		int minLoc = pass;
		for (int j = pass + 1; j <= queue->r; j = (j + 1) % queue->n)
		{
			if (A[minLoc].key > A[j].key) minLoc = j;  // 키 값 기준으로 오름차순 정렬
		}
		QueueStruct tmp = A[minLoc];
		A[minLoc] = A[pass];
		A[pass] = tmp;
	}
}

void enqueue(Queue *queue, int key, int elem)
{
	queue->r = (queue->r + 1) % queue->n;
	queue->Q[queue->r].key = key;
	queue->Q[queue->r].elem = elem;
}

void QueueInit(Queue *queue, Graph *graph)
{
	queue->n = graph->edgesSize + 1;
	queue->Q = (QueueStruct*)malloc(sizeof(QueueStruct)*queue->n);
	queue->f = 0;
	queue->r = queue->n - 1;

	for (int i = 0; i < graph->edgesSize; i++)
	{
		enqueue(queue, graph->edges[i].weight, i);
		// 키 = 간선의 무게, 원소 = 간선 인덱스
	}

	selectionSort(queue->Q, queue);  // 넣고 한번 정렬
}


// removeMin = dequeue
// init하고 replaceKey 실행할때마다 정렬되므로 
// 결과적으로 dequeue하면 키가 제일 작은 값이 나옴
int removeMin(Queue *queue)
{
	int tmp = queue->Q[queue->f].elem;
	queue->f = (queue->f + 1) % queue->n;
	return tmp;
}


int getSackIndex(Graph *graph, Sack **sack, VertexIndex v) // sack index 반환
{
	for (int i = 0; i < graph->verticesSize; i++)
	{
		Sack *p = sack[i]->next;
		while (p != NULL) // 배낭이 비어있지 않으면
		{
			if (p->v == v) return i; // 배낭 내의 정점 인덱스와 입력된 정점 인덱스와 비교하여 같으면 
			                         // 그 배낭의 인덱스를 반환
			p = p->next;
		}
	}
}

void MergeSack(Sack **sack, int uSackIndex, int vSackIndex)
{
	Sack *p = sack[uSackIndex]->next;
	
	while (p->next != NULL)
	{
		p = p->next;
	}
	p->next = sack[vSackIndex]->next; // 한 배낭의 맨 끝 노드의 next에 다른 배낭의 첫 노드를 연결
	sack[vSackIndex]->next = NULL; // 다른 배낭은 비어있게 됨
}

void freeSack(Graph *graph, Sack **sack)
{
	for (int i = 0; i < graph->verticesSize; i++) // 전체 배낭에 대해
	{
		Sack *p = sack[i];
		while (p->next != NULL) // 배낭 내용물 비우기
		{
			Sack *tmp = p->next;
			free(p);
			p = tmp;
		}
		if(p!=NULL) free(p); // 각 배낭 없애기
	}
	free(sack);
}


void kruskalMST(Graph *graph)
{
	Sack **sack = (Sack**)malloc(sizeof(Sack*)*graph->verticesSize);
	for (int i = 0; i < graph->verticesSize; i++) // 정점 갯수 = 배낭 갯수(비어있는 배낭포함)
	{
		sack[i] = (Sack*)malloc(sizeof(Sack)); // 헤더
		sack[i]->next = (Sack*)malloc(sizeof(Sack)); // 각각의 배낭 초기화
		sack[i]->next->v = i;
		sack[i]->next->next = NULL;
	}

	Edge *edges = (Edge*)malloc(sizeof(Edge)*graph->verticesSize - 1); // 최소신장트리를 만들기위한 임시 간선배열
																	   // 나중에 여기 모은 간선들로 트리를 새로 만듬
	EdgeIndex edgeNowIndex = NOT_EXIST;

	Queue *queue = (Queue*)malloc(sizeof(Queue));
	QueueInit(queue, graph);

	while (edgeNowIndex + 1 < (graph->verticesSize - 1)) // 큐가 안 비어있으면
	{
		EdgeIndex e = removeMin(queue); // 큐(배낭 밖)에서 가장 작은 간선 무게를 가진 간선을 가져옴

		VertexIndex u = graph->edges[e].endPoints[0]; // 간선의 양 끝점을 저장
		VertexIndex v = graph->edges[e].endPoints[1];

		int uSackIndex = getSackIndex(graph, sack, u); // 각 끝점의 배낭 인덱스
		int vSackIndex = getSackIndex(graph, sack, v);
		if (uSackIndex != vSackIndex) // 배낭 인덱스가 서로 다른 경우 = 다른 배낭인 경우
		{
			printf(" %d", graph->edges[e].weight);

			edgeNowIndex++; // 그 간선의 정보를 가지고 edges에 복사
			edges[edgeNowIndex].endPoints[0] = graph->edges[e].endPoints[0];
			edges[edgeNowIndex].endPoints[1] = graph->edges[e].endPoints[1];
			edges[edgeNowIndex].weight = graph->edges[e].weight;

			MergeSack(sack, uSackIndex, vSackIndex); // 두 배낭을 합침
		}	
	}

	init(graph); // 기존 그래프를 지움
	edgeNowIndex = NOT_EXIST;
	graph->edgesSize = graph->verticesSize - 1;
	for (int i = 0; i < graph->edgesSize; i++) // edges의 정보를 가지고 최소신장트리를 그림
	{
		edgeNowIndex++;
		insertEdge(graph, edges[edgeNowIndex].endPoints[0] + 1, edges[edgeNowIndex].endPoints[1] + 1, edges[edgeNowIndex].weight);
	}

	freeSack(graph, sack);
	free(edges);
	free(queue->Q);
	free(queue);
}


int dfs(Graph *graph, VertexIndex v)
{
	static int result = 0;

	graph->vertices[v].label = VISITED; // 방문으로 표시

	for (int i = 0; i < graph->verticesSize; i++) // 모든 정점들에 대해
	{
		if (graph->AdjacencyMatrix[v][i] != NOT_EXIST) // 간선이 존재하면 (부착 간선)
		{
			EdgeIndex e = graph->AdjacencyMatrix[v][i]; // 그 간선을 e라 함

			if (graph->edges[e].label == FRESH) // e가 FRESH이면
			{
				VertexIndex w = i; // e에 대한 반대 끝점을 w라 함
				if (graph->vertices[w].label == FRESH) // w가 FRESH이면 
				{
					graph->edges[e].label = TREE; // e는 TREE
					
					result += graph->edges[e].weight; // 간선들의 무게를 다 더함 = 최소 비용
					dfs(graph, w); // 반대끝점을 통하여 재귀
				}
				else // e가 FRESH가 아니면
				{
					graph->edges[e].label = BACK; // e는 BACK
				}
			}
		}
	}
	return result;
}

