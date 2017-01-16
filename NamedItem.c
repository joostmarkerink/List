//
//  NamedItem.c
//  List

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

#include "NamedItem.h"

typedef struct _NamedItem NamedItem;

struct _NamedItem{
    Item item;
    char name[64];
};


Item *createNamedItem(const char *name){
    NamedItem *item=(NamedItem *)createItem(sizeof(NamedItem));
    strcpy(item->name,name);
    return &item->item;
}


bool ItemMethod_findNamed(Item *itm,void *arg){
    return strcmp(((NamedItem *)itm)->name, (const char *)arg)==0;
}

Item *List_findNamedItem(List *list,const char *name){ 
    return List_loop(list, ItemMethod_findNamed,(void *)name); 
}
