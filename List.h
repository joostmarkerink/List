//
//  List.h
//
/*
MIT License

Copyright © 2017 Joost Markerink

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
typedef struct _NamedItem NamedItem;

struct _List{
    Item *first,*last;
    size_t length;
};

struct _Item{
    Item *previous,*next;
    void *data;
};

struct _NamedItem{
    Item item;
    char name[64];
};

typedef bool(*ItemMethod)(Item *,void *);                                   //method for use with List_findItem

bool    ItemMethod_findItemWithData (Item *,void *);                        //method to find the item in a list that contains the given data (use with List_loop)



List *  createList          (void);                                         //create an empty list 
void    destroyList         (List *);

Item *  createItem          (size_t);                                       //create an empty floating item, 0 size refers to sizeof(Item)

Item *  List_append         (List *,Item *);                                //append item at the end of the list
void    List_remove         (List *,Item *);                                //detach an item from the list (use free(item) to destroy it)
off_t   List_getIndex       (List *,Item *);                                //returns the offset of item or -1 if it is not in the list
void    List_insert         (List *,Item *toAdd,Item *point,bool after);    //insert an item before or after an existing item in the list

Item *  List_loop           (List *,ItemMethod,void *arg);                  //loops thru the list of items until the method returns false (method NULL will compare the items data)

void    List_gatherItems    (List *list,Item **items);                      //puts all items in the Items buffer ( Item *itemsBuffer[list->length]; )








#endif /* List_h */
