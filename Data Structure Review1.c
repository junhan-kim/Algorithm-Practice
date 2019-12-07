#include<stdio.h>
#include <stdlib.h>

typedef struct stLinkedList
{
	char elem;
	struct stLinkedList *prev;
	struct stLinkedList *next;
}List;

List *Add(List *p, int position, char item);
char Delete(List *p, int position);  
char Get_entry(List *p, int position);
void Print(List *p);
int ErrorInvalidPosition(int position, int n);

int n = 0;

int main()
{
	int processCount = 0;
	char processCapital = 0;
	int position = 0;
	char item = 0;
	List *head = (List*)malloc(sizeof(List));
	List *trailer = (List*)malloc(sizeof(List));

	head->next = trailer;
	trailer->prev = head;

	scanf("%d", &processCount);
	for (int i = 0; i < processCount; i++)
	{
		getchar();
		scanf("%c", &processCapital);
		switch (processCapital)
		{
		case 'A':
			getchar();
			scanf("%d", &position);
			getchar();
			scanf("%c", &item);

			if (ErrorInvalidPosition(position, n+1)) break;
			Add(head, position, item);
			break;
		case 'D':
			getchar();
			scanf("%d", &position);

			if (ErrorInvalidPosition(position, n)) break;
			Delete(head, position);
			break;
		case 'G':
			getchar();
			scanf("%d", &position);

			if (ErrorInvalidPosition(position, n)) break;
			printf("%c\n", Get_entry(head, position));
			break;
		case 'P':
			Print(head);
			break;
		}
	}

	while (head != trailer)
	{
		head = head->next;
		free(head->prev);
	}
	free(trailer);

    return 0;
}

List *Add(List *p, int position, char item)
{
	for (int i = 1; i <= position; i++)
	{
		p = p->next;
	}  

	List *node = (List*)malloc(sizeof(List));
	node->elem = item;
	node->prev = p->prev;
	node->next = p;
	(p->prev)->next = node;
	p->prev = node;
	n++;

	return node;
}

char Delete(List *p, int position)
{
	for (int i = 1; i <= position; i++)
	{
		p = p->next;
	}  

	char e = p->elem;
	(p->prev)->next = p->next;
	(p->next)->prev = p->prev;
	n--;

	free(p);
	return e;
}

char Get_entry(List *p, int position)
{
	for (int i = 1; i <= position; i++)
	{
		p = p->next;
	}  

	return p->elem;
}

void Print(List *p)
{
	for (int i = 0; i < n; i++)
	{
		p = p->next;
		printf("%c", p->elem);
	}
	printf("\n");
}

int ErrorInvalidPosition(int position, int n)
{
	if (position <= 0 || position > n)
	{
		printf("invalid position\n");
		return 1;
	}
	else return 0;
}