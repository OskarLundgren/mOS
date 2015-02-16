//
//  timing.c
//  mOS
//
//  Created by Markus Axelsson on 2015-02-10.
//  Copyright (c) 2015 Markus & Oskar. All rights reserved.
//

#include "timing.h"
#include "main.h"
#include "kernel.h"
#include "list_admin.h"
#include "memwatch.h"

int w_firstrun = TRUE;
int sd_firstrun = TRUE;

exception wait1( uint nTicks ){
    int status = DEADLINE_REACHED;
    listobj * tempObj;
    isr_off();
    SaveContext();
    if (w_firstrun) {
        w_firstrun = FALSE;
        tempObj = extract_ready_timer_list(g_readylist);
        tempObj->nTCnt = nTicks + TC;
        insert_timerlist(tempObj, tempObj->nTCnt);
        LoadContext();
    }
    else{
        if (TC >= g_readylist->pHead->pNext->pTask->DeadLine) {
            status=DEADLINE_REACHED;
        }
        else{
            status = OK;
        }
    }
    return status;
}

void set_ticks(uint nTicks){
    TC = nTicks;
}

uint ticks(void){
    return TC;
}

uint deadline(void){
    return g_readylist->pHead->pTask->DeadLine;
}

void set_deadline( uint nNew ){
    volatile int firstrun = TRUE;
    isr_off();
    SaveContext();
    if (sd_firstrun) {
        sd_firstrun = FALSE;
        g_readylist->pHead->pNext->pTask->DeadLine = nNew;
        listobj *temp = extract_ready_timer_list(g_readylist);
        insert_waiting_ready_list(g_readylist, temp);
        LoadContext();
    }
}

void TimerInt(void){
    TC++;
    listobj *newObj = g_timerlist->pHead->pNext;
    while (newObj != g_timerlist->pTail && TC >= newObj->nTCnt) {
        extract_timerlist();
        insert_waiting_ready_list(g_readylist, newObj);
        newObj = g_timerlist->pHead->pNext;
    }
    newObj = g_waitinglist->pHead->pNext;
    while(newObj != g_waitinglist->pTail){
        if(newObj->pTask->DeadLine <= TC){
            insert_waiting_ready_list(g_readylist, newObj);
            extract_waitinglist(newObj);
            newObj = g_waitinglist->pHead->pNext;
        }
    }
}



