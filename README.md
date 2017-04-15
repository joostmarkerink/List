# List
A generic and extendable linked list object in C.

This List contains a chain of Items, from Item list->first to Item list->last and keeps track of the length. The Iteration object is used to iterate thru this chain.

It uses the common [free](https://linux.die.net/man/3/free) and [malloc](https://linux.die.net/man/3/malloc) functions to allocate and free dynamic memory.

To add any "data" to a List, it needs to act as an 'Item', that means, it needs to be a 'struct' starting with a field of type 'Item'.

NamedItem is an extension example.
It allows to attach names to items (max 64 characters). So you can say, it changes a List into a Record.

## Style
In function names, I do not use short names like info, enum, init but full words for clearity: information,enumerator and initialize.

Object Oriented functions always start with the Object type name and finish with their function, like: List_create this way it's easier to remember how functions are named, and align nicely in the code layout.

When I use the word "data" in the List context, I refer to the "void *" pointer in an Item as it can be of any size or type.
The functions do as little as possible to keep it transparent and clear. 

## examples
list creation:
```c

typedef struct{
   Item base;
   int value;
}IntegerItem;


List *myList = List_create();

IntegerItem *myItem = (IntegerItem *)Item_create( sizeof(IntegerItem) );
myItem->value = 99;

List_appendItem(myList, &myItem->base);

List_destroy(myList);


```
List_destroy frees the list and its chain of items. 

List_removeItem does not destroy the Item. It is only detached from the list. use the standard free(myItem); to destroy it
An Item can only be added to one list. However, It can be detached from one and attached to the other.

to walk thru a list, use Iteration:
```c

Iteration i;

List_beginIteration( myList, &i );

while( iterate(i) ){
   //do something with i.item
}


```
sorting (using the IntegerItem as showed in the List creation example):
```c

int IntegerItem_compare(const IntegerItem **a,const IntegerItem **b){
   return (*a)->value-(*b)->value;
}

List_sort(list,IntegerItem_compare);

```
