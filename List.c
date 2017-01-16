//
//  List.c

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

#include "List.h"
#include "string.h"


Item *List_loop(List *list,ItemMethod call,void *arg){
    
    Item *n,*item=list->first;
    while(item){
        n=item->next;
        if(!call(item,arg)) break;
        item=n;
    }
    return item;
}


List *createList(){
    List *l=(List *)malloc(sizeof(List));
    l->first=l->last=NULL;
    l->length=0;
    return l;
}

bool ItemMethod_destroyAllItems(Item *item,void *arg){
    free(item);
    return true;
}

void destroyList(List *list){
    List_loop(list, ItemMethod_destroyAllItems, list);
    free(list);
}

Item *createItem(size_t itemSize){
    Item *i=(Item *)malloc(itemSize==0?sizeof(Item):itemSize);
    i->previous=i->next=NULL;
    return i;
}

bool ItemMethod_findItemWithData(Item *item,void *data){ return item->data!=data; }


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


typedef struct{
    Item *toFind;
    off_t count;
}ItemMethod_getIndexOfItemData;

bool ItemMethod_getIndexOfItem(Item *item,void *arg){
    ((ItemMethod_getIndexOfItemData *)arg)->count++;
    return ((ItemMethod_getIndexOfItemData *)arg)->toFind!=item;
}


off_t List_getIndex(List *list,Item *item){
    ItemMethod_getIndexOfItemData data={item,0};
    if(List_loop(list,ItemMethod_getIndexOfItem,&data)==NULL) return -1;
    return data.count;
}



void List_insert(List *list,Item *newItem,Item *point,bool after){
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



typedef struct _ItemList ItemList_;

struct _ItemList{
    ItemList_ *parent;
    Item **items;
};


bool ItemMethod_getAllItems(Item *item,ItemList_ *arg){
    *arg->items++=item;
    return true;
}

void List_gatherItems(List *list,Item **items){
    static ItemList_ *currentItemList=NULL;
    ItemList_ c={currentItemList,items};
    currentItemList=&c;
    List_loop(list, (ItemMethod)ItemMethod_getAllItems, &c);
    currentItemList=c.parent;
}







