# List
A generic and extendable linked list object in C.

It uses the "C Standard General Utilities Library" to allocate and free dynamic memory.

NamedItem is an extension example.
It allows to attach names to items (max 64 characters).

## Style
When I use the word "data" in the List context, I refer to the "void *" pointer in an Item as it can be of any size or type.
The functions do as little as possible to keep it transparent and clear. 

basic example:
```c

List *myList = createList();

Item *myItem = createItem(sizeof(Item));
myItem->data="Some data";

List_append(myList,myItem);

destroyList(myList);


```
destroyList frees the list and all its items. 
It does not touch the data.
When the items data needs to be freed, loop thru the items to delete its data, before calling destroyList

List_remove does not destroy the Item. It is only detached from the list. use the standard free(myItem); to destroy it
An Item can only be added to one list. However, It can be detached from one and attached to the other.

List_loop is a powerfull function. It can just be used to walk thru the entire list. It can also be used to find anything that is in the data. When the user defined method returns false on a certain Item, the loop will 'break' and return this Item.
Any other List functions themself also use List_loop whenever they need to loop the list.
