//
//  list.h
/*
 MIT License

 Copyright (c) 2017 Joost Markerink

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef list_h
#define list_h


#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#define memory_create malloc
#define memory_free   free

#endif

typedef struct _list list;
typedef struct _item item;
typedef struct _iteration iteration;

typedef void abstractitem;

typedef int(*item_compare)(const item **a,const item **b);

struct _list{
    item *first,*last;
    size_t length;
};

struct _item{
    item *previous,*next;
};

struct _iteration{
    abstractitem *item;
    item *next_item;
    int(*next)(iteration *);
};

list *          list_create             (void);
//create an empty list

void            list_free               (list *,void(*item_free)(void *));
//free list and all its items. When item_free is NULL, it uses the regular "free(void *)" to free the items.

abstractitem *  item_create             (unsigned);
//create an empty item, 0 size refers to sizeof(item)

void            item_free               (abstractitem *);

void            list_append_item         (list *,abstractitem *);
//append item at the end of the list

void            list_append_items        (list *,abstractitem **,unsigned long);
//append a bunch of items

void            list_remove_item         (list *,abstractitem *);
//detach an item from the list (use free(item) to destroy it)

int             list_get_item_index       (list *,abstractitem *);
//returns the offset of item or -1 if it is not in the list

void            list_insert_item         (list *,abstractitem *toAdd,abstractitem *point,int after);
//insert an item before or after (after = 0/1) an existing item in the list

void            list_gather_items        (list *,abstractitem **     );
//puts all items in the items buffer ( item *itemsBuffer[list->length]; )

void            list_begin_iteration        (list *,iteration *);
//initializes the iteration

void            list_begin_reversed_iteration(list *,iteration *);
//initializes the iteration in reversed order

void            list_sort                  (list *,item_compare);
//uses the standard qsort function to sort its chain of items

int iterate(iteration *);
// perform the next step in the interation. returns 1 if the iteration.item is available,0 when the iteration is finished

#endif /* list_h */
