//
//  list.c
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

#include "list.h"



list *list_create(){
    list *list=memory_create(sizeof(list));
    list->first=list->last=NULL;
    list->length=0;
    return list;
}



void list_append_items(list *list,abstractitem **items,unsigned long numberOfitems){
    int i;
    item *first=items[0],
    *last=items[numberOfitems-1];

    for(i=1;i<numberOfitems;i++)
        ((item *)items[i-1])->next=((item *)items[i])->previous=items[i-1];

    if(list->last)  list->last->next=first;
    else list->first=first;

    list->last=last;
    list->length+=numberOfitems;

    list->last->next=NULL;
    list->first->previous=NULL;

}



void item_free(abstractitem *item){ free(item); }

void item_free_(list *list,abstractitem *item){ free(item); }

void list_free(list *l, void(*item_free)(list *,void *) ){
    if(!item_free) item_free=item_free;
    iteration e;
    list_begin_iteration(l,&e);
    while(iterate(&e)) item_free_(l,e.item);
    memory_free(l);
}




abstractitem *item_create(unsigned itemSize){
    item *i=memory_create(itemSize<sizeof(item)?sizeof(item):itemSize);
    i->previous=i->next=NULL;
    return i;
}




void list_append_item(list *list,abstractitem *ii){
    item *i=ii;
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


void list_remove_item(list *list,abstractitem *i){
    item *item=i;
    if(list->last==item)  list->last=item->previous;
    if(list->first==item) list->first=item->next;

    if(item->next) item->next->previous=item->previous;
    if(item->previous) item->previous->next=item->next;

    list->length--;
    item->next=item->previous=NULL;
}




int list_get_item_index(list *list,abstractitem *item){
    int offset=0;
    iteration e;
    list_begin_iteration(list,&e);
    while(iterate(&e)) if(e.item==item) return offset;
    return offset;
}



void list_insert_item(list *list,abstractitem *n,abstractitem *p,int after){
    item *point = (item *)p;
    item *newitem = (item *)n;
    if(!point) point=after?list->last:list->first;

    if(after){
        if(point==list->last){
            list_append_item(list, newitem);
            return;
        }else if(point->next){
            newitem->next = point->next;
            point->next->previous=newitem;
            newitem->previous=point;
            point->next=newitem;
        }
    }else{
        if(point){
            if(point->previous){
                point->previous->next=newitem;
                newitem->previous=point->previous;
            }else{
                if(list->last == list->first) list->last=newitem;
                list->first=newitem;
            }
            point->previous = newitem;
        }
        newitem->next=point;
    }
    list->length++;
}



void list_gather_items(list *list,abstractitem **items){
    iteration e;
    list_begin_iteration(list, &e);
    while(iterate(&e))
        *items++=e.item;
}


int iteration_next_item(iteration *e){
    e->item=e->next_item;
    if(e->item) e->next_item=((item *)e->item)->next;
    return e->item!=0;
}

void list_begin_iteration(list *list,iteration *e){
    e->item=0;
    e->next_item=list->first;
    e->next=iteration_next_item;
}


int iteration_reverse_next_item(iteration *e){
    e->item=e->next_item;
    if(e->item) e->next_item=((item *)e->item)->previous;
    return e->item!=0;
}

void list_begin_reversed_iteration(list *list,iteration *e){
    e->item=0;
    e->next_item=list->last;
    e->next=iteration_reverse_next_item;
}

int iterate(iteration *i){ return i->next(i); }


void list_sort(list *list,item_compare compare){
    unsigned long len=list->length;

    abstractitem **all=memory_create(len*sizeof(item *));

    list_gather_items(list, all);
    qsort(all, len, sizeof(item *),(int(*)(const void *,const void *))compare);
    list->length=0;
    list->first=list->last=NULL;
    list_append_items(list, all, len);
    memory_free(all);

}
