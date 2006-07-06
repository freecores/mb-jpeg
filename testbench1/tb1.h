#ifndef _TB1_H
#define _TB1_H

//  Microblaze related declaration

#include <xparameters.h>
#include <sysace_stdio.h>

#include "xio.h"

#define __XUPV2P


// XUP2P board related declaration 

#ifdef __XUPV2P
#define JPG_ADDRESS    0x70000000
#define JPG_MAXSIZE     0x10000
#define BMP_ADDRESS  0x70010000
#define BMP_MAXSIZE   0x10000
#else
#define JPG_MAXSIZE     0x400000
#define BMP_MAXSIZE   0x1c00000
#endif

// JPEG decoder related declaration

extern unsigned char* jpgimage;
extern unsigned char* bmpimage;
extern unsigned long jpgsize;
extern unsigned long bmpsize;

// function declaration


#endif

