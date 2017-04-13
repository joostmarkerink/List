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

List *List_create(){
    List *l=(List *)malloc(sizeof(List));
    l->first=l->last=NULL;
    l->length=0;
    return l;
}



void List_destroy(List *list){
    Iteration e;
    List_beginIteration(list,&e);
    while(iterate(e))
        free(e.item);

    free(list);
}

Item *Item_create(unsigned itemSize){
    Item *i=(Item *)malloc(itemSize<sizeof(Item)?sizeof(Item):itemSize);
    i->previous=i->next=NULL;
    return i;
}




Item *List_appendItem(List *list,Item *i){
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


void List_removeItem(List *list,Item *item){
    
    if(list->last==item)  list->last=item->previous;
    if(list->first==item) list->first=item->next;
    
    if(item->next) item->next->previous=item->previous;
    if(item->previous) item->previous->next=item->next;
    
    list->length--;
    item->next=item->previous=NULL;
}




int List_getItemIndex(List *list,Item *item){
    int offset=0;
    Iteration e;
    List_beginIteration(list,&e);
    while(iterate(e)) if(e.item==item) return offset;
    return offset;
}



void List_insert(List *list,Item *newItem,Item *point,int after){
    if(!point) point=after?list->last:list->first;

    if(after){
        if(point==list->last){
            List_appendItem(list, newItem);
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


void List_gatherItems(List *list,Item **items){
    Iteration e;
    List_beginIteration(list, &e);
    while(iterate(e))
        *items++=e.item;
}





int Iteration_nextItem(Iteration *e){
    e->item=e->nextItem;
    if(e->item) e->nextItem=e->item->next;
    return e->item!=0;
}

void List_beginIteration(List *list,Iteration *e){
    e->item=0;
    e->nextItem=list->first;
    e->next=Iteration_nextItem;
}


int Iteration_reverseNextItem(Iteration *e){
    e->item=e->nextItem;
    if(e->item) e->nextItem=e->item->previous;
    return e->item!=0;
}

void List_beginReversedIteration(List *list,Iteration *e){
    e->item=0;
    e->nextItem=list->last;
    e->next=Iteration_reverseNextItem;
}






