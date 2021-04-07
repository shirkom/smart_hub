#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>  /* size_t */

typedef struct List List;

typedef enum ListResult {
	LIST_SUCCESS,
	LIST_UNINITIALIZED_ERROR,			/**< Uninitialized list 					 	*/
	LIST_ALLOCATION_ERROR,				/**< Node allocation failed due to heap error   */
	LIST_NULL_ELEMENT_ERROR,
	LIST_UNINITIALIZED_ITEM_ERROR,
	LIST_UNDERFLOW_ERROR
	/* Add more as needed by your implementation */
} ListResult;

/*
description: allocates a list 
input:
output: list ot null if allocation problem
*/
List* ListCreate(void);

/*
description: deallocates the list and every item in it changes list to null
input: the list and the way you want ot destroy each element (can be NULL)
output: none 
*/
void ListDestroy(List** _list, void (*_elementDestroy)(void* _item));

/*
Description:Pushes given item to head of the list
Input:Pointer to list and the data you want to push
Output:sucsses or error
Errors: LIST_UNINITIALIZED_ERROR if list is null  LIST_UNINITIALIZED_ITEM_ERROR if item is null 
LIST_ALLOCATION_ERROR if couldn't add space for new data
*/
ListResult ListPushHead(List* _list, void* _item);

/*
Description:Pushes given item to tail of the list
Input:Pointer to list and the data you want to push
Output:sucsses or error
Errors: LIST_UNINITIALIZED_ERROR if list is null  LIST_UNINITIALIZED_ITEM_ERROR if item is null 
LIST_ALLOCATION_ERROR if couldn't add space for new data
*/
ListResult ListPushTail(List* _list, void* _item);

/*
Description:Removes data from the head of the list and puts it in *item
Input:Pointer to list and where you want to put the taken data
Output:sucsses or error
Errors: LIST_UNINITIALIZED_ERROR if list is null LIST_UNINITIALIZED_ITEM_ERROR if item is null  LIST_UNDERFLOW_ERROR if list is empty
*/
ListResult ListPopHead(List* _list, void** _pItem);

/*
Description:Removes data from the tail of the list and puts it in *item
Input:Pointer to list and where you want to put the taken data
Output:sucsses or error
Errors: LIST_UNINITIALIZED_ERROR if list is null LIST_UNINITIALIZED_ITEM_ERROR if item is null  LIST_UNDERFLOW_ERROR if list is empty
*/
ListResult ListPopTail(List* _list, void** _pItem);

/*
Description:Counts the amount of items in the list O(n)
Input:Pointer to list 
Output: The amount of items in the list
Errors: 0 If LIST_UNINITIALIZED_ERROR 
*/
size_t ListSize(const List* _list);


#endif /* __LIST_H__ */
