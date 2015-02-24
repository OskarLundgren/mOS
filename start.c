//
//  start.c
//  mOS
//
//  Created by Oskar Lundgren on 2015-02-23.
//  Copyright (c) 2015 Oskar Lundgren & Markus Axelsson. All rights reserved.
//

#include "kernel.h"
#include <assert.h>


mailbox *mb = NULL;


volatile bool nTests[50];

void task1();

void task2();

void task3();

void task4();

void task5();

void start(){

	int i = 0;
	
	for(; i < sizeof(nTests); i++){
	
		nTests[i] = FALSE;
	}

	assert(init_kernel() == OK);
	assert(create_task(task1, 2000));
	assert((mb = create_mailbox(100, sizeof(int))));
	
	run();
	
}


void task1(){
	
	create_task(task2,4000);
	assert(nTests[0] == FALSE);
	wait1(20);
	assert(nTests[0] == TRUE);
	

}

void task2(){

	nTests[0] = TRUE;
	
}

