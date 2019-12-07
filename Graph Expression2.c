#include <stdio.h>
#include <stdlib.h>

#define NOT_EXIST -1
#define REMOVED -2

typedef int VertexIndex;
typedef int EdgeIndex;

typedef struct Vertex
{
	int id;
}Vertex;

typedef struct Edge
{
	int vertexIndices[2];
	int weight;
}Edge;

typedef struct Graph
{
	Vertex *vertices; // 정점 배열
	Edge *edges; // 간선 배열
	int verticesSize; // 현재 정점 갯수
	int edgesSize; // 현재 간선 갯수
	int **adjacencyMatrix;
}Graph;

typedef struct A  // runA 임시 저장용 구조체
{
	int id;
	int weight;
}A;

void init(Graph *graph);
void insertEdge(Graph *graph, VertexIndex a, VertexIndex b, int o);
void init_insertEdge(Graph *graph);
int runA(Graph *graph, int nodeNumber, A **a, int *size);
int runM(Graph *graph, int a, int b, int weight);
void freeAdjacencyMatrix(Graph *graph);

int main()
{
	Graph *graph = (Graph*)malloc(sizeof(Graph));
	A *arr = NULL;

	init(graph);

	int q = 0;
	while (!q)
	{
		char cInput = 0;
		int nInput = 0;
		int a = 0;
		int b = 0;
		int w = 0;
		int size = 0;
		int bRun = 0;
		scanf("%c", &cInput);
		switch (cInput)
		{
		case 'a':
			scanf("%d", &nInput);
			if (runA(graph, nInput, &arr, &size) == NOT_EXIST) printf("-1\n");
			else
			{
				for (int i = 0; i < size; i++)
				{
					printf(" %d %d", arr[i].id, arr[i].weight);
				}
				printf("\n");
			}
			break;
		case 'm':
			scanf("%d %d %d", &a, &b, &w);
			bRun = runM(graph, a, b, w);
			if (bRun == NOT_EXIST) printf("-1\n");
			if (bRun == REMOVED) printf("\n");
			break;
		case 'q':
			q = 1;
			break;
		}
	}

	free(arr);
	freeAdjacencyMatrix(graph);
	free(graph->vertices);
	free(graph->edges);
	free(graph);
	return 0;
}


void freeAdjacencyMatrix(Graph *graph)
{
	for (int i = 0; i < graph->verticesSize; i++)
	{
		free(graph->adjacencyMatrix[i]);
	}
}

int getEdgesSize(int n)
{
	if (n == 1) return 1; // 정점이 0인 경우는 없으므로 베이스조건 : n = 1
	else return n + getEdgesSize(n - 1);
}

void init(Graph *graph)
{
	const int verticesSize = 6;
	const int edgesSize = getEdgesSize(verticesSize);

	graph->vertices = (Vertex*)malloc(sizeof(Vertex) * verticesSize);
	graph->verticesSize = verticesSize;
	for (int i = 0; i < verticesSize; i++) // 정점을 모두 추가
	{
		graph->vertices[i].id = i + 1; // 1~verticesSize 의 id 부여
	}

	graph->edges = (Edge*)malloc(sizeof(Edge) * edgesSize);
	graph->edgesSize = 0; // 간선은 아직 추가되지 않음
	for (int i = 0; i < edgesSize; i++) // 간선 배열 초기화
	{
		for (int j = 0; j < 2; j++)
		{
			graph->edges[i].vertexIndices[j] = NOT_EXIST;
		}
		graph->edges[i].weight = 0;
	}

	graph->adjacencyMatrix = (int**)malloc(sizeof(int*)*graph->verticesSize); // 인접행렬 생성 (정점 갯수 x 정점 갯수)
	for (int i = 0; i < graph->verticesSize; i++)
	{
		graph->adjacencyMatrix[i] = (int*)malloc(sizeof(int)*graph->verticesSize);
	}
	for (int i = 0; i < graph->verticesSize; i++)  // 초기화
	{
		for (int j = 0; j < graph->verticesSize; j++)
		{
			graph->adjacencyMatrix[i][j] = NOT_EXIST;
		}
	}

	init_insertEdge(graph);
}

VertexIndex getVertexIndex(Graph *graph, int a)
{
	for (int i = 0; i < graph->verticesSize; i++) // a와 b의 정점 id를 이용하여 정점 index를 찾는다
	{
		if (graph->vertices[i].id == a) return i;
	}
	return NOT_EXIST;
}

void insertEdge(Graph *graph, VertexIndex v, VertexIndex w, int weight) // 간선 추가
{
	int edgeIndex = graph->edgesSize;
	Edge *edge = &(graph->edges[edgeIndex]);  // 간선 배열 맨 뒤에, 새 간선 추가

	edge->vertexIndices[0] = v;
	edge->vertexIndices[1] = w;
	edge->weight = weight;
	(graph->edgesSize)++;
   
	// 인접행렬에 간선 인덱스를 저장
	graph->adjacencyMatrix[edge->vertexIndices[0]][edge->vertexIndices[1]] = edgeIndex;
	graph->adjacencyMatrix[edge->vertexIndices[1]][edge->vertexIndices[0]] = edgeIndex;
}

void init_insertEdge(Graph *graph) // 간선들의 초기값을 지정
{
	insertEdge(graph, getVertexIndex(graph, 1), getVertexIndex(graph, 2), 1);
	insertEdge(graph, getVertexIndex(graph, 1), getVertexIndex(graph, 3), 1);
	insertEdge(graph, getVertexIndex(graph, 1), getVertexIndex(graph, 4), 1);
	insertEdge(graph, getVertexIndex(graph, 1), getVertexIndex(graph, 6), 2);
	insertEdge(graph, getVertexIndex(graph, 2), getVertexIndex(graph, 3), 1);
	insertEdge(graph, getVertexIndex(graph, 3), getVertexIndex(graph, 5), 4);
	insertEdge(graph, getVertexIndex(graph, 5), getVertexIndex(graph, 5), 4);
	insertEdge(graph, getVertexIndex(graph, 5), getVertexIndex(graph, 6), 3);
}

void removeEdge(Graph *graph, EdgeIndex edgeIndex)
{
	Edge *e = &(graph->edges[edgeIndex]);  // 지워야할 간선
	EdgeIndex lastEdgeIndex = (graph->edgesSize) - 1;
	Edge *lastEdge = &(graph->edges[lastEdgeIndex]);   // 간선 배열의 맨 마지막 간선

	// 간선의 양끝점을 인접행렬에서 제거
	graph->adjacencyMatrix[e->vertexIndices[0]][e->vertexIndices[1]] = NOT_EXIST;
	graph->adjacencyMatrix[e->vertexIndices[1]][e->vertexIndices[0]] = NOT_EXIST;

	for (int i = 0; i < 2; i++) // 간선 배열의 맨 마지막 값들을 삭제할 e 위치에 덮어 씌우고, 맨 마지막 간선은 지운다.
	{
		e->vertexIndices[i] = lastEdge->vertexIndices[i];
		lastEdge->vertexIndices[i] = NOT_EXIST;
	}
	e->weight = lastEdge->weight;
	lastEdge->weight = 0;
	(graph->edgesSize)--;

	if (e->vertexIndices[0] != NOT_EXIST) // 삭제한 간선이 간선 배열의 마지막 간선이었던 경우는 시행할 필요 없음
	{
		// last가 옮겨진 e에 대한 인접행렬내의 간선 인덱스를 수정
		graph->adjacencyMatrix[e->vertexIndices[0]][e->vertexIndices[1]] = edgeIndex;
		graph->adjacencyMatrix[e->vertexIndices[1]][e->vertexIndices[0]] = edgeIndex;
	}
}

EdgeIndex getEdgeIndex(Graph *graph, VertexIndex v, VertexIndex w)
{
	if (graph->adjacencyMatrix[v][w] == NOT_EXIST) return NOT_EXIST;
	else return graph->adjacencyMatrix[v][w];   // 인접행렬[v][w] 의 간선인덱스를 반환
}

void changeWeight(Graph *graph, EdgeIndex edgeIndex, int weight) // 무게 변경
{
	graph->edges[edgeIndex].weight = weight;
}

int runM(Graph *graph, int a, int b, int weight)  // m 명령 실행 함수
{
	VertexIndex v = NOT_EXIST;
	VertexIndex w = NOT_EXIST;
	EdgeIndex edgeIndex = NOT_EXIST;

	v = getVertexIndex(graph, a);  // id에 해당하는 VertexIndex를 구함
	w = getVertexIndex(graph, b);

	if ((v == NOT_EXIST) || (w == NOT_EXIST)) return NOT_EXIST;  // 정점이 존재하지 않는 경우

	edgeIndex = getEdgeIndex(graph, v, w);
	if (weight == 0) // 무게가 0이면
	{
		removeEdge(graph, edgeIndex); // 간선 삭제
		return REMOVED;
	}
	else if (edgeIndex == NOT_EXIST)  // 간선이 존재하지 않는 경우
	{
		insertEdge(graph, v, w, weight);  // 새로운 간선을 추가
	}
	else  // 간선이 존재하는 경우
	{
		changeWeight(graph, edgeIndex, weight);  // 간선의 무게를 변경
	}
}


void swap(A *a, A *b)  // swap 함수
{
	int tmp = a->id;  // id를 swap
	a->id = b->id;
	b->id = tmp;

	tmp = a->weight;  // 무게를 swap
	a->weight = b->weight;
	b->weight = tmp;
}

void selectionSort(A *A, int n)  // 오름차순 정렬 
{
	int pass;
	int minLoc = 0;

	for (pass = 0; pass < (n - 1); pass++)
	{
		minLoc = pass;
		for (int j = pass + 1; j < n; j++)
		{
			if (A[j].id < A[minLoc].id) minLoc = j;
		}
		swap(&A[pass], &A[minLoc]);
	}
}

EdgeIndex *incidentEdge(Graph *graph, VertexIndex v, int *size)  // 부착 간선 인덱스 반환
{
	int i = 0;
	EdgeIndex *arr = (EdgeIndex*)malloc(sizeof(EdgeIndex)*(graph->verticesSize)); // 일단 최대값으로 저장용 배열 생성

	for (int i = 0; i < graph->verticesSize; i++)
	{
		arr[i] = NOT_EXIST;
	}

	int j = 0;
	for (int i = 0; i < graph->verticesSize; i++)  // 인접행렬 v행의 특정 열에 대해 간선인덱스가 존재하면 배열에 저장
	{
		int index = graph->adjacencyMatrix[v][i];
		if (index != NOT_EXIST)
		{
			arr[j] = index;
			j++;
			(*size)++;
		}
	}

	arr = (EdgeIndex*)realloc(arr, sizeof(EdgeIndex) * (*size));  // 배열에 추가된 만큼 배열 크기를 조정(축소)
	return arr;
}

int runA(Graph *graph, int nodeNumber, A **a, int *aSize)
{
	VertexIndex v = NOT_EXIST;
	int edgeIndexArrN = 0;
	EdgeIndex *edgeIndexArr = NULL;
	A *arr = NULL;

	v = getVertexIndex(graph, nodeNumber);
	if (v == NOT_EXIST) return NOT_EXIST;  // 정점이 존재하지 않는 경우

	edgeIndexArr = incidentEdge(graph, v, &edgeIndexArrN); // 부착간선의 배열주소를 저장 
	arr = (A*)malloc(sizeof(A)*(edgeIndexArrN)); // 부착간선과 동일한 단위크기의 구조체 생성
	for (int i = 0; i < edgeIndexArrN; i++)
	{
		arr[i].id = NOT_EXIST;
		arr[i].weight = 0;
	}

	for (int i = 0; i < edgeIndexArrN; i++) // 모든 부착간선에 대해
	{
		for (int j = 0; j < 2; j++)
		{
			VertexIndex index = graph->edges[edgeIndexArr[i]].vertexIndices[j]; // 간선의 각 정점 인덱스 중
			if ((index != v) ||  // v가 아닌 정점 인덱스
				(graph->edges[edgeIndexArr[i]].vertexIndices[0] == graph->edges[edgeIndexArr[i]].vertexIndices[1]))
				// 또는 루프인 경우
			{
				arr[i].id = graph->vertices[index].id;  // 해당 정점의 id를 arr로 가져옴
				arr[i].weight = graph->edges[edgeIndexArr[i]].weight;  // 해당 간선의 weight을 arr로 가져옴
				break;
			}
		}
	}
	free(edgeIndexArr);

	selectionSort(arr, edgeIndexArrN);  // 오름차순 정렬

	*a = arr;  // arr과 그 크기를 내보냄
	*aSize = edgeIndexArrN;
	return 0;
}