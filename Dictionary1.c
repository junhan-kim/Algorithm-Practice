#include <stdio.h>
#include <stdlib.h>

int findKey(int *A, int k, int l, int r);

int main()
{
	int N = 0;
	int K = 0;
	int *A = NULL;

	scanf("%d", &N);
	scanf("%d", &K);
	A = (int*)malloc(sizeof(int)*N);
	
	for (int i = 0; i < N; i++)
	{
		scanf("%d", &A[i]);
	}

	printf(" %d", findKey(A, K, 0, N - 1));
	
	free(A);
    return 0;
}

int findKey(int *A, int k, int l, int r)
{
	int mid = 0;
	mid = (l + r) / 2;  // 중간키

	if (l > r) // 예외
	{
		return r;
	}

	if (k == A[mid]) // 같을때 중간키 반환
		return mid;
	else if (k < A[mid]) // 작을 땐 시작인덱스와 중간키보다 하나 작은 인덱스 범위에서 재귀
		return findKey(A, k, l, mid - 1);
	else if (k > A[mid]) // 클 땐 중간키보다 하나 큰 인덱스와 끝인덱스 범위에서 재귀
		return findKey(A, k, mid + 1, r);
}