#include<stdio.h>
#include <stdlib.h>

int main()
{
	int N = 0;
	int maxLoc = 0;
	int tmp = 0;

	scanf("%d", &N);
	int *A = (int*)malloc(sizeof(int)*N);
	for (int i = 0; i < N; i++)
	{
		scanf("%d", &A[i]);
	}

	for (int pass = N - 1; pass > 0; pass--) // 맨 마지막은 안해도 됨
	{
		maxLoc = pass; // minLoc의 초기값이 반복시마다 하나씩 줄어듬
		for (int j = pass - 1; j >= 0; j--) // 초기값 앞에서부터 순회
		{
			if (A[j] > A[maxLoc]) // 제일 큰 값을 maxLoc에 넣음
				maxLoc = j;
		}
		tmp = A[maxLoc];
		A[maxLoc] = A[pass];
		A[pass] = tmp; // 최종 maxLoc과 초기값(pass)를 교환
	}

	for (int i = 0; i < N; i++)
	{
		printf(" %d", A[i]);
	}

	free(A);

	return 0;
}