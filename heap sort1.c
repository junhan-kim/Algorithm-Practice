#include <stdio.h>
#include <stdlib.h>

#define ROOT_INDEX 1
#define leftChild(v) v*2
#define rightChild(v) v*2+1
#define parent(v) v/2

int *H;
int n;

void insertItem(int key);
void upHeap(int i);
int removeMax();
void downHeap(int i);
void swap(int a, int b);
void printHeap();


int main()
{
	char command = 0;
	H = (int*)malloc(sizeof(int) * 100);

	for (int i = 0; i < 100; i++)
	{
		H[i] = 0;
	}

	while (command != 'q')
	{
		int key = 0;
		scanf("%c", &command);
		switch (command)
		{
		case 'i':
			scanf("%d", &key);
			insertItem(key);
			printf("%d", 0);
			break;
		case 'd':
			printf("%d", removeMax());
			break;
		case 'p':
			printHeap();
			break;
		}
		printf("\n");
		getchar();
	}

    free(H);
    return 0;
}

void insertItem(int key)
{
	n++; // 힙 확장
	H[n] = key; // 마지막 노드에 키값 삽입
	upHeap(n); // 힙 조정
}

void upHeap(int i)
{
	if (i == ROOT_INDEX) return; // 루트노드거나
	if (H[i] <= H[parent(i)]) return; // 부모노드보다 작거나 같으면 끝(최대힙)
	
	swap(i, parent(i)); //부모노드와 키값 스왑

	upHeap(parent(i)); // 부모노드로 재귀
}

int removeMax() // 루트키 삭제
{
	int removedKey = H[ROOT_INDEX]; // 루트키 보관
	H[ROOT_INDEX] = H[n]; // 마지막 키를 루트키로 옮김
	n--; // 힙 축소
	downHeap(ROOT_INDEX); // 힙 조정

	return removedKey;
}

void downHeap(int i)
{
	int bigger = 0;

	if ((leftChild(i) > n) && (rightChild(i) > n)) return; //자식노드가 외부노드일때 끝
	
	bigger = leftChild(i); // bigger를 왼쪽 자식으로 초기화
	if (rightChild(i) <= n) // 오른쪽 자식이 내부노드고
	{
		if (H[rightChild(i)] > H[bigger]) // bigger보다 크면
		{
			bigger = rightChild(i); // 오른쪽 자식을 bigger로 바꿈
		}
	}
	if (H[i] >= H[bigger]) return; // 자식들보다 키값이 크면 끝
	
	swap(i, bigger); // 자식노드와 키값 스왑

	downHeap(bigger); // 자식노드로 재귀
}

void swap(int a, int b)
{
	int tmp = 0;
	tmp = H[a];
	H[a] = H[b];
	H[b] = tmp;
}

void printHeap()
{
	for (int i = 1; i <= n; i++)
	{
		printf(" %d", H[i]);
	}
}