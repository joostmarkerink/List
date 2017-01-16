//
//  List.c
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

#include "List.h"

List *createList(){
    List *l=(List *)malloc(sizeof(List));
    l->first=l->last=NULL;
    l->length=0;
    return l;
}



void destroyList(List *list){
    Iteration e;
    List_beginIteration(list,&e);
    Item *itm;
    while((itm = next(e)))
        free(itm);

    free(list);
}

Item *createItem(size_t itemSize){
    Item *i=(Item *)malloc(itemSize<sizeof(Item)?sizeof(Item):itemSize);
    i->data=NULL;
    i->previous=i->next=NULL;
    return i;
}



Item *List_findItem(List *list,const void *data){
    Iteration e;
    List_beginIteration(list, &e);
    Item *item;
    while((item=next(e))) if(item->data==data) return item;
    return NULL;
}


Item *List_append(List *list,Item *i){
    i->previous=list->last;
    i->next=NULL;
    if(list->last){
        list->last->next=i;
    }else{
        list->last=list->first=i;
    }
    list->last=i;
    list->length++;
    return i;
}


void List_remove(List *list,Item *item){
    
    if(list->last==item)  list->last=item->previous;
    if(list->first==item) list->first=item->next;
    
    if(item->next) item->next->previous=item->previous;
    if(item->previous) item->previous->next=item->next;
    
    list->length--;
    item->next=item->previous=NULL;
}


off_t List_getIndex(List *list,Item *item){
    off_t offset=0;
    Iteration e;
    List_beginIteration(list,&e);
    Item *itm;
    while((itm = next(e))) if(itm==item) return offset;
    return offset;
}


void List_insert(List *list,Item *newItem,Item *point,int after){
    if(!point) point=after?list->last:list->first;

    if(after){
        if(point==list->last){
            List_append(list, newItem);
            return;
        }else if(point->next){
            newItem->next = point->next;
            point->next->previous=newItem;
            newItem->previous=point;
            point->next=newItem;
        }
    }else{
        if(point){
            if(point->previous){
                point->previous->next=newItem;
                newItem->previous=point->previous;
            }else{
                if(list->last == list->first) list->last=newItem;
                list->first=newItem;
            }
            point->previous = newItem;
        }
        newItem->next=point;
    }
    list->length++;
}


bool ItemMethod_gatherData(Item *item,void *arg){
    static void **gatherDataList = NULL;
    if(!item->previous) gatherDataList=arg;
    *gatherDataList++=item->data;
    return true;
}

void List_gatherItems(List *list,Item **items){
    Iteration e;
    List_beginIteration(list, &e);
    Item *item;
    while((item=next(e)))
        *items++=item;
}




Item *Iteration_nextItem(Iteration *e){
    Item *res=e->item;
    if(res) e->item=res->next;
    return res;
}

void List_beginIteration(List *list,Iteration *e){
    e->item=list->first;
    e->next=Iteration_nextItem;
}

