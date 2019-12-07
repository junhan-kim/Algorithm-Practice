#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Tree {
	int id;
	struct Tree *left;
	struct Tree *right;
}Tree;

bool isInternal(Tree *v);
void binaryPreOrder(Tree *v, int id, Tree **out);
int Strlen(char *string);
void Free(Tree *v);

int main()
{
	int n = 0;
	int input[3] = { 0, };
	int id = 0;
	int left = 0;
	int right = 0;
	Tree *tree = NULL;

	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		Tree *out = NULL;

		scanf("%d", &id);
		scanf("%d", &left);
		scanf("%d", &right);

		if(tree != NULL) binaryPreOrder(tree, id, &out);

		Tree *p = NULL;
		if (out != NULL) p = out;
		else
		{
			p = (Tree*)malloc(sizeof(Tree));
			p->id = id;
			p->left = NULL;
			p->right = NULL;
			if (i == 0) tree = p;
		}
		if (left != 0)
		{
			Tree *pLeft = (Tree*)malloc(sizeof(Tree));
			p->left = pLeft;
			pLeft->id = left;
			pLeft->left = NULL;
			pLeft->right = NULL;
		}
		if (right != 0)
		{
			Tree *pRight = (Tree*)malloc(sizeof(Tree));
			p->right = pRight;
			pRight->id = right;
			pRight->left = NULL;
			pRight->right = NULL;
		}
	}

	int s = 0;
	scanf("%d", &s);
	getchar();
	int result[10] = { 0, };

	for (int i = 0; i < s; i++)
	{
		Tree *v = tree;
		char info = 0;
		int j = 0;

		result[j] = (v->id);
		while (info != '\n')
		{
			info = getchar();
			if (info == 'L')
			{
				v = (v->left);
			}
			else if (info == 'R')
			{
				v = (v->right);
			}
			else break;
			j++;
			result[j] = (v->id);
		}
		for (int k = 0; k < (j + 1); k++)
		{
			printf(" %d", result[k]);
		}
		printf("\n");
	}

	Free(tree);
    return 0;
}

bool isInternal(Tree *v)
{
	return ((v->left) != NULL) || ((v->right) != NULL);
}

void binaryPreOrder(Tree *v, int id, Tree **out)
{
	if ((v->id) == id) *out = v;
	if (isInternal(v))
	{
		if ((v->left) != NULL) binaryPreOrder((v->left), id, out);
		if ((v->right) != NULL) binaryPreOrder((v->right), id, out);
	}
}

void Free(Tree *v)
{
	if (isInternal(v))
	{
		if ((v->left) != NULL) Free(v->left);
		if ((v->right) != NULL) Free(v->right);
	}
	free(v);
}

int Strlen(char *string)
{
	int cnt = 0;
	while (string[cnt] != '\0')
	{
		++cnt;
	}
	return cnt;
}
