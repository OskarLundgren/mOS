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

int first_run = 0;

exception wait1( uint nTicks ){
    volatile uint firstrun = TRUE;
    int status = DEADLINE_REACHED;
    listobj * tempObj;
    isr_off();
    SaveContext();
    if (firstrun) {
        firstrun = FALSE;
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
    isr_off();
    SaveContext();
    if () {
        <#statements#>
    }
}