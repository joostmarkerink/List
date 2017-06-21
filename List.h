//
//  List.h
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

#ifndef List_h
#define List_h

#include <stdlib.h>

#ifndef MEMORY_H
#define MEMORY_H
Memory_create malloc
Memory_free   free
#endif

typedef struct _List List;
typedef struct _Item Item;
typedef struct _Iteration Iteration;

typedef void AbstractItem;

typedef int(*Item_compare)(const Item **a,const Item **b);

struct _List{
    Item *first,*last;
    size_t length;
};

struct _Item{
    Item *previous,*next;
};

struct _Iteration{
    AbstractItem *item;
    Item *nextItem;
    int(*next)(Iteration *);
};

List *          List_create             (void);
//create an empty list 

void            List_free               (List *);
//destroy list and all its items (does not touch the data)

AbstractItem *  Item_create             (unsigned);
//create an empty item, 0 size refers to sizeof(Item)

void            Item_free               (AbstractItem *);

void            List_appendItem         (List *,AbstractItem *);
//append item at the end of the list

void            List_appendItems        (List *,AbstractItem **,unsigned long);
//append a bunch of items

void            List_removeItem         (List *,AbstractItem *);
//detach an item from the list (use free(item) to destroy it)

int             List_getItemIndex       (List *,AbstractItem *);
//returns the offset of item or -1 if it is not in the list

void            List_insertItem         (List *,AbstractItem *toAdd,Item *point,int after); 
//insert an item before or after (after = 0/1) an existing item in the list

void            List_gatherItems        (List *,AbstractItem **     );
//puts all items in the Items buffer ( Item *itemsBuffer[list->length]; )

void            List_beginIteration        (List *,Iteration *);
//initializes the Iteration

void            List_beginReversedIteration(List *,Iteration *);
//initializes the Iteration in reversed order

void            List_sort                  (List *,Item_compare);
//uses the standard qsort function to sort its chain of items

int iterate(Iteration *);
// perform the next step in the interation. returns 1 if the Iteration.item is available,0 when the iteration is finished

#endif /* List_h */
