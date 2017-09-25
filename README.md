# list
A generic and extendable linked list object in C.

This list contains a chain of Items, from item list->first to item list->last and keeps track of the length. The iteration object is used to iterate thru this chain.

It uses the common [free](https://linux.die.net/man/3/free) and [malloc](https://linux.die.net/man/3/malloc) functions to allocate and free dynamic memory. To replace these, #define MEMORY_H and implement memory_create(size) and memory_free(data)

To add any "data" to a list, it needs to act as an 'item', that means, it needs to be a 'struct' starting with a field of type 'item'.
The abstractitem type is defined so you do not need to convert item types all the time. 
You can just do ``` myitem *item=item_create();``` instead of having to do ``` myitem *item=(myitem *)item_create();```


## style
In function names, I do not use short names like info, enum, init but full words for clearity: information,enumerator and initialize. All words are lowercase with a dash between the words except for type names, they do not have dashes between words, 'abstractitem' for example.

Object Oriented functions always start with the object type name and finish with their function, like: list_create this way it's easier to remember how functions are named, and align nicely in the code layout.

The functions do as little as possible to keep it transparent and clear. I use the preprocessor as little as possible as it makes the structures unclear and too "magical".

## examples
list creation:
```c

typedef struct{
   item base;
   int value;
}integeritem;


list *mylist = list_create();

integeritem *myitem = item_create( sizeof(integeritem) );
myitem->value = 99;

list_append_item(mylist, myitem);

list_free(mylist);


```
list_free frees the list and its entire chain of items. 

list_remove_item does not free the item. It is only detached from the list. use the standard free(myitem); to destroy it
An item can only be added to one list. However, It can be detached from one and attached to the other.

to walk thru a list, use iteration:
```c

iteration i;

list_begin_iteration( mylist, &i );

while( iterate(&i) ){
   //do something with i.item
}


```
sorting (using the IntegerItem as shown in the List creation example):
```c

int integeritem_compare(const integeritem **a,const integeritem **b){
   return (*a)->value - (*b)->value;
}

list_sort(list, integeritem_compare);

```
