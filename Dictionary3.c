#include <stdio.h>
#include <stdlib.h>

int findKey(int l, int r, int N);

int main()
{
	int A = 0; // 시작 수
	int B = 0; // 끝 수
	int N = 0; // 문자 정보 개수

	scanf("%d", &A);
	scanf("%d", &B);
	scanf("%d", &N);

	printf(" %d", findKey(A, B, N)); // 결과 출력

	return 0;
}

int findKey(int l, int r, int N)
{
	char input = 0;
	int mid = 0;

	getchar(); // 버퍼에 있는 개행 제거
	for (int i = 0; i < N; i++)
	{
		mid = (l + r) / 2;  // 중간값
		scanf("%c", &input);

		if (input == 'Y')
		{
			l = mid + 1; // K > mid
		}
		else if (input == 'N')
		{
			r = mid; // K <= mid
		}
	}
	if (l == r) // 결과값을 찾기위한 정확한 문자들이 주어졌으므로 반드시 이 조건문에 진입하게 됨
	{
		return l; // 최종값 K. 어차피 l = r 이므로 그냥 l 리턴
	}
}