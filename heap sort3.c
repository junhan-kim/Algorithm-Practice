#include <stdio.h>
#include <stdlib.h>

#define ROOT_INDEX 1
#define leftChild(v) v*2
#define rightChild(v) v*2+1
#define parent(v) v/2

int *H;
int n;

void buildHeap();
void inPlaceHeapSort();
void downHeap(int i);
void downHeap_inPlaceHeapSort(int i, int last);
void swap(int a, int b);
void printArray();

int main()
{
	int tmp = 0;
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
	inPlaceHeapSort();
	printArray();

	free(H);
	return 0;
}


void inPlaceHeapSort()
{
	int tmp = 0;

	for (int i = n; i >= 2; i--) // 맨 마지막 반복은 정렬완료된 상태이므로 2까지만
	{
		tmp = H[i];
		H[i] = H[1];
		H[1] = tmp; // 루트노드와 마지막노드의 값을 교환
		downHeap_inPlaceHeapSort(1, i - 1); // 재귀를 하며 마지막 인자가 정렬되므로 last에 -1 해주어 전달
	}
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

void downHeap_inPlaceHeapSort(int i, int last) // 힙 정렬용 다운 힙
{
	int tmp = 0;
	int greater = 0;
	int left = leftChild(i);
	int right = rightChild(i);
	
	if (left > last) // left가 외부노드이면 끝
		return;
	greater = left; // 일단 greater에 left 대입

	if (right <= last) // right이 범위 내에 있으면서
		if (H[right] > H[greater]) // greater의 값보다 크면
			greater = right; // right를 대입 (즉, greater = 자식 노드중 가장 큰 노드, i = greater의 부모노드)

	if (H[i] >= H[greater]) return; // 부모가 더 크면 이미 정렬된 것이므로 끝
	
	tmp = H[i];
	H[i] = H[greater];
	H[greater] = tmp; // 부모와 자식의 값을 교환

	downHeap_inPlaceHeapSort(greater, last); // 자식노드로서 재귀
}

void swap(int a, int b)
{
	int tmp = 0;
	tmp = H[a];
	H[a] = H[b];
	H[b] = tmp;
}

void printArray()
{
	for (int i = 1; i <= n; i++)
	{
		printf(" %d", H[i]);
	}
}



