#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int N; // 배열 크기

void inPlaceQuickSort(int **A, int l, int r);
int find_pivot_index(int *A, int l, int r); 
int *inPlacePartition(int *A, int l, int r, int k);
void swap(int *a, int *b);

int main()
{
	srand(time(NULL));

	int *A = NULL;

	scanf("%d", &N);
	A = (int*)malloc(sizeof(int)*N);

	for (int i = 0; i < N; i++) // 입력
	{
		scanf("%d", &A[i]);
	}

	inPlaceQuickSort(&A, 0, N - 1); // 퀵소트에 배열 A의 주소와 시작,끝 인덱스 전달


	for (int i = 0; i < N; i++) // 출력
	{
		printf(" %d", A[i]);
	}

	free(A); // 해제
    return 0;
}


void inPlaceQuickSort(int **A, int l, int r)
{
	int k = 0;
	int a = 0;
	int b = 0;
	int *ab = NULL;

	if (l >= r) // 한칸이하이면 더 이상 나누지 않음.
	{
		return;
	}

	k = find_pivot_index(*A, l, r); // 피벗의 인덱스를 k에 전달

	ab = inPlacePartition(*A, l, r, k); // EQ의 시작과 끝 인덱스를 각각 a,b에 전달
	a = ab[0];
	b = ab[1];

	inPlaceQuickSort(A, l, a - 1); // LT를 재귀
	inPlaceQuickSort(A, b + 1, r); // GT를 재귀
}


int find_pivot_index(int *A, int l, int r)
{
	int index[3] = { 0, };
	int randIndex = 0;
	int median = 0;
	int tmp = 0;

	for (int i = 0; i < 3; i++)
	{
		randIndex = (rand() % (r - l + 1)) + l; // l과 r사이 값을 랜덤으로 뽑아 index 배열에 넣음
		index[i] = randIndex;
	}

	if (A[index[0]] >= A[index[1]]) // index[0]와 index[1] 중 큰 값의 인덱스를 median에 넣고, 작은 값의 인덱스를 tmp에 넣음
	{
		median = index[0];
		tmp = index[1];
	}
	else
	{
		median = index[1];
		tmp = index[0];
	}
	if (A[index[2]] <= A[median]) // index[2]가 median보다 큰 값이면 기존 median이 중간값임.
		                          // 그게 아니라면, index[2]와 tmp를 비교하여 더 큰 값이 중간값임.
	{
		if (A[index[2]] <= A[tmp])
		{
			median = tmp;
		}
		else
		{
			median = index[2];
		}
	}

	return median;
}

int *inPlacePartition(int *A, int l, int r, int k)
{
	int i = l;
	int p = A[k]; // 피벗 초기화
	int ab[2] = { 0 , 0 };

	while (i <= r) // i를 l에서 r까지 반복
	{
		if (A[i] < p) // 피벗보다 작은 경우
		{
			swap(&A[l], &A[i]); // l위치와 교환
			l++;
			i++;
		}
		else if (A[i] > p) // 피벗보다 큰 경우
		{
			swap(&A[i], &A[r]); // r위치와 교환
			r--;
		}
		else if (A[i] == p) // 같을땐 넘김
		{
			i++;
		}
	}
	
	ab[0] = l; // 최종적인 l은 EQ의 시작, r은 EQ의 끝이 됨.
	ab[1] = r;
	return ab;
}

void swap(int *a, int *b) // 교환 함수
{
	int tmp = 0;
	tmp = *a;
	*a = *b;
	*b = tmp;
}