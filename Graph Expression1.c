#include <stdio.h>
#include <stdlib.h>

#define NOT_EXIST -1
#define REMOVED -2

typedef int VertexIndex;
typedef int EdgeIndex;

typedef struct IncidenceList
{
	struct IncidenceList *next;
	int edgeIndex;
}IncidenceList;

typedef struct Vertex
{
	IncidenceList *header;
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
void freeIncidenceList(Graph *graph);

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
	freeIncidenceList(graph);
	free(graph->vertices);
	free(graph->edges);
	free(graph);
    return 0;
}


void freeIncidenceList(Graph *graph)
{
	IncidenceList *tmp = NULL;

	for (int i = 0; i < graph->verticesSize; i++)  // 모든 정점에 대해
	{
		while (graph->vertices[i].header != NULL)
		{
			tmp = graph->vertices[i].header;  // 헤더 주소 저장
			graph->vertices[i].header = graph->vertices[i].header->next;  // 헤더를 바로 뒤 노드로 바꿈
			free(tmp); // 기존 헤더 할당 해제
		}
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
		graph->vertices[i].header = (IncidenceList*)malloc(sizeof(IncidenceList)); // 헤더를 초기화
		graph->vertices[i].header->next = NULL;
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

	for (int i = 0; i < 2; i++)  // 간선의 정점에 대한 부착리스트에 간선 인덱스를 담고 있는 노드 추가
	{
		IncidenceList *header = graph->vertices[edge->vertexIndices[i]].header;
		IncidenceList *node = (IncidenceList*)malloc(sizeof(IncidenceList));

		node->edgeIndex = edgeIndex;
		node->next = header->next;
		header->next = node;
		if (edge->vertexIndices[0] == edge->vertexIndices[1]) break;  // 루프면 한번만 실행
	}
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
	IncidenceList *list = NULL;

	for (int i = 0; i < 2; i++) // 간선의 양끝점을 부착리스트에서 제거
	{
		list = graph->vertices[e->vertexIndices[i]].header;
		while (list->next != NULL)
		{
			if (list->next->edgeIndex == edgeIndex) // 지워야할 edgeIndex를 발견한 경우
			{
				IncidenceList *tmp = list->next;  //  리스트의 노드 삭제
				list->next = list->next->next;
				free(tmp);
				break;
			}
			list = list->next;
		}
	}

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
		for (int i = 0; i < 2; i++) // 마지막 간선의 인덱스를 가지고 있는 정점들을 찾아 부착 리스트에서 인덱스값을 수정
		{
			IncidenceList *list = graph->vertices[e->vertexIndices[i]].header; // last값이 옮겨진 e를 통해
			while (list->next != NULL)
			{
				if (list->next->edgeIndex == lastEdgeIndex) // 기존 lastEdgeIndex와 같은 값을 찾으면
				{
					list->next->edgeIndex = edgeIndex; // 옮겨진 위치의 index로 변경
					break;
				}
				list = list->next;
			}
		}
	}
}


EdgeIndex getEdgeIndex(Graph *graph, VertexIndex v, VertexIndex w)
{
	IncidenceList *header = graph->vertices[v].header;  
	  // v 위치의 정점배열이 가지고 있는 연결리스트의 첫 간선인덱스부터 시작
	IncidenceList *list = header->next;
	while (list != NULL)
	{
		for (int i = 0; i < 2; i++)
		{
			VertexIndex index = graph->edges[list->edgeIndex].vertexIndices[i]; // 간선의 두 정점 인덱스 중
			if ((index != v) // v가 아닌 정점 인덱스인 경우
				|| (graph->edges[list->edgeIndex].vertexIndices[0] == graph->edges[list->edgeIndex].vertexIndices[1]))
				 // 또는 두 정점 인덱스가 같은 경우 (루프) 
			{
				if (index == w) return list->edgeIndex; // 그 정점 인덱스가 w와 같다면 해당 간선 인덱스를 반환
			}
		}
		list = list->next;
	}
	return NOT_EXIST;
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
	IncidenceList *list = NULL;

	for (int i = 0; i < graph->verticesSize; i++)
	{
		arr[i] = NOT_EXIST;
	}
	list = graph->vertices[v].header->next;  // 해당 정점의 부착리스트
	
	i = 0;
	while (list != NULL) // 모든 부착리스트의 값을 배열에 추가
	{
		arr[i] = list->edgeIndex;  
		i++;
		(*size)++;
		list = list->next;
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