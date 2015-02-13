//
//  linkedlist.h
//  mOS DSTII
//
//  Created by Markus Axelsson and Oskar Lundgren  on 2015-02-06.
//  Copyright (c) 2015 Markus & Oskar. All rights reserved.
//

#ifndef __mOS_DSTII__linkedlist__
#define __mOS_DSTII__linkedlist__

#include <stdio.h>
#include "kernel.h"

/** Definitions **/
extern list *g_readylist;
extern list *g_timerlist;
extern list *g_waitinglist;

/** Functions **/
list* create_list(void);


/*Inserting objects in lists*/
int insert_timerlist(listobj *insert_object, int nTCnt);
int insert_waiting_ready_list(list *list, listobj *insert_object);

/*Extracting objects from lists*/

exception extract_waitinglist(listobj *object);
listobj* extract_ready_timer_list(list* list);


#endif /* defined(__mOS_DSTII__linkedlist__) */
