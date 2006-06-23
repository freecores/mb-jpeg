////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2004 Xilinx, Inc.  All rights reserved. 
// 
// Xilinx, Inc. 
// XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
// COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS 
// ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
// STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
// IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
// FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION. 
// XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
// THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
// ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
// FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
// AND FITNESS FOR A PARTICULAR PURPOSE. 
// 
// File   : mb_interface.h
// Date   : 2002, March 20.
// Company: Xilinx
// Group  : Emerging Software Technologies
//
// Summary:
// Header file for mb_interface
//
// $Id: mb_interface.h,v 1.1 2006-06-23 19:03:42 quickwayne Exp $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MICROBLAZE_INTERFACE_H_
#define _MICROBLAZE_INTERFACE_H_

#include "xbasic_types.h"
extern void microblaze_enable_interrupts(void);                 // Enable Interrupts
extern void microblaze_disable_interrupts(void);                // Disable Interrupts
extern void microblaze_enable_icache(void);                     // Enable Instruction Cache
extern void microblaze_disable_icache(void);                    // Disable Instruction Cache
extern void microblaze_register_handler(XInterruptHandler Handler, void *DataPtr);                               // Register top level interrupt handler
extern void microblaze_register_exception_handler(Xuint8 ExceptionId, XExceptionHandler Handler, void *DataPtr); // Register exception handler


extern void microblaze_update_icache (int , int , int );
extern void microblaze_init_icache_range (int , int );
extern void microblaze_update_dcache (int , int , int );
extern void microblaze_init_dcache_range (int , int );


// FSL Access Macros

// Blocking Data Read and Write to FSL no. id
#define microblaze_bread_datafsl(val, id)       asm volatile ("get %0, rfsl" #id : "=d" (val))
#define microblaze_bwrite_datafsl(val, id)      asm volatile ("put %0, rfsl" #id :: "d" (val))

// Non-blocking Data Read and Write to FSL no. id
#define microblaze_nbread_datafsl(val, id)      asm volatile ("nget %0, rfsl" #id : "=d" (val))
#define microblaze_nbwrite_datafsl(val, id)     asm volatile ("nput %0, rfsl" #id :: "d" (val))

// Blocking Control Read and Write to FSL no. id
#define microblaze_bread_cntlfsl(val, id)       asm volatile ("cget %0, rfsl" #id : "=d" (val))
#define microblaze_bwrite_cntlfsl(val, id)      asm volatile ("cput %0, rfsl" #id :: "d" (val))

// Non-blocking Control Read and Write to FSL no. id
#define microblaze_nbread_cntlfsl(val, id)      asm volatile ("ncget %0, rfsl" #id : "=d" (val))
#define microblaze_nbwrite_cntlfsl(val, id)     asm volatile ("ncput %0, rfsl" #id :: "d" (val))


#endif // _MICROBLAZE_INTERFACE_H_
