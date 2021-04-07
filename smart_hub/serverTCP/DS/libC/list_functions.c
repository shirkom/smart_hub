#include "../libH/list_itr.h"
#include "../libH/list_functions.h"
#include <stddef.h>

typedef struct
{
    ListItr m_begin;
    ListItr m_end;
    int (*m_checkFunc)(void * _element, void* _context);
    void* m_context;
} LoopParameters;

static ListItr  GenericForEach(LoopParameters _parameters, int (*_innerFunction)(int, void*), void* _innerContext);
static int ChangeIfZero(int _changed, void* _context);
static int InnerCount(int _changed, void* _count);
static int InnerReturnSame(int _changed, void* _context);

ListItr ListItrFindFirst(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
    LoopParameters parameters;

    if (NULL == _begin || NULL == _end || NULL == _predicate || NULL == _context)
    {
        return NULL;
    }

    parameters.m_begin = _begin;
    parameters.m_end = _end;
    parameters.m_checkFunc = _predicate;
    parameters.m_context = _context;
    return GenericForEach(parameters, ChangeIfZero, NULL);
}

size_t ListItrCountIf(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
    size_t i = 0;
    LoopParameters parameters;

    if (NULL == _begin || NULL == _end || NULL == _predicate|| NULL == _context)
    {
        return 0;
    }

    parameters.m_begin = _begin;
    parameters.m_end = _end;
    parameters.m_checkFunc = _predicate;
    parameters.m_context = _context;
    GenericForEach(parameters, InnerCount, &i);

    return i;
}

ListItr ListItrForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context)
{
    LoopParameters parameters;

    if (NULL == _begin || NULL == _end || NULL == _action)
    {
        return NULL;
    }  

    parameters.m_begin = _begin;
    parameters.m_end = _end;
    parameters.m_checkFunc = _action;
    parameters.m_context = _context;
    return GenericForEach(parameters, InnerReturnSame, NULL);
}

static int InnerReturnSame(int _changed, void* _context)
{
    if(_context == NULL)
    {}
    return _changed;
}

static int ChangeIfZero(int _changed, void* _context)
{
    if(_context == NULL)
    {}
    if (_changed == 0)
    {
        return 1;
    }

    return 0;
}

static int InnerCount(int _changed, void* _count)
{
    if (_changed != 0)
    {
    	++(*(size_t*)_count);  
    }

    return 1;
}

static ListItr  GenericForEach(LoopParameters _parameters, int (*_innerFunction)(int, void*), void* _innerContext)
{
    ListItr node;

    node = _parameters.m_begin;
    while (node != _parameters.m_end && ListItrNext(node) != node)
    {
        if (_innerFunction(_parameters.m_checkFunc(ListItrGet(node), _parameters.m_context), _innerContext) == 0)
        {   
            break;
        }
        node = ListItrNext(node);
    }

    return node;
}
