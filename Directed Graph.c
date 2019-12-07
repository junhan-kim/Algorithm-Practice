#include <stdio.h>
#include <stdlib.h>

typedef int VertexIndex;
typedef int EdgeIndex;

typedef struct Queue  // 큐 구조체
{
	VertexIndex *queue;  // 큐
	int N; // 큐 크기
	int f; // 큐 시작 인덱스
	int r; // 큐 끝 인덱스
}Queue;

typedef struct List  // 리스트
{
	struct List *next;  // 다음 노드 주소
	EdgeIndex edgeIndex;  // 간선 인덱스
}List;

typedef struct Vertex  // 정점
{
	char name;  // 이름
	List *outEdges;  // 진출 간선
	List *inEdges;  // 진입 간선
	int inDegree;  // 진입 차수
}Vertex;

typedef struct Edge  // 간선
{
	VertexIndex origin;  // 시작 정점
	VertexIndex destination;  // 도착 정점
}Edge;

typedef struct Graph  // 그래프
{
	Vertex *vertices;  // 정점 배열
	Edge *edges;  // 간선 배열
}Graph;

Graph *graph;
int n;  // 정점 갯수
int m;  // 간선 갯수
int *topOrder;  // 위상 순서 배열
Queue *Q;  // 정점 인덱스 큐

void buildGraph();
void insertVertex(char vName, int i);
void insertDirectedEdge(char uName, char wName, int i);
VertexIndex index(char vName);
void insertFirst(List *H, int i);
void topologicalSort();
void initQueue(Queue *Q);
int isEmpty(Queue *Q);
void enqueue(Queue *Q, VertexIndex v);
VertexIndex dequeue(Queue *Q);
void freeList();


int main()
{
	buildGraph();

	topologicalSort();

	if (topOrder[0] == 0)  // DAG가 아닌 경우
	{
		printf("0\n");
	}
	else
	{
		for (int i = 1; i <= n; i++)
		{
			printf("%c ", graph->vertices[topOrder[i]].name);  // 위상 순서대로 알파벳 출력
		}
	}

	free(topOrder);
	freeList();
	free(graph->vertices);
	free(graph->edges);
	free(graph);
    return 0;
}


void buildGraph()
{
	int i = 0;

	graph = (Graph*)malloc(sizeof(Graph));

	// 정점 배열 초기화
	scanf("%d", &n);
	graph->vertices = (Vertex*)malloc(sizeof(Vertex) * n);  

	getchar();
	for (i = 0; i < n; i++)
	{
		char vName = 0;

		vName = getchar();
		insertVertex(vName, i);
		getchar();
	}
	
	// 간선 배열 초기화
	scanf("%d", &m);
	graph->edges = (Edge*)malloc(sizeof(Edge) * m);

	getchar();
	for (i = 0; i < m; i++)
	{
		char uName = 0;
		char wName = 0;

		uName = getchar();
		getchar();
		wName = getchar();
		insertDirectedEdge(uName, wName, i);
		getchar();
	}
}

void insertVertex(char vName, int i)
{
	// 이름 넣고 나머지는 0으로 초기화
	graph->vertices[i].name = vName;
	graph->vertices[i].outEdges = (List*)malloc(sizeof(List));
	graph->vertices[i].outEdges->next = NULL;
	graph->vertices[i].inEdges = (List*)malloc(sizeof(List));
	graph->vertices[i].inEdges->next = NULL;
	graph->vertices[i].inDegree = 0;
}

void insertDirectedEdge(char uName, char wName, int i)
{
	VertexIndex u = 0;
	VertexIndex w = 0;

	u = index(uName);  // 인덱스 값으로 변환
	w = index(wName);

	graph->edges[i].origin = u;  // 간선(인덱스 i)에 각 정점 입력
	graph->edges[i].destination = w;

	// 각 정점의 진출,진입 간선 리스트에 간선 인덱스 i를 가진 노드를 추가
	insertFirst(graph->vertices[u].outEdges, i);  
	insertFirst(graph->vertices[w].inEdges, i);

	(graph->vertices[w].inDegree)++;  // 도착 끝점의 진입 차수 1 증가
}

VertexIndex index(char vName)
{
	int i = 0;

	// 모든 정점을 순회하며 주어진 이름과 매칭되는 정점의 인덱스를 반환
	for (i = 0; i < n; i++)
	{
		if (graph->vertices[i].name == vName)
		{
			return i;
		}
	}
}

void insertFirst(List *H, int i)
{
	List *node = NULL;
	
	node = (List*)malloc(sizeof(List));  // 새 노드 생성하여 i를 입력
	node->edgeIndex = i;
	
	node->next = H->next;  // 헤더 뒷 부분에 노드 삽입
	H->next = node;
}

void topologicalSort()
{
	int *in = NULL;
	int i = 0;
	int t = 0;
	VertexIndex u = 0;

	in = (int*)malloc(sizeof(int) * n);
	Q = (Queue*)malloc(sizeof(Queue));
	initQueue(Q);  // 큐 구조체 초기화

	for (i = 0; i < n; i++)
	{
		in[i] = graph->vertices[i].inDegree;  // 각 정점의 진입 차수를 in(진입 차수 배열)에 받아서
		if (in[i] == 0)  // 0 이면
		{
			enqueue(Q, i);  // 해당 인덱스를 큐에 추가
		}
	}

	t = 1;  // 1로 초기화(0은 DAG 판별용이므로)
	topOrder = (int*)malloc(sizeof(int) * (n + 1));
	while (!isEmpty(Q))  // 큐가 비어있지 않으면
	{
		List *H = NULL;

		u = dequeue(Q);  // 큐에서 인덱스를 dequeue함
		topOrder[t] = u;  // 그 인덱스(u)를 위상 순서 배열에 넣음
		t++;

		H = graph->vertices[u].outEdges->next;
		while (H != NULL)  // u의 모든 진출 간선에 대하여
		{
			VertexIndex w = 0;

			w = graph->edges[H->edgeIndex].destination;  // 각 진출 간선의 도착 정점(w)
			in[w]--;  // 그 간선의 진입 차수를 1 감소
			if (in[w] == 0)  // 진입 차수가 0이 되면
			{
				enqueue(Q, w);  // w를 enqueue
			}
			H = H->next;
		}
	}

	if (t <= n)  // t가 n+1보다 작으면 (방향 싸이클이 존재하는 경우 정지하므로)
	{
		topOrder[0] = 0;  // DAG가 아님
	}
	else  // t가 n+1이면 (마지막 t에서 증가문에 의해 1 증가하였으므로 n+1 임)
	{
		topOrder[0] = 1;  // DAG임
	}

	free(in);
	free(Q->queue);
	free(Q);
}

//큐-----------------------
void initQueue(Queue *Q)
{
	Q->N = n;
	Q->queue = (VertexIndex*)malloc(sizeof(VertexIndex) * Q->N);
	Q->f = 0;
	Q->r = Q->N - 1;
}

int isEmpty(Queue *Q)
{
	return ((Q->r + 1) % Q->N == Q->f);  // r이 f의 한칸 뒤인 경우 empty
}

void enqueue(Queue *Q, VertexIndex v)
{
	Q->r = (Q->r + 1) % Q->N;
	Q->queue[Q->r] = v;
}

VertexIndex dequeue(Queue *Q)
{
	VertexIndex e = 0;
	
	e = Q->queue[Q->f];
	Q->f = (Q->f + 1) % Q->N;
	return e;
}

//free-----------------------
void freeList()
{
	List *p = NULL;
	List *tmp = NULL;
	int i = 0;

	for (i = 0; i < n; i++)  // 모든 정점에 대하여
	{
		p = graph->vertices[i].outEdges;  // 해당 정점의 모든 진출 간선에 대하여
		while (p != NULL)
		{
			tmp = p;  // 계속해서 삭제하며 다음 노드로 이동
			p = p->next;
			free(tmp);
		}

		p = graph->vertices[i].inEdges;  // 해당 정점의 모든 진입 간선에 대하여
		while (p != NULL)
		{
			tmp = p;
			p = p->next;
			free(tmp);
		}
	}
}