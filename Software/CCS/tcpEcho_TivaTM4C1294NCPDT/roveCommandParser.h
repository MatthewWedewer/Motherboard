/*
 * roveCommandParser.h
 *
 *  Created on: Jan 24, 2015
 *      Author: owen
 */

#ifndef ROVECOMMANDPARSER_H_
#define ROVECOMMANDPARSER_H_

#include <string.h>

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/GPIO.h>

/* NDK BSD support */
#include <sys/socket.h>

/* Example/Board Header file */
#include "Board.h"

/* Mailbox Support */
#include <ti/sysbios/knl/Mailbox.h>

/* Defines communication interface with Network Task */
#include "roveTcpHandler.h"

Void roveCommandParser(UArg arg0, UArg arg1);

#endif /* ROVECOMMANDPARSER_H_ */