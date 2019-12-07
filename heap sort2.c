#include <stdio.h>
#include <stdlib.h>

#define ROOT_INDEX 1
#define leftChild(v) v*2
#define rightChild(v) v*2+1
#define parent(v) v/2

int *H;
int n;

void buildHeap();
void downHeap(int i);
void swap(int a, int b);
void printHeap();

int main()
{
	H = (int*)malloc(sizeof(int) * 100);

	for (int i = 0; i < 100; i++)
	{
		H[i] = 0;
	}

	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
	{
		scanf("%d", &H[i]);
	}

	buildHeap();
	printHeap();

	free(H);
    return 0;
}

void buildHeap() // 비재귀적 상향식 힙생성
{
	for (int i = parent(n); i >= ROOT_INDEX; i--)
		// 마지막노드의 부모노드부터 루트노드까지 다운힙
	{
		downHeap(i);
	}
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