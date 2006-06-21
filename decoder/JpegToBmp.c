#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "jpeg.h"
#include "fast_int_idct.c"
#include "huffman.c"
#include "color.c"
#include "parse.c"
#include "table_vld.c"
#include "utils.c"

/* real declaration of global variables here */
/* see jpeg.h for more info			*/

cd_t   comp[3];	      /* descriptors for 3 components */
PBlock *MCU_buff[10]; /* decoded DCT blocks buffer */
int    MCU_valid[10]; /* components of above MCU blocks */

PBlock *QTable[4];	/* quantization tables */
int    QTvalid[4];

int   x_size,y_size;	 /* Video frame size	 */
int   rx_size,ry_size;	 /* down-rounded Video frame size (integer MCU) */
int   MCU_sx, MCU_sy;	 /* MCU size in pixels	 */
int   mx_size, my_size;	 /* picture size in units of MCUs */
int   n_comp;		 	 /* number of components 1,3 */

unsigned char *ColorBuffer; /* MCU after color conversion */
unsigned char *FrameBuffer; /* complete final RGB image */
FBlock	      *FBuff;	    /* scratch frequency buffer */
PBlock	      *PBuff;	    /* scratch pixel buffer */

int	in_frame, curcomp;   /* frame started ? current component ? */
int	MCU_row, MCU_column; /* current position in MCU unit */

FILE	*fi;			/* input  File stream pointer	*/

int stuffers = 0;	/* stuff bytes in entropy coded segments */
int passed = 0;		/* bytes passed when searching markers */

int verbose = 0;


/*-----------------------------------------------------------------*/
/*		MAIN		MAIN		MAIN		   */
/*-----------------------------------------------------------------*/

int JpegToBmp(void)
{
  char *p;
  unsigned int aux, mark;
  int n_restarts, restart_interval, leftover; /* RST check */
  int i,j;

  fi = fopen(file1,"rb");
  if (fi == NULL) {
    MessageBox(0,"unable to open the file",file1,0);
    return 0;
  }

  /* First find the SOI marker: */
  aux = get_next_MK(fi);
  if (aux != SOI_MK)
    aborted_stream(fi);

  if (verbose)
    fprintf(stderr, "%ld:\tINFO:\tFound the SOI marker!\n", ftell(fi));
  in_frame = 0;
  restart_interval = 0;
  for (i = 0; i < 4; i++)
    QTvalid[i] = 0;

  /* Now process segments as they appear: */
  do {
    mark = get_next_MK(fi);

    switch (mark) {
    case SOF_MK:
      if (verbose)
	fprintf(stderr, "%ld:\tINFO:\tFound the SOF marker!\n", ftell(fi));
      in_frame = 1;
      get_size(fi);	/* header size, don't care */

      /* load basic image parameters */
      fgetc(fi);	/* precision, 8bit, don't care */
      y_size = get_size(fi);
      x_size = get_size(fi);
      if (verbose)
	fprintf(stderr, "\tINFO:\tImage size is %d by %d\n", x_size, y_size);

      n_comp = fgetc(fi);	/* # of components */
      if (verbose) {
	fprintf(stderr, "\tINFO:\t");
	switch (n_comp)
	  {
	  case 1:
	    fprintf(stderr, "Monochrome");
	    break;
	  case 3:
	    fprintf(stderr, "Color");
	    break;
	  default:
	    fprintf(stderr, "Not a");
	    break;
	  }
	fprintf(stderr, " JPEG image!\n");
      }

      for (i = 0; i < n_comp; i++) {
	/* component specifiers */
	comp[i].CID = fgetc(fi);
	aux = fgetc(fi);
	comp[i].HS = first_quad(aux);
	comp[i].VS = second_quad(aux);
	comp[i].QT = fgetc(fi);
      }
      if ((n_comp > 1) && verbose)
	fprintf(stderr, "\tINFO:\tColor format is %d:%d:%d, H=%d\n",
		comp[0].HS * comp[0].VS,
		comp[1].HS * comp[1].VS,
		comp[2].HS * comp[2].VS,
		comp[1].HS);

      if (init_MCU() == -1)
	aborted_stream(fi);

      /* dimension scan buffer for YUV->RGB conversion */
      FrameBuffer =
	(unsigned char *) malloc( (size_t) x_size * y_size * n_comp);
      ColorBuffer =
	(unsigned char *) malloc( (size_t) MCU_sx * MCU_sy * n_comp);
      FBuff = (FBlock *) malloc(sizeof(FBlock));
      PBuff = (PBlock *) malloc(sizeof(PBlock));

      if ((FrameBuffer == NULL) || (ColorBuffer == NULL) ||
	  (FBuff == NULL) || (PBuff == NULL) ) {
	fprintf(stderr, "\tERROR:\tCould not allocate pixel storage!\n");
	exit(1);
      }
      break;

    case DHT_MK:
      if (verbose)
	fprintf(stderr, "%ld:\tINFO:\tDefining Huffman Tables\n", ftell(fi));
      if (load_huff_tables(fi) == -1)
	aborted_stream(fi);
      break;

    case DQT_MK:
      if (verbose)
	fprintf(stderr,
		"%ld:\tINFO:\tDefining Quantization Tables\n", ftell(fi));
      if (load_quant_tables(fi) == -1)
	aborted_stream(fi);
      break;

    case DRI_MK:
      get_size(fi);	/* skip size */
      restart_interval = get_size(fi);
      if (verbose)
	fprintf(stderr, "%ld:\tINFO:\tDefining Restart Interval %d\n",
		ftell(fi), restart_interval);
      break;

    case SOS_MK:		/* lots of things to do here */
      if (verbose)
	fprintf(stderr, "%ld:\tINFO:\tFound the SOS marker!\n", ftell(fi));
      get_size(fi); /* don't care */
      aux = fgetc(fi);
      if (aux != (unsigned int) n_comp) {
	fprintf(stderr, "\tERROR:\tBad component interleaving!\n");
	aborted_stream(fi);
      }

      for (i = 0; i < n_comp; i++) {
	aux = fgetc(fi);
	if (aux != comp[i].CID) {
	  fprintf(stderr, "\tERROR:\tBad Component Order!\n");
	  aborted_stream(fi);
	}
	aux = fgetc(fi);
	comp[i].DC_HT = first_quad(aux);
	comp[i].AC_HT = second_quad(aux);
      }
      get_size(fi); fgetc(fi);	/* skip things */

      MCU_column = 0;
      MCU_row = 0;
      clear_bits();
      reset_prediction();

      /* main MCU processing loop here */
      if (restart_interval) {
	n_restarts = ceil_div(mx_size * my_size, restart_interval) - 1;
	leftover = mx_size * my_size - n_restarts * restart_interval;
	/* final interval may be incomplete */

	for (i = 0; i < n_restarts; i++) {
	  for (j = 0; j < restart_interval; j++)
	    process_MCU(fi);
	  /* proc till all EOB met */

	  aux = get_next_MK(fi);
	  if (!RST_MK(aux)) {
	    fprintf(stderr, "%ld:\tERROR:\tLost Sync after interval!\n",
		   ftell(fi));
	    aborted_stream(fi);
	  }
	  else if (verbose)
	    fprintf(stderr, "%ld:\tINFO:\tFound Restart Marker\n", ftell(fi));

	  reset_prediction();
	  clear_bits();
	}		/* intra-interval loop */
      }
      else
	leftover = mx_size * my_size;

      /* process till end of row without restarts */
      for (i = 0; i < leftover; i++)
	process_MCU(fi);

      in_frame = 0;
      break;

    case EOI_MK:
      if (verbose)
	fprintf(stderr, "%ld:\tINFO:\tFound the EOI marker!\n", ftell(fi));
      if (in_frame)
	aborted_stream(fi);

      if (verbose)
	fprintf(stderr, "\tINFO:\tTotal skipped bytes %d, total stuffers %d\n",
		passed, stuffers);
      fclose(fi);

//******** WRITE IT HERE !!! *************
{
	FILE *fpBMP;

	// Create bitmap
	fpBMP=fopen(file2,"wb");
	if(fpBMP == 0)
		return 0;

	// Write header
	fprintf(fpBMP,"%c%c%c%c%c%c%c%c%c%c", 66, 77, 54,252, 18,  0,  0,  0,  0,  0);
	fprintf(fpBMP,"%c%c%c%c%c%c%c%c%c%c", 54,  0,  0,  0, 40,  0,  0,  0,x_size%256,  x_size/256);
	fprintf(fpBMP,"%c%c%c%c%c%c%c%c%c%c",  0,  0, y_size%256,  y_size/256,  0,  0,  1,  0, 24,  0);
	fprintf(fpBMP,"%c%c%c%c%c%c%c%c%c%c",  0,  0,  0,  0,  0,252, 18,  0,206, 14);
	fprintf(fpBMP,"%c%c%c%c%c%c%c%c%c%c",  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);
	fprintf(fpBMP,"%c%c%c%c",  0,  0,216, 14,  0);

	for(int i=y_size-1; i>=0; i--){		// in bitmaps the bottom line of the image is at the beginning of the file
		for(int j=0; j<x_size; j++){
			putc(FrameBuffer[3*(i*x_size+j)+0],fpBMP);
			putc(FrameBuffer[3*(i*x_size+j)+1],fpBMP);
			putc(FrameBuffer[3*(i*x_size+j)+2],fpBMP);
		}
		for(int j=0; j<x_size%4; j++)
			putc(0,fpBMP);
	}

	fclose(fpBMP);
}

      free_structures();
	  return 0;
      break;

    case COM_MK:
      if (verbose)
	fprintf(stderr, "%ld:\tINFO:\tSkipping comments\n", ftell(fi));
      skip_segment(fi);
      break;

    case EOF:
      if (verbose)
	fprintf(stderr, "%ld:\tERROR:\tRan out of input data!\n", ftell(fi));
      aborted_stream(fi);

    default:
      if ((mark & MK_MSK) == APP_MK) {
	if (verbose)
	  fprintf(stderr, "%ld:\tINFO:\tSkipping application data\n",
		  ftell(fi));
	skip_segment(fi);
	break;
      }
      if (RST_MK(mark)) {
	reset_prediction();
	break;
      }
      /* if all else has failed ... */
      fprintf(stderr, "%ld:\tWARNING:\tLost Sync outside scan, %d!\n",
	     ftell(fi), mark);
      aborted_stream(fi);
      break;
    } /* end switch */
  }
  while (1);

  return 0;
}

