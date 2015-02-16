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
#include "list_admin.h"


void remove_message(msg *new_message){
	
	new_message->pPrevious->pNext = new_message->pNext;
	new_message->pNext->pPrevious = new_message->pPrevious;
	new_message->pNext = new_message->pPrevious = NULL;
	free(new_message);
	
}



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
		newMb->nBlockedMsg = 0;
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
	if (firstrun){
		firstrun = FALSE;
		if (mBox->nMessages < 0 && mBox->pHead->pNext != mBox->pTail) {
			listobj * newListObj;
			memcpy(mBox->pHead->pNext->pData, pData, mBox->nDataSize); //??
			listobj *recieving_task;
			recieving_task = mBox->pHead->pNext->pBlock;
			remove_message(mBox->pHead->pNext);
			extract_waitinglist(recieving_task);
			insert_waiting_ready_list(g_readylist,recieving_task);
		}
		
		else{
			msg *message = malloc(sizeof(msg));
			if(message == NULL){
				free(message);
				return FAIL;
			}
			message->pData = pData;
			message->pNext = mBox->pTail;
			message->pPrevious = mBox->pTail->pPrevious;
			message->pBlock = g_readylist->pHead->pNext;
			mBox->pTail->pPrevious->pNext = message;
			mBox->pTail->pPrevious = message;
			mBox->nBlockedMsg++;
			mBox->nMessages++;
			listobj *sending_task;
			sending_task = extract_readylist();
			insert_waiting_ready_list(g_waitinglist, sending_task);
		}
		LoadContext();
	}
	
	else{
		
		if(g_readylist->pHead->pNext->pTask->DeadLine <= TC){
			isr_off();
			remove_message(mBox->pHead->pNext);
			isr_on();
			return DEADLINE_REACHED;
		}
		
		else{
			return OK;
			
		}
	}
	
	return OK;
}

