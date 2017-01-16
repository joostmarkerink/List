//
//  NamedItem.h
//  RPModules
//
//  Created by Joost Markerink on 16-01-17.
//  Copyright © 2017 Joost Markerink. All rights reserved.
//

#ifndef NamedItem_h
#define NamedItem_h

#include "List.h"

Item *  createNamedItem     (void *,const char *);  //create an empty item with name (of max. 63 characters)
Item *  List_findNamedItem  (List *,const char *);  //loops thru the list of named items until the item name is detected




#endif /* NamedItem_h */
