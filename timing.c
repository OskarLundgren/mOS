//
//  timing.c
//  mOS
//
//  Created by Markus Axelsson on 2015-02-10.
//  Copyright (c) 2015 Markus & Oskar. All rights reserved.
//

#include "timing.h"
#include "main.h"

void set_ticks(uint nTicks){
    TC = nTicks;
}