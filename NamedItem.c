//
//  NamedItem.c
//  RPModules
//
//  Created by Joost Markerink on 16-01-17.
//  Copyright © 2017 Joost Markerink. All rights reserved.
//

#include "NamedItem.h"

typedef struct _NamedItem NamedItem;

struct _NamedItem{
    Item item;
    char name[64];
};


Item *createNamedItem(void *data,const char *name){
    NamedItem *item=(NamedItem *)createItem(data,sizeof(NamedItem));
    strcpy(item->name,name);
    return &item->item;
}


bool ItemMethod_findNamed(Item *itm,void *arg){
    return strcmp(((NamedItem *)itm)->name, (const char *)arg)==0;
}

Item *List_findNamedItem(List *list,const char *name){ 
    return List_loop(list, ItemMethod_findNamed,(void *)name); 
}
