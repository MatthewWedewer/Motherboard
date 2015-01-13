/*
 * networkRove.h
 *
 *  Created on: Jan 12, 2015
 *      Author: Owen, Judah and Ghost of Zeus
 */

#ifndef NETWORKROVE_H_
#define NETWORKROVE_H_


/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

 /* NDK Header files */
#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/_stack.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* Example/Board Header files */
#include "Board.h"


//const UINT32 RED_IP_ADDR = inet_addr("192.168.1..tbd....");
const short ROVE_LISTEN_PORT = 4500;
const int MAX_PACKET_SIZE = 1024;
const int RECV_BUFFER_SIZE = 1024;
//max size of connection request queue to hold pending conn rqst
const int MAXCONN = 3;

Void networkFncRove(UArg arg0, UArg arg1);

#endif /* NETWORKROVE_H_ */
