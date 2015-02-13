//
//  interproc_com.c
//  mOS
//
//  Created by Markus Axelsson on 2015-02-13.
//  Copyright (c) 2015 Markus & Oskar. All rights reserved.
//

#include "interproc_com.h"
#include "kernel.h"

mailbox* create_mailbox( uint nMessages, uint nDataSize ){
    mailbox *newMb = malloc(sizeof(mailbox));
    newMb->nMessages = nMessages;
    newMb->nDataSize = nDataSize;
    return newMb;
}

exception remove_mailbox( mailbox mb ){
    free( mb );
    
}