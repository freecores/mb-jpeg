#include <stdio.h>
#include <stdlib.h>

#include "tb1.h"

unsigned char* jpgimage;
unsigned long jpgsize;
unsigned char* bmpimage;
unsigned long bmpsize;

int main() {
  SYSACE_FILE *jpgfile;
  SYSACE_FILE *bmpfile;
  int i, numread, numwrite;

  jpgsize = bmpsize = 0;
  jpgimage = (unsigned char*)JPG_ADDRESS;
  bmpimage = (unsigned char*)BMP_ADDRESS;
  
  if ((jpgfile = sysace_fopen("image01.jpg", "r")) == NULL) {
  	xil_printf("Error in opening input file\r\n");
	exit(1);
  	}
  
  for (i = 0; i<JPG_MAXSIZE; i++) {
  	numread = sysace_fread(jpgimage+i, 1, 1, jpgfile);
	if (numread == 0) break;
	}

  sysace_fclose(jpgfile);

  if (i==JPG_MAXSIZE) {
  	xil_printf("Input file exceeds max size\r\n");
	exit(1);
  	}

  jpgsize = i;
  
  memcpy(bmpimage, jpgimage, jpgsize);
  bmpsize = jpgsize;

  if ((bmpfile = sysace_fopen("image01.bmp", "w")) == NULL) {
  	xil_printf("Error in opening output file\r\n");
	exit(1);
 	}

  numwrite = sysace_fwrite(bmpimage, 1, bmpsize, bmpfile);
  
  sysace_fclose(bmpfile);

  if (numwrite != bmpsize) {
  	xil_printf("Error in writing output file\r\n");
	exit(1);
  	}

  return 0;

}
