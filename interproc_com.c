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



//mailbox struct with FIFO-implementation

exception push_mailbox(mailbox *mBox, msg *message){
	
	if(mBox == NULL || message == NULL){
	
		return FAIL;
	}

	message->pPrevious = mBox->pHead;
	message->pNext = mBox->pHead->pNext;
	mBox->pHead->pNext->pPrevious = message;
	mBox->pHead->pNext = message;
	
	return OK;
}

msg* pop_mailbox(mailbox *mBox){
	
	msg* target_message = mBox->pTail->pPrevious;
	target_message->pPrevious->pNext = target_message->pNext;
	mBox->pTail->pPrevious = target_message->pPrevious;
	
	target_message->pNext = NULL;
	target_message->pPrevious = NULL;
	
	
	return target_message;
	
	
}



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
		newMb->pTail->pPrevious = newMb->pHead;
		
		
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
		if (mBox->nBlockedMsg < 0 && mBox->pHead->pNext != mBox->pTail) {
			msg* reciever = pop_mailbox(mBox);
			memcpy(reciever->pData, pData, mBox->nDataSize);
			
			
			extract_waitinglist(reciever->pBlock);
			insert_waiting_ready_list(g_readylist,reciever->pBlock);
			mBox->nBlockedMsg++;
		}
		
		else{
			msg *message = malloc(sizeof(msg));
			if(message == NULL){
				free(message);
				return FAIL;
			}
			message->pData = malloc(mBox->nDataSize);
			if(message->pData == NULL){
			
				return FAIL;
			}
			
			memcpy(message->pData, pData, mBox->nDataSize);
			exception push_status = push_mailbox(mBox, message);
			
			if(push_status == FAIL){
				
				return push_status;
			
			}
			
			
			listobj *sending_task;
			sending_task = extract_readylist();
			insert_waiting_ready_list(g_waitinglist, sending_task);
			
			message->pBlock = sending_task;
			sending_task->pMessage = message;
			mBox->nBlockedMsg++;
		}
		LoadContext();
	}
	
	else{
		
		if(g_waitinglist->pHead->pNext->pTask->DeadLine <= TC){
			isr_off();

			g_readylist->pHead->pNext->pMessage->pNext->pPrevious = g_readylist->pHead->pNext->pMessage->pPrevious;
			g_readylist->pHead->pNext->pMessage->pPrevious->pNext = g_readylist->pHead->pNext->pMessage->pNext;
			
			free(g_readylist->pHead->pNext->pMessage->pData);
			free(g_readylist->pHead->pNext->pMessage);
						
			mBox->nMessages--;
			mBox->nBlockedMsg--;
			isr_on();
			return DEADLINE_REACHED;
		}
		
		else{
			return OK;
			
		}
	}
	
	return OK;
}


exception recieve_wait(mailbox *mBox, void *data){

	volatile int firstrun = TRUE;
	isr_off();
	SaveContext();
	
	if(firstrun){
		firstrun = FALSE;
		
		if(mBox->nBlockedMsg > 0 && mBox->pHead->pNext != mBox->pTail){
			memcpy(mBox->pHead->pNext->pData, data, mBox->nDataSize);
			remove_message(mBox->pHead->pNext);
			mBox->nBlockedMsg--;
			mBox->nMessages--;

		
		
		if(mBox->nBlockedMsg < 0){
			listobj *sending_task;
			sending_task = mBox->pHead->pNext->pBlock;
			extract_waitinglist(sending_task);
			insert_waiting_ready_list(g_readylist, sending_task);
			mBox->nBlockedMsg--;
			mBox->nMessages++;
	
		
		}
		else{
			free(mBox->pHead->pNext->pData);
		
		}
	}
	else{
		listobj *recieving_task;
		msg *message = malloc(sizeof(msg));
		mBox->pHead->pNext = message;
		recieving_task = extract_readylist();
		insert_waiting_ready_list(g_waitinglist, recieving_task);
	}
	LoadContext();
}
	else{
		if(g_waitinglist->pHead->pNext->pTask->DeadLine <= TC){
			isr_off();
			
			g_readylist->pHead->pNext->pMessage->pNext->pPrevious = g_readylist->pHead->pNext->pMessage->pPrevious;
			g_readylist->pHead->pNext->pMessage->pPrevious->pNext = g_readylist->pHead->pNext->pMessage->pNext;
			
			free(g_readylist->pHead->pNext->pMessage->pData);
			free(g_readylist->pHead->pNext->pMessage);
			
			mBox->nMessages--;
			mBox->nBlockedMsg--;
			
			
			isr_on();
			return DEADLINE_REACHED;
		
		}
		
		else{
			return OK;
		}
	
	}
	return OK;
}



exception send_no_wait(mailbox *mBox, void *data){
	volatile int first_run = TRUE;
	isr_off();
	SaveContext();
	
	if(first_run){
		first_run = FALSE;
		
		if(mBox->nMessages < 0 && mBox->pHead->pNext != mBox->pTail){
			listobj *recieving_task;
			memcpy(mBox->pHead->pNext->pData, data, mBox->nDataSize);
			remove_message(mBox->pHead->pNext);
			recieving_task = mBox->pHead->pNext->pBlock;
			extract_waitinglist(recieving_task);
			insert_waiting_ready_list(g_readylist, recieving_task);
			
		}
	
	}
	
}
	





















