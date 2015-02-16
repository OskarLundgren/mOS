//
//  interproc_com.c
//  mOS
//
//  Created by Markus Axelsson on 2015-02-13.
//  Copyright (c) 2015 Markus & Oskar. All rights reserved.
//

#include "interproc_com.h"

#include "main.h"
#include "kernel.h"



mailbox* create_mailbox( uint nMessages, uint nDataSize ){
    mailbox *newMb = malloc(sizeof(mailbox));
	newMb->pHead = malloc(sizeof(msg));
	newMb->pTail = malloc(sizeof(msg));
	if(newMb == NULL || newMb->pHead == NULL || newMb->pTail == NULL){
		free(newMb->pHead);
		free(newMb->pTail);
		free(newMb);
		return NULL;
	}
	
	else{
		newMb->nMessages = nMessages;
		newMb->nDataSize = nDataSize;
		newMb->nMessages = 0;
		
		newMb->pHead->pNext = newMb->pTail;
		newMb->pTail->pNext = newMb->pHead;
		
		
		
    return newMb;
	}
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

exception send_wait(mailbox* mBox, void* pData){
	volatile int firstrun = TRUE;
    isr_off();
    SaveContext();
    if (firstrun) {
        firstrun = FALSE;
        if (mBox->pHead->pNext != mBox->pTail) {
            listobj * newListObj;
            //void	*memcpy(void *, const void *, size_t); ????
            memcpy(mBox->pHead->pNext->pData, pData, mBox->nDataSize); //??
            
        }

    }
	else{
		
		int i = 0;
	}
	return OK;
}