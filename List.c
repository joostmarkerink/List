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
    List *l=Memory_create(sizeof(List));
    l->first=l->last=NULL;
    l->length=0;
    return l;
}



void List_appendItems(List *list,AbstractItem **items,unsigned long numberOfItems){
    int i;
    Item *first=items[0],
    *last=items[numberOfItems-1];
    
    for(i=1;i<numberOfItems;i++)
        ((Item *)items[i-1])->next=((Item *)items[i])->previous=items[i-1];
    
    if(list->last)  list->last->next=first; 
    else list->first=first;
    
    list->last=last;
    list->length+=numberOfItems;
    
    list->last->next=NULL;
    list->first->previous=NULL;

}



void List_free(List *list){
    Iteration e;
    List_beginIteration(list,&e);
    while(iterate(&e)) Item_free(e.item);
    Memory_free(list);
}




AbstractItem *Item_create(unsigned itemSize){
    Item *i=Memory_create(itemSize<sizeof(Item)?sizeof(Item):itemSize);
    i->previous=i->next=NULL;
    return i;
}



 
void List_appendItem(List *list,AbstractItem *ii){
    Item *i=ii;
    i->previous=list->last;
    i->next=NULL;
    if(list->last){
        list->last->next=i;
    }else{
        list->last=list->first=i;
    }
    list->last=i;
    list->length++;
}


void List_removeItem(List *list,AbstractItem *i){
    Item *item=(Item *)i;
    if(list->last==item)  list->last=item->previous;
    if(list->first==item) list->first=item->next;
    
    if(item->next) item->next->previous=item->previous;
    if(item->previous) item->previous->next=item->next;
    
    list->length--;
    item->next=item->previous=NULL;
}




int List_getItemIndex(List *list,AbstractItem *item){
    int offset=0;
    Iteration e;
    List_beginIteration(list,&e);
    while(iterate(&e)) if(e.item==item) return offset;
    return offset;
}



void List_insert(List *list,AbstractItem *n,AbstractItem *p,int after){
    Item *point = (Item *)p;
    Item *newItem = (Item *)n;
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



void List_gatherItems(List *list,AbstractItem **items){
    Iteration e;
    List_beginIteration(list, &e);
    while(iterate(&e))
        *items++=e.item;
}


int Iteration_nextItem(Iteration *e){
    e->item=e->nextItem;
    if(e->item) e->nextItem=((Item *)e->item)->next;
    return e->item!=0;
}

void List_beginIteration(List *list,Iteration *e){
    e->item=0;
    e->nextItem=list->first;
    e->next=Iteration_nextItem;
}


int Iteration_reverseNextItem(Iteration *e){
    e->item=e->nextItem;
    if(e->item) e->nextItem=((Item *)e->item)->previous;
    return e->item!=0;
}

void List_beginReversedIteration(List *list,Iteration *e){
    e->item=0;
    e->nextItem=list->last;
    e->next=Iteration_reverseNextItem;
}

int iterate(Iteration *i){ return i->next(i); }


void List_sort(List *list,Item_compare compare){
    unsigned long len=list->length;

    AbstractItem **all=Memory_create(len*sizeof(Item *));
    
    List_gatherItems(list, all);
    qsort(all, len, sizeof(Item *),(int(*)(const void *,const void *))compare);
    list->length=0;
    list->first=list->last=NULL;
    List_appendItems(list, all, len);
    Memory_free(all);
    
}





