#include <stdio.h>
#include <stdlib.h>


typedef struct SingleLinkedList {
	struct SingleLinkedList *next;
	int elem;
}List;


void init(List *p);

void mergeSort(List **L);
int listSize(List *p);
List **mg_partition(List *L, int k);

List *merge(List *L1, List *L2);
int isEmpty(List *p);
int getFirst(List *p);
void addLast(List *p, int elem);
int removeFirst(List *p);
void addLast(List *p, int elem);

void print(List *p);
void Free(List *p);

//------------------------------------------------------------

int main()
{
	List *head = (List*)malloc(sizeof(List));

	init(head);	
	mergeSort(&head);
	print(head);
	
	Free(head);
    return 0;
}

//------------------------------------------------------------

void init(List *p) // 리스트 초기화
{
	int N = 0;
	int input = 0;

	scanf("%d", &N);
	for (int i = 0; i < N; i++)
	{
		scanf("%d", &input);

		List *node = (List*)malloc(sizeof(List)); // 새 노드만들어서 입력된 값넣고 이전 노드와 연결.
		node->elem = input;
		p->next = node;
		
		p = p->next;
	}
	p->next = NULL; // 맨 마지막 노드의 next를 NULL로 초기화(제어문에서 이용하기 위하여)
}

//------------------------------------------------------------

void mergeSort(List **L) // 오름차순 정렬, 리스트의 주소를 전달
{
	int n = listSize(*L);
	if (n > 1) // 1이하이면 나감
	{
		List **L1L2 = mg_partition(*L, n / 2);
		List *L1 = L1L2[0]; // 반으로 쪼개서 L1, L2로 전달
		List *L2 = L1L2[1];
		
		mergeSort(&L1); // 계속 반으로 쪼개도록 재귀
		mergeSort(&L2);
		
		*L = merge(L1, L2); // 합병.  더블포인터를 썼기 때문에, 합병한 결과를 전달된 인자로 되돌려보냄.
	}
	return;
}


int listSize(List *p) // 리스트 크기 반환.
{
	int count = 0;

	p = p->next;
	while (p != NULL) // 순회할때마다 카운트를 1씩 늘림.
	{
		count++;
		p = p->next;
	}
	return count;
}

List **mg_partition(List *L, int k)
{
	List *L1L2[2] = { NULL, };
	List *L1 = L; // 기존 L의 헤더를 L1 으로 백업
	List *L2 = (List*)malloc(sizeof(List)); // L2 헤더 생성

	for (int i = 0; i < k; i++) // 첫번째 리스트의 맨끝으로 이동
	{
		L = L->next;
	}
	List *tmp = L->next; // L(현재 첫번째 리스트의 맨끝)의 다음 주소(두번째 리스트의 맨앞)를 임시 저장
	L->next = NULL; // L의 next는 NULL을 가리킴(두번째 리스트와 연결 끊음)
	L2->next = tmp; // L1과 끊어진 리스트는 L2가 됨

	L1L2[0] = L1;
	L1L2[1] = L2;
	return L1L2;
}

//------------------------------------------------------------

List *merge(List *L1, List *L2) // 두 리스트를 하나로 합병
{
	List *L = (List*)malloc(sizeof(List));
	L->next = NULL;
	
	while (!isEmpty(L1) && !isEmpty(L2)) // 둘 다 비어있지 않을 때
	{
		if (getFirst(L1) <= getFirst(L2)) // L1이 더 작으면
		{
			addLast(L, removeFirst(L1)); // L1의 첫 elem을 삭제하여 L에 추가
		}
		else // L2가 더 작으면
		{
			addLast(L, removeFirst(L2)); // L2의 첫 elem을 삭제하여 L에 추가
		}
	}
	while(!isEmpty(L1)) // 나머지 처리
		addLast(L, removeFirst(L1));
	while(!isEmpty(L2))
		addLast(L, removeFirst(L2));

    free(L1);
    free(L2);

	return L; // 합병된 리스트 L 반환
}


int isEmpty(List *p) // 리스트가 비어있는가
{
	if (p->next == NULL) return 1;
	else return 0;
}

int getFirst(List *p) // 첫 노드의 elem 반환
{
	p = p->next;
	return p->elem;
}

void addLast(List *p, int elem) // 새 노드를 만들어 elem을 넣고 p 뒤에 연결.
{
	List *node = (List*)malloc(sizeof(List));
	node->elem = elem;
	node->next = NULL;
	
	while (p->next != NULL) // 마지막 노드까지 이동
	{
		p = p->next;
	}
	p->next = node; // 기존 리스트 끝에 삽입
}

int removeFirst(List *p) // 첫 노드 삭제 후 elem 반환
{
	int tmp = 0;
	List *head = p; // 헤더 저장
	
	p = p->next;
	tmp = p->elem;
	head->next = p->next; // 헤더의 next에 삭제할 p의 다음 노드를 저장
	free(p);

	return tmp;
}

//------------------------------------------------------------

void print(List *p)  // 리스트 출력
{
	p = p->next;
	while(p != NULL)
	{
		printf(" %d", p->elem);
		p = p->next;
	}
}

void Free(List *p) // 리스트의 노드들 할당 해제
{
	List *tmp = NULL;
	while (p != NULL)
	{
		tmp = p->next; // 해제하기전 다음 노드주소 저장
		free(p); // 해제
		p = tmp;
	}
}