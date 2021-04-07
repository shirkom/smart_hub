#include "../libH/list_itr.h"
#include "../libH/list_struct.h"
#include <stdlib.h>

ListItr ListItrBegin(const List* _list)
{
	if(NULL == _list)
	{
		return NULL;
	}
	
	return _list->m_head.m_next;
}

ListItr ListItrEnd(const List* _list)
{
	if(NULL == _list)
	{
		return NULL;
	}
	
	return _list->m_tail.m_next;
}

int ListItrEquals(const ListItr _a, const ListItr _b)
{
	return _a == _b;
}

ListItr ListItrNext(ListItr _itr)
{
	if(NULL == _itr)
	{
		return NULL;
	}
	
	return ((Node*)_itr)->m_next;
}

ListItr ListItrPrev(ListItr _itr)
{
	if(NULL == _itr)
	{
		return NULL;
	}
	if(((Node*)_itr)->m_prev == ((Node*)_itr)->m_prev->m_prev)
	{
		return _itr;
	}
	return ((Node*)_itr)->m_prev;
}

void* ListItrGet(ListItr _itr)
{
	if(NULL == _itr || ((Node*)_itr)->m_next == ((Node*)_itr) || ((Node*)_itr)->m_prev == ((Node*)_itr))
	{
		return NULL;
	}
	
	return ((Node*)_itr)->m_data;
}

void* ListItrSet(ListItr _itr, void* _element)
{
	void* returnedValue;
	
	if(NULL == _itr || NULL == _element  || ((Node*)_itr)->m_next == ((Node*)_itr) || ((Node*)_itr)->m_prev == ((Node*)_itr))
	{
		return NULL;
	}
	
	returnedValue = ((Node*)_itr)->m_data;
	((Node*)_itr)->m_data = _element;
	
	return returnedValue;
}

ListItr ListItrInsertBefore(ListItr _itr, void* _element)
{
	Node *node;
	
	if(NULL == _itr || NULL == _element || ((Node*)_itr)->m_prev == ((Node*)_itr))
	{
		return NULL;
	}
	
	if (NULL == (node = (Node*) malloc(sizeof(Node))))
	{
		return NULL;
	}
	node->m_data = _element;
	node->m_next = ((Node*)_itr);
	node->m_prev = ((Node*)_itr)->m_prev;
	((Node*)_itr)->m_prev->m_next = node;
	((Node*)_itr)->m_prev = node;
	
	return node;
}

void* ListItrRemove(ListItr _itr)
{
	void* element;
	
	if(NULL == _itr|| ((Node*)_itr)->m_next == ((Node*)_itr) || ((Node*)_itr)->m_prev == ((Node*)_itr))
	{
		return NULL;
	}
	
	((Node*)_itr)->m_prev->m_next = ((Node*)_itr)->m_next;
	((Node*)_itr)->m_next->m_prev = ((Node*)_itr)->m_prev;
	element = ((Node*)_itr)->m_data;
	free(((Node*)_itr));
	
	return element;
}
