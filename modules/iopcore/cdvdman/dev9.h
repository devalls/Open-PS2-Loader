/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright (c) 2003 Marcus R. Brown <mrbrown@0xd6.org>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: dev9.h 1454 2007-11-04 23:19:57Z roman_ps2dev $
# DEV9 Device Driver definitions and imports.
*/

#ifndef IOP_DEV9_H
#define IOP_DEV9_H

#include "types.h"
#include "irx.h"

#include "hdd-ioctl.h"

typedef int (*dev9_intr_cb_t)(int flag);
typedef void (*dev9_shutdown_cb_t)(void);
typedef void (*dev9_dma_cb_t)(int bcr, int dir);

int dev9d_init(void);
void SpdRegisterIntrHandler(int intr, dev9_intr_cb_t cb);        // #4
int SpdDmaTransfer(int ctrl, void *buf, int bcr, int dir);       // #5
void Dev9CardStop(void);                                         // #6
void SpdIntrEnable(int mask);                                    // #7
void SpdIntrDisable(int mask);                                   // #8
int SpdGetEthernetID(u16 *buf);                                  // #9
void SpdSetLED(int ctl);                                         // #10
int Dev9RegisterPowerOffHandler(int idx, dev9_shutdown_cb_t cb); // #11
void dev9RegisterPreDmaCb(int ctrl, dev9_dma_cb_t cb);           // #12
void dev9RegisterPostDmaCb(int ctrl, dev9_dma_cb_t cb);          // #13

#endif /* IOP_PS2DEV9_H */
