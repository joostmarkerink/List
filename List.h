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
#include <stdbool.h>

typedef struct _List List;
typedef struct _Item Item;
typedef struct _Iteration Iteration;

struct _List{
    Item *first,*last;
    size_t length;
};

struct _Item{
    Item *previous,*next;
    void *data;
};

struct _Iteration{
    Item *item;
    Item *(*next)(Iteration *);
};

#define iterate(e_n_u_m) e.next(&e_n_u_m)

List *  createList          (void);                                         //create an empty list 
void    destroyList         (List *);

Item *  createItem          (size_t);                                //create an empty item, 0 size refers to sizeof(Item)

Item *  List_append         (List *,Item *);                                //append item at the end of the list
void    List_remove         (List *,Item *);                                //detach an item from the list (use free(item) to destroy it)
off_t   List_getIndex       (List *,Item *);                                //returns the offset of item or -1 if it is not in the list
void    List_insert         (List *,Item *toAdd,Item *point,int after);    //insert an item before or after (after = 0/1) an existing item in the list

Item *  List_findItem       (List *,const void *);
void    List_gatherItems    (List *,Item **     );                      //puts all items in the Items buffer ( Item *itemsBuffer[list->length]; )

void List_beginIteration         (List *,Iteration *);
void List_beginReversedIteration (List *,Iteration *);

#endif /* List_h */
