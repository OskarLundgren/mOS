//
//  task_admin.c
//  mOS
//
//  Created by Markus Axelsson on 2015-02-10.
//  Copyright (c) 2015 Markus & Oskar. All rights reserved.
//

#include "list_admin.h"
#include "timing.h"
#include "main.h"


bool State = INIT;


void idle(){

	for(;;);
	
}

exception init_kernel(void){
    set_ticks(0);
	g_readylist = create_list();
	
	if(g_readylist == NULL){
	
		free(g_readylist);
		return FAIL;
	}
	
    g_timerlist = create_list();
	
	if(g_timerlist == NULL){
		free(g_readylist);
		free(g_timerlist);
		return FAIL;
	}
	
    g_waitinglist = create_list();
	
	if(g_waitinglist == NULL){
		free(g_readylist);
		free(g_timerlist);
		free(g_waitinglist);
		return FAIL;
	}
	
	exception status = create_task(&idle, 0);
	
	if(status != OK){
		
		free(g_readylist);
		free(g_timerlist);
		free(g_waitinglist);
		
		return status;
		
	
	}
	State = INIT;
	return OK;
		
    
}

exception create_task( void(*body)(), uint d ){
    int status;
    TCB *newTCB = malloc(sizeof(TCB));
    listobj *newObj = malloc(sizeof(listobj));
    if((newObj == 0) && (newTCB == 0)){
        free(newTCB);
        free(newObj);
        return FAIL;
    }
    else{

        newTCB->DeadLine = d;
        newTCB->PC = body;
        newTCB->SP = &(newTCB->StackSeg[99]);
        if (g_running_mode == 0) {
            status = insert_waiting_ready_list(g_readylist,newObj);

        newTCB->DeadLine = d;
        newTCB->PC = body;
        newTCB->SP = &(newTCB->StackSeg[99]);
        if (g_running_mode == 0) {
            status = insert_waiting_ready_list(g_readylist,newObj);

            return status;
        }
        else{
            //Enter line here to disable interupts
            
            SaveContext();
            if (g_firstrun == 1) {
                g_firstrun = 0;

                status = insert_waiting_ready_list(g_readylist, newObj);

                status = insert_waiting_ready_list(g_readylist,newObj);

                LoadContext();
            }
        }
        return status;  

	}
}
	return status;
	
}


void run(void){

	//Timer0_Start();
	
	
	State = RUNNING;
	isr_on();
	LoadContext();
}



void terminate(void){
	
	
	listobj *remove_object;

	remove_object = extract_readylist();


	free(remove_object);
	LoadContext();

}