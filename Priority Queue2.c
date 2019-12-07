#include <stdio.h>
#include <stdlib.h>

int main()
{
	int save = 0;
	int j = 0;

	int N = 0;
	scanf("%d", &N);

	int *A = (int*)malloc(sizeof(int)*N);
	for (int i = 0; i < N; i++)
	{
		scanf("%d", &A[i]);
	}

	for (int pass = 1; pass < N; pass++) // 1부터 시작
	{
		save = A[pass]; // 삽입할 요소 저장
		j = pass - 1; // 그 앞쪽부터 반복
		while ((j >= 0) && (A[j] > save))
		{
			A[j + 1] = A[j]; // 자기 위치 찾을 때까지 뒤로 밀기
			j--;
		}
		A[j + 1] = save; // 찾으면 삽입
	}

	for (int i = 0; i < N; i++)
	{
		printf(" %d", A[i]);
	}

	free(A);

	return 0;
}
