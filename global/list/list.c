#include "list.h"

typedef struct s_node {
	UC* data;
	int length;
	struct s_node *prev;
	struct s_node *next;
} Node;

struct s_list {
	int count;
	struct s_node *first;
};

List* list_create(void)
{
	List *l = (List*) malloc(sizeof(List));
	l->count = 0;
	l->first = NULL;
	return l;
}

errCO list_destroy(List *h)
{
	if (!h)
		return BAD_PARAMETERS;

	while (list_deleteByIndex(h, 0) != EMPTY_LIST)
		continue;

	free(h);
	return SUCCESS;
}

int list_getCount(List *h)
{ return h->count; }

static Node* node_create(UC* d, int l)
{
	Node *n = (Node*) malloc(sizeof(Node));
	n->data = (UC*) malloc(l);
	n->length = l;
	memcpy(n->data, d, l);

	n->prev = NULL;
	n->next = NULL;

	return n;
}

errCO list_addByIndex(List *h, UC *d, int l, int i)
{
	if (!h || !d || l < 1 )
		return BAD_PARAMETERS;

	if (i < -1 || i > h->count)
		return OUT_BOUND;

	Node *cur = h->first;
	Node *n = node_create(d, l);
	
	// First position in list	
	if (i == 0)
	{
		h->first = n;
		h->count += 1;

		if (cur)
		{
			n->next = cur;
			cur->prev = n;
		}

		return SUCCESS;
	}

	// Last position in list
	if (i == -1 || i == h->count)
	{
		while (cur->next)
			cur = cur->next;

		n->prev = cur;
 		cur->next = n;
 		h->count += 1;
 		return SUCCESS;
	}

	// Middle position in list
	for (int j = 0; j < i; j++)
		cur = cur->next;

	n->next = cur;
	n->prev = cur->prev;
	cur->prev->next = n;
	cur->prev = n;
 	h->count += 1;
 	return SUCCESS;
}

errCO list_addByData(List *h, UC *d, int l, fCMP f, fCMP c)
{
	if (!h || !d || l < 1 || !f || !c)
		return BAD_PARAMETERS;

	Node *cur = h->first;
	
	int i;
	for (i = 0; i < h->count; i++)
	{
		if (f(cur->data, d))
			break;

		cur = cur->next;
	}

	if (i == h->count || !c(cur->data, d))
	{
		list_addByIndex(h, d, l, i);
		return SUCCESS;
	}
	
	if (cur->length != l)
	{
		free(cur->data);
		cur->data = (UC*) malloc(l);
		cur->length = l;
	}
	memcpy(cur->data, d, l);
	return SUCCESS;
}

UC* list_getByIndex(List *h, int i)
{
	if (!h || i < -1 || i >= h->count)
		return NULL;

	if (h->count == 0)
		return NULL;

	if (i == -1)
		i = h->count-1;

	Node* cur = h->first;
	for (int j = 0; j < i; j++)
		cur = cur->next;

	return cur->data;
}

UC* list_getByData(List *h, UC *d, fCMP f)
{
	if (!h || !d || !f)
		return NULL;

	Node* cur = h->first;
	while (cur)
	{
		if (f(cur->data, d))
			return cur->data;

		cur = cur->next;
	}
	return NULL;
}

errCO list_modifyData(List* h, fCMP f, fMOD m)
{
	if (!h || !f || !m)
		return BAD_PARAMETERS;

	Node *cur = h->first;
	while (cur)
	{
		if (f(cur->data, NULL))
			m(cur->data);

		cur = cur->next;
	}

	return SUCCESS;
}

static void node_delete(Node *n)
{
	if (n)
	{
		free(n->data);

		Node *prev = n->prev;
		Node *next = n->next; 
		free(n);

		if (prev)
			prev->next = next;
		
		if (next)
			next->prev = prev;
	}
}

errCO list_extractByData(List *h, UC *d, fCMP f, UC *r)
{
	if (!h || !d || !f || !r)
		return BAD_PARAMETERS;

	Node *cur = h->first;
	for (int i = 0; i < h->count; i++)
	{
		if (f(cur->data, d))
		{
			if (i == 0)
				h->first = cur->next;

			memcpy(r, cur->data, cur->length);
			node_delete(cur);
			h->count -= 1;
			
			return SUCCESS;
		}
		cur = cur->next;
	}
	return NOT_FOUND;
}

errCO list_deleteByIndex(List *h, int i)
{
	if (!h)
		return BAD_PARAMETERS;

	if (h->count == 0)
		return EMPTY_LIST;

	if (i < -1 || i >= h->count)
		return OUT_BOUND;

	if (i == -1)
		i = h->count-1;

	Node *cur = h->first;
	for (int j = 0; j < i; j++)
		cur = cur->next;

	if (i == 0)
		h->first = cur->next;

	node_delete(cur);

	h->count -= 1;
	return SUCCESS;
}

errCO list_deleteByData(List *h, UC *d, fCMP f)
{
	if (!h || !f)
		return BAD_PARAMETERS;

	if (h->count == 0)
		return EMPTY_LIST;

	Node *tmp;
	Node *cur = h->first;
	
	int i = 0;
	while (cur)
	{
		if (f(cur->data, d))
		{
			tmp = cur->next;
			node_delete(cur);
			cur = tmp;
			h->count -= 1;

			if (i == 0)
				h->first = tmp;
		}
		else
		{
			cur = cur->next;
			i += 1;
		}
	}

	return SUCCESS;
}

void list_print(List *h)
{
	if (!h)
		return;

	Node *cur = h->first;
	printf("\n");
	if (!cur)
	{
		printf("No entries\n");
		printf("\n");
		return;
	}	
	
	int c = 0;
	while (cur)
	{
		printf("# %d:\n", c);
		memprint(cur->data, cur->length);
		printf("\n");
		cur = cur->next;
		c += 1;
	}
}