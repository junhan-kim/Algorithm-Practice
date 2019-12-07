#include <stdio.h>
#include <stdlib.h>

#define NOT_EXIST - 1
#define INT_MAX 2147483647

typedef int VertexIndex;
typedef int EdgeIndex;
typedef int ID;

typedef struct Edge
{
	int weight;
	VertexIndex endPoints[2];
}Edge;

typedef struct Vertex
{
	int id;
	int d;
	EdgeIndex p;
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
	int key;
	VertexIndex elem;
}QueueStruct;

typedef struct Queue
{
	QueueStruct *Q;
	int f;
	int r;
	int n;
}Queue;

void Prim_JarnikMST(Graph *graph);


void init(Graph *graph)
{
	int N = 0;
	int M = 0;

	scanf("%d %d", &N, &M);

	graph->verticesSize = N;
	graph->edgesSize = M;

	graph->vertices = (Vertex*)malloc(sizeof(Vertex)*N);
	graph->edges = (Edge*)malloc(sizeof(Edge)*M);

	for (int i = 0; i < graph->verticesSize; i++)
	{
		graph->vertices[i].id = i + 1; // id는 1~N
		graph->vertices[i].d = INT_MAX; // 무한대 = int 최대값
		graph->vertices[i].p = NOT_EXIST;
	}

	for (int i = 0; i < graph->edgesSize; i++)
	{
		graph->edges[i].weight = 0;
		graph->edges[i].endPoints[0] = NOT_EXIST;
		graph->edges[i].endPoints[1] = NOT_EXIST;
	}

	//인접행렬 초기화
	graph->AdjacencyMatrix = (int**)malloc(sizeof(int*)*N);
	for (int i = 0; i < N; i++)
	{
		graph->AdjacencyMatrix[i] = (int*)malloc(sizeof(int)*N);
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
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
	
	init(graph);
	for (int i = 0; i < graph->edgesSize; i++)
	{
		int a = 0, b = 0, weight = 0;
		scanf("%d", &a);
		scanf("%d", &b);
		scanf("%d", &weight);
		insertEdge(graph, a, b, weight);
	}

	Prim_JarnikMST(graph);

	int result = 0;
	for (int i = 0; i < graph->verticesSize; i++) // 모든 정점에 대해
	{
		EdgeIndex p = graph->vertices[i].p; // 정점의 부모 간선 인덱스가
		if (p != NOT_EXIST) // 존재하면
		{
			result += graph->edges[p].weight; // 다 더함 = 최소 비용
		}
	}
	printf("\n%d", result);

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
	for (int pass = queue->f; pass < queue->r; pass=(pass+1)%queue->n)
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
	queue->n = graph->verticesSize + 1;
	queue->Q = (QueueStruct*)malloc(sizeof(QueueStruct)*queue->n);
	queue->f = 0;
	queue->r = queue->n - 1;

	for (int i = 0; i < graph->verticesSize; i++)
	{
		enqueue(queue, graph->vertices[i].d, graph->vertices[i].id - 1);
		// 원소 = 정점 인덱스,  키 = 거리
	}

	selectionSort(queue->Q, queue);  // 넣고 한번 정렬
}

int isEmpty(Queue *queue)
{
	return ((queue->r + 1) % queue->n) == queue->f;  // r이 f 한칸 뒤면 비어있음
}

// removeMin = dequeue
// init하고 replaceKey 실행할때마다 정렬되므로 
// 결과적으로 dequeue하면 d가 제일 작은 값이 나옴
int removeMin(Queue *queue)  
{
	int tmp = queue->Q[queue->f].elem;
	queue->f = (queue->f + 1) % queue->n;
	return tmp;
}

void replaceKey(Queue *queue, int e, int k)
{
	for (int i = queue->f; i <= queue->r; i = (i + 1) % queue->n) // 큐 안에
	{
		if (queue->Q[i].elem == e) // 해당 원소에 대한
		{
			queue->Q[i].key = k; // 키를 변환
		}
	}
	selectionSort(queue->Q, queue); // 위치 갱신
}

int isQueueElem(Queue *queue, VertexIndex e)
{
	for (int i = queue->f; i <= queue->r; i = (i+1)%queue->n) // 큐 안에
	{
		if (queue->Q[i].elem == e) return 1; // 원소가 있나?
	}
	return 0;
}


void Prim_JarnikMST(Graph *graph)
{
	int s = 0; // 0번 인덱스 = id 1인 정점의 인덱스
	graph->vertices[s].d = 0; // 정점 1의 d를 0으로 하여, 1부터 시작하게끔

	Queue *queue = (Queue*)malloc(sizeof(Queue));
	QueueInit(queue, graph);

	while (!isEmpty(queue)) // 큐가 안 비어있으면
	{
		VertexIndex u = removeMin(queue); // 큐(배낭 밖)에서 가장 작은 d(거리)를 가진 정점을 가져옴
		printf(" %d", u + 1);
		for (int i = 0; i < graph->verticesSize; i++) 
		{			
			if (graph->AdjacencyMatrix[u][i] != NOT_EXIST) // 그 정점과 i 정점과의 간선이 존재하면
			{
				EdgeIndex e = graph->AdjacencyMatrix[u][i]; // 그 간선을 e라 하고
				VertexIndex z = i; // 반대 정점을 z라 함
				if (isQueueElem(queue, z) && (graph->edges[e].weight < graph->vertices[z].d))
					// z가 큐에 속해있으며, e의 무게가 z의 d값보다 작으면
				{
					graph->vertices[z].d = graph->edges[e].weight; // z의 거리를 e의 무게로서 갱신함
					graph->vertices[z].p = e; // z의 부모를 e라 함
					replaceKey(queue, z, graph->edges[e].weight); // 큐에서도 원소 z의 키(거리)를 e의 무게로 갱신함
				}
			}
		}
	}

	free(queue->Q);
	free(queue);
}