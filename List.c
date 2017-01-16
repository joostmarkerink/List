//
//  List.c
//  RPModules
//
//  Created by Joost Markerink on 09-01-17.
//  Copyright © 2017 Joost Markerink. All rights reserved.
//

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

Item *createItem(void *data,size_t itemSize){
    Item *i=(Item *)malloc(itemSize==0?sizeof(Item):itemSize);
    i->data=data;
    i->previous=i->next=NULL;
    return i;
}

bool ItemMethod_findItemWithData(Item *item,void *data){ return item->data!=data; }



Item *createNamedItem(void *data,const char *name){
    NamedItem *item=(NamedItem *)createItem(data,sizeof(NamedItem));
    strcpy(item->name,name);
    return &item->item;
}


bool ItemMethod_findNamed(Item *itm,void *arg){
    return strcmp(((NamedItem *)itm)->name, (const char *)arg)==0;
}

Item *List_findNamedItem(List *list,const char *name){ return List_loop(list, ItemMethod_findNamed,(void *)name); }

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







