#ifndef _MB_JPEG
#define _MB_JPEG

//  Microblaze related declaration

#ifdef __MICROBLAZE
#include <xparameters.h>
#include <sysace_stdio.h>

#include "xio.h"
#endif

// XUP2P board related declaration 

#ifdef __XUPV2P
#define JPG_ADDRESS    0x70000000
#define JPG_MAXSIZE     0x4000
#define BMP_ADDRESS  0x70000800
#define BMP_MAXSIZE   0x1c000
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

int decode();

#endif

