//
//  main.c
//  mOS DSTII
//
//  Created by Markus Axelsson on 2015-02-06.
//  Copyright (c) 2015 Markus & Oskar. All rights reserved.
//

#include <stdio.h>
#include "list_admin.h"
#include "main.h"
#include "memwatch.h"

int g_running_mode = FALSE;
int g_firstrun = TRUE;
TCB *running = NULL;


int main(void) {
    TC = 0;
    
    return 0;
}
