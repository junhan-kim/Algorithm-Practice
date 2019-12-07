#include <stdio.h>
#include <stdlib.h>

#define h(k) ((k) % (M))  // 해쉬함수
#define h2(k, q) ((q) - ((k) % (q))) // 두번째 해쉬함수
#define f(i, k, q) ((i) * (h2(k, q)))  // 이중 해싱
#define EMPTY 0
#define NO_SUCH_KEY -1
#define TRUE 1
#define FALSE 0

void findElement(int *A, int k, int M, int *retArray, int q);
int insertItem(int *A, int k, int M, int N, int q);
void printHashTable(int *A, int M);

int main()
{
	int M = 0;
	int N = 0;
	int q = 0;

	scanf("%d", &M);
	scanf("%d", &N);
	scanf("%d", &q);
	int *A = (int*)malloc(sizeof(int)*M); // 버켓 배열 생성(리스트들의 주소를 저장)
	for (int i = 0; i < M; i++) // 각 배열 원소에 리스트의 헤더를 연결하고 생성 및 초기화 해줌
	{
		A[i] = EMPTY;
	}

	int e = 1; // exit 부울 변수
	char cInput = 0;
	int k = 0;
	int overflowException = 0;
	int retArray[2] = { 0 , 0 }; // out 변수

	while (e)
	{
		getchar();
		scanf("%c", &cInput);
		switch (cInput)
		{
		case 'i': // 삽입
			scanf("%d", &k);
			overflowException = insertItem(A, k, M, N, q);
			// if(overflowException) { // 재해싱 }
			printf("\n");
			break;
		case 's': // 탐색
			scanf("%d", &k);
			findElement(A, k, M, retArray, q);
			if (retArray[0] == NO_SUCH_KEY) // 키 없을땐 NO_SUCH_KEY 출력
			{
				printf("%d", retArray[0]);
			}
			else // 있을땐 인덱스와 학번 출력
			{
				printf("%d %d", retArray[0], retArray[1]);
			}
			printf("\n");
			break;
		case 'p':
			printHashTable(A, M);
			break;
		case 'e':
			e = 0; // e <- false
			printHashTable(A, M);
			break;
		}
	}

	free(A);
	return 0;
}


int getNextBucket(int v, int i, int M, int k, int q)
{
	return ((v + f(i, k, q)) % M); // 기존 인덱스에 i만큼 더한 곳으로 이동 (%M이므로 M보다 같거나 커지면 Wrap Around) 
}

void findElement(int *A, int k, int M, int *retArray, int q)
{
	int v = h(k); // 해싱한 값을 v에 저장

	int i = 0;
	while (i < M) // 한바퀴 다 돌았는데도 없으면 끝
	{
		int b = getNextBucket(v, i, M, k, q); // 다음 인덱스 찾기
		if (A[b] == EMPTY) // 비어있는 경우
		{
			retArray[0] = NO_SUCH_KEY;
			return;
		}
		else if (A[b] == k) // 비어있지 않고 키가 같은 경우
		{
			retArray[0] = b;
			retArray[1] = k;
			return;
		}
		else // 비어있지는 않은데 키가 다른 경우
		{
			i++;
		}
	}

	retArray[0] = NO_SUCH_KEY; // 결국 못 찾음
	return;
}


int overflowException(int M, int N)
{
	if (N >= M) return TRUE; // N은 M보다 작은 자연수
	else return FALSE;
}

int insertItem(int *A, int k, int M, int N, int q)
{
	int v = h(k);

	int i = 0;
	while (i < M)
	{
		int b = getNextBucket(v, i, M, k, q);
		if (A[b] == EMPTY) // 비어있는 경우
		{
			A[b] = k; // 키 삽입
			printf("%d", b);
			return FALSE;
		}
		else // 비어있지 않은 경우
		{
			printf("C"); // 충돌
			i++;
		}
	}

	return overflowException(M, N); // 꽉 찼을 때 예외처리
}


void printHashTable(int *A, int M) // 해쉬 테이블 출력
{
	for (int i = 0; i < M; i++)
	{
		printf(" %d", A[i]);
	}
	printf("\n");
}