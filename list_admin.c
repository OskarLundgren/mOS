//
//  linkedlist.c
//  mOS DSTII
//
//  Created by Markus Axelsson and Oskar Lundgren on 2015-02-06.
//  Copyright (c) 2015 Markus & Oskar. All rights reserved.
//

#include "list_admin.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "main.h"
#include "memwatch.h"

/** Variables **/
list *g_readylist;
list *g_timerlist;
list *g_waitinglist;

/** Functions **/

list* create_list(void){
	list *new_list;
    new_list = malloc(sizeof(list));
    new_list->pHead = malloc( sizeof(listobj) );
    new_list->pTail = malloc( sizeof(listobj) );
    
    if(new_list == 0 || new_list->pHead == 0 || new_list->pTail == 0){
        free(new_list);
        free(new_list->pHead);
        free(new_list->pTail);
        return NULL;
    }
    else{
        new_list->pHead->pNext = new_list->pTail;
        new_list->pTail->pPrevious = new_list->pHead;
        return new_list;
    }
}

exception insert_timerlist(listobj *insert_object, int TC){
    insert_object->nTCnt = TC;
    if (g_timerlist->pHead->pNext == g_timerlist->pTail) {
        g_timerlist->pHead->pNext = insert_object;
        insert_object->pNext = g_timerlist->pTail;
		return OK;
        
    }
    else{
        listobj * tempObject;
        tempObject = g_timerlist->pHead;
        while (tempObject->pNext != 0) {
            if (tempObject->pNext->nTCnt > insert_object->nTCnt) {
                insert_object->pNext = tempObject->pNext;
                tempObject->pNext = insert_object;
				return OK;
            }
            tempObject = tempObject->pNext;
        }
        insert_object->pNext = g_timerlist->pTail;
        tempObject->pNext = insert_object;
		return OK;
    }
}

listobj * extract_timerlist(void){
    listobj * returnObject;
    if(g_timerlist->pHead->pNext == g_timerlist->pTail){
        exit(0);
    }
    else{
        returnObject = g_timerlist->pHead->pNext;
        g_timerlist->pHead->pNext = g_timerlist->pHead->pNext->pNext;
        return returnObject;
    }
}

exception insert_waiting_ready_list(list *list, listobj * object){
    //Checking if WAITING_LIST is empty
    if(list->pHead->pNext == list->pTail){
        list->pHead->pNext = object;
        list->pTail->pPrevious = object;
        object->pPrevious = list->pHead;
        object->pNext = list->pTail;
    }
    else{
        listobj *tempObject;
        tempObject = g_waitinglist->pHead;
        while ((tempObject->pNext != list->pTail) && (tempObject->pNext->pTask->DeadLine < object->pNext->pTask->DeadLine)) {
            tempObject = tempObject->pNext;
        }
        object->pNext = tempObject->pNext;
        object->pPrevious = tempObject;
        tempObject->pNext->pPrevious = object;
        tempObject->pNext = object;
    }
	return OK;
}

void extract_waitinglist(listobj *object){
    int found = 0;
    listobj *tempObject = g_waitinglist->pHead;
    while ((tempObject->pNext != g_waitinglist->pTail) && (found != 1)) {
        tempObject = tempObject->pNext;
        if (tempObject == object) {
			object->pPrevious->pNext = object->pNext;
			object->pNext->pPrevious = object->pPrevious;
			object->pNext = NULL;
			object->pPrevious = NULL;
        }
    }

}

listobj* extract_readylist(void){
    listobj * returnObject;
    if (g_readylist->pHead->pNext == g_readylist->pTail) {
        return NULL;
    }
    else{
        returnObject = g_readylist->pHead->pNext;
        g_readylist->pHead->pNext = returnObject->pNext;
        return returnObject;
    }
}

void delete_list(list **target){
	
	listobj *temp = (*target)->pHead;
	
	while(temp->pNext != (*target)->pTail){
	
		temp = temp->pNext;
		
		free(temp->pPrevious);
		
	}
	
	free((*target)->pTail);
	free(*target);
	*target = NULL;
	
	
	
	
}


