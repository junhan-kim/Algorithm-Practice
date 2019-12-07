#include <stdio.h>
#include <stdlib.h>

#define VERTEX_SIZE 6

typedef struct List
{
	int opposite;
	int weight;
	struct List *next;
}List;

void init(List **nodePArr);
void runA(List **nodePArr, int nodeNumber);
void runM(List **nodePArr, int a, int b, int w);
void freeList(List **nodePArr);

int main()
{
	List **nodePArr = NULL; // 노드 포인터 배열의 주소
	char command = 0;
	int q = 0;
	int nodeNumber = 0;
	int a = 0;
	int b = 0;
	int w = 0;

	nodePArr = (List**)malloc(sizeof(List*)*VERTEX_SIZE);
	init(nodePArr);
		
	while (!q)
	{
		scanf("%c", &command);
		switch (command)
		{
		case 'a':
			scanf("%d", &nodeNumber);
			runA(nodePArr, nodeNumber);
			break;
		case 'm':
			scanf("%d %d %d", &a, &b, &w);
			runM(nodePArr, a, b, w);
			break;
		case 'q':
			q = 1;
			break;
		}
	}

	freeList(nodePArr); // 노드 포인터 배열, 부착 리스트 해제
    return 0;
}


void insertEdge(List **nodePArr, int v, int opposite, int weight)
{
	for (int i = 0; i < 2; i++)
	{
		List *nodeP = NULL;
		List *node = NULL;
		int tmp = 0;

		nodeP = nodePArr[v - 1]; // 입력된 정점의 id - 1 = 해당 정점 부착 리스트의 헤더

		while ((nodeP->next != NULL) &&   // 다음 노드가 NULL이 아니거나
			(opposite > nodeP->next->opposite))  // 입력된 반대 끝점이 아직 다음 노드의 반대 끝점 값보다 클 때
		{
			nodeP = nodeP->next;
		}

		// 입력된 반대 끝점이 다음 노드의 반대 끝점과 같으면 
		if ((nodeP->next != NULL) && (opposite == nodeP->next->opposite))
		{
			nodeP->next->weight = weight;  // 다음 노드의 무게 변경
		}
		else
		{
			node = (List*)malloc(sizeof(List));  // 새 노드 생성하여 새 간선의 정보 입력
			node->opposite = opposite;
			node->weight = weight;
			node->next = NULL;

			node->next = nodeP->next;  // nodeP의 다음 위치에 노드 삽입하여 간선 추가
			nodeP->next = node;
		}

		if (v == opposite) break;  // 루프인 경우 간선을 1번만 추가 (중복되지 않도록)
		if (i == 0)
		{
			tmp = v;  // 정점과 반대 끝점을 바꾼 위치에도 간선을 추가
			v = opposite;
			opposite = tmp;
		}	
	}
}

void init(List **nodePArr)
{
	for (int i = 0; i < VERTEX_SIZE; i++)
	{
		nodePArr[i] = (List*)malloc(sizeof(List)); // 각 정점 부착리스트의 헤더(각 노드 포인터가 가리키는 곳)
		nodePArr[i]->next = NULL;
	}

	insertEdge(nodePArr, 1, 2, 1);
	insertEdge(nodePArr, 1, 3, 1);
	insertEdge(nodePArr, 1, 4, 1);
	insertEdge(nodePArr, 1, 6, 2);
	insertEdge(nodePArr, 2, 3, 1);
	insertEdge(nodePArr, 3, 5, 4);
	insertEdge(nodePArr, 5, 5, 4);
	insertEdge(nodePArr, 5, 6, 3);
}

void runA(List **nodePArr, int nodeNumber)
{
	if ((nodeNumber < 1) || (nodeNumber > VERTEX_SIZE))  // 정점이 존재하지 않는 경우
	{
		printf("-1\n");
		return;
	}

	List *nodeP = nodePArr[nodeNumber - 1]->next;
	while (nodeP != NULL)  // 해당 정점 부착 리스트의 모든 노드(연결된 모든 간선) 순회
	{
		printf(" %d %d", nodeP->opposite, nodeP->weight);
		nodeP = nodeP->next;
	}
	printf("\n");
}

void removeEdge(List **nodePArr, int a, int b)
{
	int tmp = 0;

	for (int i = 0; i < 2; i++)
	{
		List *nodeP = nodePArr[a - 1];
		while ((nodeP->next != NULL) &&  // 다음 노드가 NULL이 아니거나 
			(b != nodeP->next->opposite)) // 입력된 반대 끝점이 다음 노드의 반대 끝점 값과 다를 때
		{
			nodeP = nodeP->next;
		}
		if (nodeP->next != NULL)  // 다음 노드가 NULL일 때를 제외한 경우 즉, 반대 끝점 값이 서로 같은 경우
		{
			List *tmp = nodeP->next;  // nodeP 다음 위치의 노드 삭제
			nodeP->next = nodeP->next->next;
			free(tmp);
		}

		if (a == b) break;  // 루프인 경우 한번만 삭제 (잘못된 위치 참조 방지)
		if (i == 0)
		{
			tmp = a;  // 정점과 반대 끝점을 바꾼 위치에도 간선을 삭제
			a = b;
			b = tmp;
		}
	}
}

void runM(List **nodePArr, int a, int b, int w)
{
	if ((a < 1) || (a > VERTEX_SIZE) ||   // 정점이 존재하지 않는 경우
		(b < 1) || (b > VERTEX_SIZE))
	{
		printf("-1\n");
		return;
	}

	if (w == 0)  // 무게 0이 입력된 경우
	{
		removeEdge(nodePArr, a, b);  // 간선 삭제
		printf("\n");
	}
	else  // 무게 0 이외의 값이 입력된 경우
	{
		insertEdge(nodePArr, a, b, w);  // 간선 추가
	}
}


void freeList(List **nodePArr)
{
	for (int i = 0; i < VERTEX_SIZE; i++)
	{
		List *p = nodePArr[i]->next;
		List *tmp = NULL;

		while (p != NULL)
		{
			tmp = p->next;  // 각 부착 리스트의 노드를 삭제 (헤더 포함)
			free(p);
			p = tmp;
		}
		free(nodePArr[i]);  // 노드 포인터 배열을 삭제
	}
	free(nodePArr);  // 노드 포인터 배열의 주소를 삭제
}