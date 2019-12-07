#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct Tree
{
	int key;
	struct Tree *parent;
	struct Tree *lChild;
	struct Tree *rChild;
}Tree;

Tree *root; // 전역 루트노드
int noSuchKey = FALSE;

// 기본 메서드 --------------------------------------------------
Tree *sibling(Tree *v) // 형제노드
{
	Tree *p = v->parent;
	if (v == p->lChild) return p->rChild;
	else if (v == p->rChild) return p->lChild;
}

int isExternal(Tree *v) // 왼쪽,오른쪽 자식 모두 NULL = 외부노드
{
	if ((v->lChild == NULL) && (v->rChild == NULL)) return TRUE;
	else return FALSE;
}

void expandExternal(Tree *v) // 외부노드들을 추가
{
	Tree *l = (Tree*)malloc(sizeof(Tree));
	Tree *r = (Tree*)malloc(sizeof(Tree));
	l->parent = v;
	l->lChild = NULL;
	l->rChild = NULL;
	r->parent = v;
	r->lChild = NULL;
	r->rChild = NULL;
	v->lChild = l;
	v->rChild = r;
}

Tree *reduceExternal(Tree *z) // 외부노드와 부모노드를 삭제
{
	Tree *w = z->parent;
	Tree *zs = sibling(z);

	if (w == root) // w가 루트이면
	{
		root = zs; // zs를 루트화
		zs->parent = NULL;
	}
	else
	{
		Tree *g = w->parent; // g와 zs를 부모-자식 관계로 연결
		zs->parent = g;
		if (w == g->lChild) g->lChild = zs; // g와 w의 관계에 따라 왼쪽, 오른쪽 결정
		else g->rChild = zs;
	}
	free(w);
	free(z);

	return zs; // 삭제한 외부노드의 형제노드였던 노드를 반환
}

// 이진탐색트리 보조메서드  --------------------------------------------------
Tree *inOrderSucc(Tree *v) // 이진탐색트리에서의 중위순회계승자
{
	v = v->rChild; // 오른쪽 자식으로 이동
	while (!isExternal(v->lChild)) v = v->lChild; // 왼쪽 자식이 외부노드가 아닌 경우, 왼쪽 자식으로 이동
	return v; // 외부노드인 왼쪽 자식을 반환
}

Tree* treeSearch(Tree *v, int k)
{
	if (isExternal(v)) return v; // 외부노드(키가 있어야 할 곳)까지 도달한 경우 현재 노드를 반환
	
	if (k < v->key) treeSearch(v->lChild, k); // 키가 작으면 왼쪽 자식으로 이동
	else if (k > v->key) treeSearch(v->rChild, k); // 크면 오른쪽 자식으로 이동
	else if (k == v->key) return v; // 같으면 현재 노드를 반환
}

void treePrintToPreOrder(Tree *v) // 전위순회 출력
{
	if (!isExternal(v)) printf(" %d", v->key); // 내부노드의 key만 출력
	if (!isExternal(v)) treePrintToPreOrder(v->lChild);
	if (!isExternal(v)) treePrintToPreOrder(v->rChild);
}

void treeFree(Tree *v) // 후위순회 해제
{
	if (!isExternal(v)) treeFree(v->lChild);
	if (!isExternal(v)) treeFree(v->rChild);
	free(v); // 외부노드까지 모두 해제
}

// 이진탐색트리 메인메서드 --------------------------------------------------
int findElement(int k)
{
	Tree *v = treeSearch(root, k); // 검색
	
	if (isExternal(v))
	{
		noSuchKey = TRUE; // 검색결과가 외부노드이면 키는 존재하지 않음
		return -1;
	}
	else return v->key; // 내부노드이면 그 노드의 키(원소)를 반환
}

void insertItem(int k)
{
	Tree *v = treeSearch(root, k); // 검색

	if (!isExternal(v)) return; // 내부노드이면 이미 중복된 키가 존재하므로 예외
	else
	{
		v->key = k; // 외부노드이면 키 삽입 후
		expandExternal(v); // 내부노드화
	}
}

int removeElement(int k)
{
	Tree *v = treeSearch(root, k);
	int tmp = v->key;
	if (isExternal(v))
	{
		noSuchKey = TRUE; // 외부노드이면 삭제할 키가 없음
		return -1;
	}

	Tree *z = v->lChild;
	if (!isExternal(z)) z = v->rChild; // 왼쪽 자식이 내부이고
	if (isExternal(z)) // 오른쪽 자식이 외부이거나, 왼쪽이 외부이면 (둘 중 하나가 외부인 경우)
	{
		reduceExternal(z); // 외부노드 z에서 reduceExternal
	}
	else // 둘 다 내부인 경우
	{
		Tree *y = inOrderSucc(v); // 중위순회계승자를 y라 하고
		Tree *z = y->lChild; // y의 왼쪽자식을 z라 함

		v->key = y->key; // y의 내용(키)을 삭제할 노드로 복사
		reduceExternal(z); // 외부노드 z에서 reduceExternal
	}
	return tmp;
}



// main --------------------------------------------------
int main()
{
	char cInput = 0;
	int nInput = 0;
	int key = 0;
	int exit = FALSE;

	root = (Tree*)malloc(sizeof(Tree)); // 루트노드는 처음에 외부노드이므로 모든 연결점을 초기화
	root->parent = NULL;
	root->lChild = NULL;
	root->rChild = NULL;

	while (exit != TRUE)
	{
		scanf("%c", &cInput);
		switch (cInput)
		{
		case 'i':
			scanf("%d", &nInput);
			insertItem(nInput);

			break;
		case 'd':
			scanf("%d", &nInput);
			key = removeElement(nInput);
			if (noSuchKey == TRUE)
			{
				printf("X\n");
				noSuchKey = FALSE;
			}
			else printf("%d\n", key);

			break;
		case 's':
			scanf("%d", &nInput);
			key = findElement(nInput);
			if (noSuchKey == TRUE)
			{
				printf("X\n");
				noSuchKey = FALSE;
			}
			else printf("%d\n", key);

			break;
		case 'p':
			treePrintToPreOrder(root);
			printf("\n");

			break;
		case 'q':
			exit = TRUE; // 종료 플래그 on

			break;
		default:
			// 예외 처리 생략
			break;
		}
	}
	return 0;
}