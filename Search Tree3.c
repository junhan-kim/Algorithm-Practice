#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct Tree
{
	int height;
	int key;
	struct Tree *parent;
	struct Tree *lChild;
	struct Tree *rChild;
}Tree;

Tree *root; // 전역 루트노드
int noSuchKey = FALSE;

// 기타 메서드 --------------------------------------------------
int abs(int n) // 절댓값 반환
{
	if (n < 0) return -1 * n;
	else return n;
}

int max(int a, int b) // 큰 값 반환
{
	if (a > b) return a;
	else return b;
}

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
	l->height = 0; // height의 갱신이 추가됨
	r->parent = v;
	r->lChild = NULL;
	r->rChild = NULL;
	r->height = 0;
	v->lChild = l;
	v->rChild = r;
	v->height = 1;
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

int height(Tree *v)
{
	if (isExternal(v)) return 0; // 외부노드는 높이 0
	else
	{
		return 1 + max(height(v->lChild), height(v->rChild)); // v의 높이는 (1 + v의 자식중 높은 노드의 높이)
	}
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

int updateHeight(Tree *v)
{
	v->height = height(v); // 재귀함수 height의 결과를 노드에 대입
	return TRUE;
}

int isBalanced(Tree *v)
{
	if (abs(v->lChild->height - v->rChild->height) > 1) return FALSE; // 왼쪽과 오른쪽 자식의 높이 차가 1을 초과하면 불균형
	else return TRUE;
}

Tree *restructure(Tree *x, Tree *y, Tree *z)
{
	Tree *a = NULL;
	Tree *b = NULL;
	Tree *c = NULL;
	Tree *T0 = NULL;
	Tree *T1 = NULL;
	Tree *T2 = NULL;
	Tree *T3 = NULL;
	Tree *bigger = NULL;
	Tree *smaller = NULL;

	// x,y,z를 a,b,c에 오름차순으로 대입 (AVR트리에서 키의 오름차순 = 중위순회)
	if (x->key > y->key) { bigger = x;  smaller = y; }
	else { bigger = y; smaller = x; }
	if (z->key < bigger->key)
	{
		if (z->key > smaller->key) { a = smaller; b = z; c = bigger; }
		else { a = z; b = smaller; c = bigger; }
	}
	else { a = smaller; b = bigger; c = z; }

	T0 = a->lChild; // T0와 T3는 항상 a 왼쪽자식, c 오른쪽자식임
	T3 = c->rChild;
	if ((y == z->rChild) && (x == y->rChild)) // T1과 T2는 x,y,z의 왼쪽,오른쪽 자식 관계에 의한 조건들로 인해 결정됨.
											  // 단일, 이중회전의 총 네가지 케이스를 참고하여 조건식을 세움
	{
		T1 = y->lChild;
		T2 = x->lChild;
	}
	else if ((y == z->lChild) && (x == y->lChild))
	{
		T1 = x->rChild;
		T2 = y->rChild;
	}
	else if ((y == z->rChild) && (x == y->lChild) || (y == z->lChild) && (x == y->rChild))
	{
		T1 = x->lChild;
		T2 = x->rChild;
	}

	// 부트리의 루트 주소를 갱신 => 부트리가 옮겨지는 것과 같음

	// z를 b로 대체
	if (z == root)
	{
		root = b; // z = root일땐 루트에다 직접 대체
		b->parent = NULL;
	}
	else if (z == z->parent->lChild) // 그 외엔 특정 노드의 자식이므로 왼쪽or오른쪽 자식이냐에 따라 대체
	{
		z->parent->lChild = b;
		b->parent = z->parent;
	}
	else if (z == z->parent->rChild)
	{
		z->parent->rChild = b;
		b->parent = z->parent;
	}

	// a의 왼쪽과 오른쪽 자식에 T0와 T1을 대입
	a->lChild = T0;
	T0->parent = a;
	a->rChild = T1;
	T1->parent = a;
	updateHeight(a);

	// c의 왼쪽과 오른쪽 자식에 T2와 T3을 대입
	c->lChild = T2;
	T2->parent = c;
	c->rChild = T3;
	T3->parent = c;
	updateHeight(c);

	// b의 왼쪽과 오른쪽 자식에 a와 c을 대입
	b->lChild = a;
	a->parent = b;
	b->rChild = c;
	c->parent = b;
	updateHeight(b);

	return b; // restructure 작업을 통해 가장 높은 노드가 된 b를 반환
}

void searchAndFixAfterInsertion(Tree *v)
{
	Tree *x = NULL;
	Tree *y = NULL;
	Tree *z = NULL;

	v->lChild->height = 0; // 삽입은 외부로부터 확장된 높이 1의 내부노드에서 수행하므로
	v->rChild->height = 0;
	v->height = 1;

	while (updateHeight(v) && isBalanced(v)) // v의 height을 업데이트하면서 균형 여부 체크
	{
		v = v->parent; // 계속 위로 올라감
		if (v == NULL) return; // 부모노드의 parent까지 도달했을땐 균형이므로 끝
	}
	z = v; // 불균형 노드를 z로 갱신

	if (z->lChild->height > z->rChild->height)	y = z->lChild; // z의 높은 자식을 y로 갱신
	else y = z->rChild;

	if (y->lChild->height > y->rChild->height)	x = y->lChild; // y의 높은 자식을 x로 갱신
	else x = y->rChild;

	restructure(x, y, z); // 개조

	return;
}

void searchAndFixAfterRemoval(Tree *v)
{
	Tree *x = NULL;
	Tree *y = NULL;
	Tree *z = NULL;
	Tree *b = NULL;

    if (v == NULL) return; // searchAndFixAfterRemoval의 베이스

	z = v;
	while (updateHeight(z) && isBalanced(z)) // v의 height을 업데이트하면서 균형 여부 체크
	{
		if (z == root) return;
		z = z->parent; // 계속 위로 올라감
	}
	if (isBalanced(z)) return;

	if (z->lChild->height > z->rChild->height)	y = z->lChild; // z의 높은 자식을 y로 갱신
	else y = z->rChild;

	if (y->lChild->height > y->rChild->height)	x = y->lChild; // y의 높은 자식을 x로 갱신
	else if (y->lChild->height < y->rChild->height)	x = y->rChild;
	else if (y->lChild->height == y->rChild->height)// y 자식의 높이가 서로 같을 땐 z-y의 관계에 따름
	{
		if (y == z->lChild) { x = y->lChild; }
		else if (y == z->rChild) { x = y->rChild; }
	}
	
	b = restructure(x, y, z);
	searchAndFixAfterRemoval(b->parent); // 개조가 수행된 b 부트리의 균형속성이 지역적으로 한정되므로
	                                     // 부모로 재귀하며, 전역적 균형속성을 회복하도록 함
	                                     // 베이스케이스는 이미 균형체크 부분에 작성되어 있음
}

// AVR트리 메인메서드 --------------------------------------------------
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
		searchAndFixAfterInsertion(v);
	}
}

int removeElement(int k)
{
	Tree *v = treeSearch(root, k);
	Tree *zs = NULL;

	if (isExternal(v))
	{
		noSuchKey = TRUE; // 외부노드이면 삭제할 키가 없음
		return -1;
	}

	Tree *z = v->lChild;
	if (!isExternal(z)) z = v->rChild; // 왼쪽 자식이 내부이고
	if (isExternal(z)) // 오른쪽 자식이 외부이거나, 왼쪽이 외부이면 (둘 중 하나가 외부인 경우)
	{
		zs = reduceExternal(z); // 외부노드 z에서 reduceExternal하여 zs(z의 부모인 v의 자리를 대체)를 반환
	}
	else // 둘 다 내부인 경우
	{
		Tree *y = inOrderSucc(v); // 중위순회계승자를 y라 하고
		Tree *z = y->lChild; // y의 왼쪽자식을 z라 함

		v->key = y->key; // y의 내용(키)을 삭제할 노드로 복사
		zs = reduceExternal(z); // 외부노드 z에서 reduceExternal하여 zs(z의 부모인 v의 자리를 대체)를 반환
	}

	searchAndFixAfterRemoval(zs->parent); // v가 삭제되었으므로 대체된 zs의 부모를 인자로하여 복구작업 수행
	return k; // 삭제된 키를 반환
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

	treeFree(root);
	return 0;
}