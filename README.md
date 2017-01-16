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
