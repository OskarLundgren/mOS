//
//  interproc_com.c
//  mOS
//
//  Created by Markus Axelsson on 2015-02-13.
//  Copyright (c) 2015 Markus & Oskar. All rights reserved.
//

#include "interproc_com.h"
#include "kernel.h"
#include "main.h"

int sw_firstrun = TRUE;

mailbox* create_mailbox( uint nMessages, uint nDataSize ){
    mailbox *newMb = malloc(sizeof(mailbox));
    newMb->nMessages = nMessages;
    newMb->nDataSize = nDataSize;
    return newMb;
}

exception remove_mailbox( mailbox * mBox){
    if (mBox->nMessages == 0) {
        free(mBox->pHead);
        free(mBox->pTail);
        free(mBox);
        return OK;
    }
    else{
        return NOT_EMPTY;
    }
}

exception send_wait( mailbox* mBox, void* pData ){
    isr_off();
    SaveContext();
    if (sw_firstrun) {
        sw_firstrun = FALSE;
        if (mBox->pHead->pNext != mBox->pTail) {
            listobj * newListObj;
            
            //void	*memcpy(void *, const void *, size_t); ????
            memcpy(mBox->pHead->pNext->pData, pData, mBox->nDataSize); //??
            
        }
    }
}