#include <stdio.h>
#include <stdlib.h>

#define h(k) k % M  // 인라인 해쉬 함수

typedef struct List
{
	struct List *next;
	int key;
}List;

int findElementInHash(List **A, int k, int M);
void insertItemInHash(List **A, int k, int M);
int removeElementInHash(List **A, int k, int M);
void printHashTable(List **A, int M);
void freeHashTable(List **A, int M);

int main()
{
	int M = 0;
	
	scanf("%d", &M);
	List **A = (List**)malloc(sizeof(List*)*M); // 버켓 배열 생성(리스트들의 주소를 저장)
	for (int i = 0; i < M; i++) // 각 배열 원소에 리스트의 헤더를 연결하고 생성 및 초기화 해줌
	{
		A[i] = (List*)malloc(sizeof(List));
		A[i]->next = NULL;
	}

	int e = 1; // exit 부울 변수
	int k = 0;
	char cInput = 0;
	
	while (e)
	{
		getchar();
		scanf("%c", &cInput);
		switch (cInput)
		{
		case 'i': // 삽입
			scanf("%d", &k);
			insertItemInHash(A, k, M);
			break;
		case 's': // 탐색
			scanf("%d", &k);
			printf("%d\n", findElementInHash(A, k, M));
			break;
		case 'd':
			scanf("%d", &k);
			printf("%d\n", removeElementInHash(A, k, M));
			break;
		case 'p':
			printHashTable(A, M);
			break;
		case 'e':
			e = 0; // e <- false
			break;
		}
	}

	freeHashTable(A, M);
    return 0;
}


int findElementinList(List *L, int k)
{
	int cnt = 0;

	L = L->next; // 처음 노드로 이동
	while (L != NULL)
	{
		cnt++;
		if (L->key == k) // 키를 찾으면 cnt 반환
		{
			return cnt;
		}
		L = L->next;
	}
	return 0; // 못 찾으면 0 반환
}

int findElementInHash(List **A, int k, int M)
{
	int v = h(k); // 키를 해싱하여 버켓배열의 인덱스를 얻음 
	return findElementinList(A[v], k);
}


void insertItemInList(List *L, int k) // L은 해당 리스트의 헤더
{
	List *node = (List*)malloc(sizeof(List));
	node->key = k;

	List *p = L->next; // 헤더 바로 뒤에 있는 리스트의 주소 저장(첫 노드)
	L->next = node; // 헤더 뒤에 노드 연결
	node->next = p; // 그 노드 뒤에 아까 저장해둔 노드 연결
}

void insertItemInHash(List **A, int k, int M)
{
	int v = h(k);
	insertItemInList(A[v], k);
}


int removeElementInList(List *L, int k)
{
	int cnt = 0;

	while (L->next != NULL) // 싱글리스트에서 삭제를 수행하기 위해 이전노드의 위치에서 다음노드에 대한 작업 수행
	{
		cnt++;
		if (L->next->key == k)
		{
			List *p = L->next->next; // p에 다음다음노드 임시 저장
			free(L->next); // 다음노드 삭제
			L->next = p; // 현재 노드에 다음다음노드를 연결

			return cnt;
		}
		L = L->next;
	}
	return 0;
}

int removeElementInHash(List **A, int k, int M)
{
	int v = h(k);
	return removeElementInList(A[v], k);
}


void printList(List *L)
{
	L = L->next; // 첫 노드로 이동
	while (L != NULL) // 널이 아닌 모든 노드 출력
	{
		printf(" %d", L->key);
		L = L->next;
	}
}

void printHashTable(List **A, int M)
{
	for (int i = 0; i < M; i++) // 버켓배열 순회
	{
		printList(A[i]);
	}
	printf("\n");
}


void freeList(List *L)
{
	while (L->next != NULL) // 다음노드가 NULL이 아닌 동안 현재노드 삭제
	{
		List *tmp = L->next;
		free(L); // L이 가리키는 메모리 삭제
		L = tmp;
	}
	free(L); // 마지막 노드 삭제
}

void freeHashTable(List **A, int M)
{
	for (int i = 0; i < M; i++)
	{
		freeList(A[i]); // 리스트 삭제(헤더 포함)
	}

	free(A); // 버켓배열 삭제
}
