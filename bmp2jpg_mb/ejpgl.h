#ifndef _EJPGL_H
#define _EJPGL_H

#define MATRIX_SIZE 8


#ifndef htonl
#define htonl(x)    ((((x)&0xff000000)>>24) | (((x)&0x00ff0000)>>8) | (((x)&0x0000ff00)<<8) | (((x)&0x000000ff)<<24))
#endif

#ifndef hton
#define hton(x)  ((((x) & 0xff00)>>8) | (((x) &0x00ff)<<8))
#endif



int idct8x8(int* fblock, char* sblock);

#endif

