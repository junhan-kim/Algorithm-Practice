#include <stdio.h>
#include <stdlib.h>

#define VERTEX_SIZE 6

void init(int **AdjArr);
void runA(int **AdjArr, int nodeNumber);
void runM(int **AdjArr, int a, int b, int w);
void freeAdjArr(int **AdjArr);

int main()
{
	int **AdjArr = NULL;  // 인접 행렬으 ㅣ주소
	char command = 0;
	int q = 0;
	int nodeNumber = 0;
	int a = 0;
	int b = 0;
	int w = 0;

	AdjArr = (int**)malloc(sizeof(int*)*VERTEX_SIZE);
	init(AdjArr);

	while (!q)
	{
		scanf("%c", &command);
		switch (command)
		{
		case 'a':
			scanf("%d", &nodeNumber);
			runA(AdjArr, nodeNumber);
			break;
		case 'm':
			scanf("%d %d %d", &a, &b, &w);
			runM(AdjArr, a, b, w);
			break;
		case 'q':
			q = 1;
			break;
		}
	}

	freeAdjArr(AdjArr); // 인접 행렬 해제
	return 0;
}


void insertEdge(int **AdjArr, int v, int opposite, int weight)  // 간선 추가
{
	v--;
	opposite--;

	AdjArr[v][opposite] = weight;
	AdjArr[opposite][v] = weight;
}

void init(int **AdjArr)
{
	for (int i = 0; i < VERTEX_SIZE; i++)
	{
		AdjArr[i] = (int*)malloc(sizeof(int)*VERTEX_SIZE);
	}

	for (int i = 0; i < VERTEX_SIZE; i++)  // 인접 행렬 초기화
	{
		for (int j = 0; j < VERTEX_SIZE; j++)
		{
			AdjArr[i][j] = 0;
		}
	}

	insertEdge(AdjArr, 1, 2, 1);
	insertEdge(AdjArr, 1, 3, 1);
	insertEdge(AdjArr, 1, 4, 1);
	insertEdge(AdjArr, 1, 6, 2);
	insertEdge(AdjArr, 2, 3, 1);
	insertEdge(AdjArr, 3, 5, 4);
	insertEdge(AdjArr, 5, 5, 4);
	insertEdge(AdjArr, 5, 6, 3);
}

void runA(int **AdjArr, int nodeNumber)
{
	if ((nodeNumber < 1) || (nodeNumber > VERTEX_SIZE))  // 정점이 존재하지 않는 경우
	{
		printf("-1\n");
		return;
	}

	for(int i = 0; i < VERTEX_SIZE; i++)  // 행 전체에서
	{
		if (AdjArr[(nodeNumber - 1)][i] != 0)  // 무게가 0이 아니면
		{
			printf(" %d %d", i + 1, AdjArr[(nodeNumber - 1)][i]);  // 반대 정점과 무게 출력
		}
	}
	printf("\n");
}

void removeEdge(int **AdjArr, int a, int b)  // 간선 삭제
{
	a--;
	b--;
	AdjArr[a][b] = 0;
	AdjArr[b][a] = 0;
}

void runM(int **AdjArr, int a, int b, int w)
{
	if ((a < 1) || (a > VERTEX_SIZE) ||   // 정점이 존재하지 않는 경우
		(b < 1) || (b > VERTEX_SIZE))
	{
		printf("-1\n");
		return;
	}

	if (w == 0)  // 무게 0이 입력된 경우
	{
		removeEdge(AdjArr, a, b);  // 간선 삭제
		printf("\n");
	}
	else  // 무게 0 이외의 값이 입력된 경우
	{
		insertEdge(AdjArr, a, b, w);  // 간선 추가
	}
}


void freeAdjArr(int **AdjArr)
{
	for (int i = 0; i < VERTEX_SIZE; i++)
	{
		free(AdjArr[i]);
	}
	free(AdjArr);
}