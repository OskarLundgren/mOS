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

int g_mode = 0;
int g_firstrun = 1;

int main(void) {
    TC = 0;
    init_kernel();
    
    return 0;
}
