//
//  interproc_com.h
//  mOS
//
//  Created by Markus Axelsson on 2015-02-13.
//  Copyright (c) 2015 Markus & Oskar. All rights reserved.
//

#ifndef __mOS__interproc_com__
#define __mOS__interproc_com__

#include <stdio.h>
#include "kernel.h"

mailbox * create_mailbox(uint nMessages, uint ndataSize);


#endif /* defined(__mOS__interproc_com__) */
