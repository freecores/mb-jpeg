/*-----------------------------------------*/
/* File : utils.c, utilities for jfif view */
/* Author : Pierre Guerrier, march 1998	   */
/*-----------------------------------------*/

#include <stdlib.h>
#include <stdio.h>

/* Prints a data block in frequency space. */
void
show_FBlock(FBlock *S)
{
  int i,j;

  for (i=0; i<8; i++) {
    for (j=0; j<8; j++)
      fprintf(stderr, "\t%d", S->block[i][j]);
    fprintf(stderr, "\n");
  }
}

/* Prints a data block in pixel space. */
void
show_PBlock(PBlock *S)
{
  int i,j;

  for (i=0; i<8; i++) {
    for (j=0; j<8; j++)
      fprintf(stderr, "\t%d", S->block[i][j]);
    fprintf(stderr, "\n");
  }
}

/* Prints the next 800 bits read from file `fi'. */
void
bin_dump(FILE *fi)
{
  int i;

  for (i=0; i<100; i++) {
    unsigned int bitmask;
    int c = fgetc(fi);

    for (bitmask = 0x80; bitmask; bitmask >>= 1)
      fprintf(stderr, "\t%1d", !!(c & bitmask));
    fprintf(stderr, "\n");
  }
}


/*-------------------------------------------*/
/* core dump generator for forensic analysis */
/*-------------------------------------------*/

void
suicide(void)
{
  int *P;

  fflush(stdout);
  fflush(stderr);
  P = NULL;
  *P = 1;
}


/*-------------------------------------------*/


void
aborted_stream(FILE *fi)
{
  fprintf(stderr, "%ld:\tERROR:\tAbnormal end of decompression process!\n",
	  ftell(fi));
  fprintf(stderr, "\tINFO:\tTotal skipped bytes %d, total stuffers %d\n",
	  passed, stuffers);

  fclose(fi);

  free_structures();

  if (DEBUG) suicide();
}


/*----------------------------------------------------------*/

/* Returns ceil(N/D). */
int
ceil_div(int N, int D)
{
  int i = N/D;

  if (N > D*i) i++;
  return i;
}


/* Returns floor(N/D). */
int
floor_div(int N, int D)
{
  int i = N/D;

  if (N < D*i) i--;
  return i;
}


/*----------------------------------------------------------*/

/* For all components reset DC prediction value to 0. */
void
reset_prediction(void)
{
  int i;

  for (i=0; i<3; i++) comp[i].PRED = 0;
}


/*---------------------------------------------------------*/


/* Transform JPEG number format into usual 2's-complement format. */
int
reformat(unsigned long S, int good)
{
  int St;

  if (!good)
    return 0;
  St = 1 << (good-1);	/* 2^(good-1) */
  if (S < (unsigned long) St)
    return (S+1+((-1) << good));
  else
    return S;
}


/*----------------------------------------------------------*/

void
free_structures(void)
{
  int i;

  for (i=0; i<4; i++) if (QTvalid[i]) free(QTable[i]);

  free(ColorBuffer);
  free(FrameBuffer);

  for (i=0; MCU_valid[i] != -1; i++) free(MCU_buff[i]);
}


/*-------------------------------------------*/
/* this is to save final RGB image to disk   */
/* using the sunraster uncompressed format   */
/*-------------------------------------------*/

/* Sun raster header */

typedef struct {
  unsigned long	MAGIC;
  unsigned long	Width;
  unsigned long	Heigth;
  unsigned long	Depth;
  unsigned long	Length;
  unsigned long	Type;
  unsigned long	CMapType;
  unsigned long	CMapLength;
} sunraster;


void
RGB_save(FILE *fo)
{
  sunraster *FrameHeader;
  int i;

  FrameHeader = (sunraster *) malloc(sizeof(sunraster));
  FrameHeader->MAGIC      = 0x59a66a95L;
  FrameHeader->Width      = 2 * ceil_div(x_size, 2); /* round to 2 more */
  FrameHeader->Heigth     = y_size;
  FrameHeader->Depth      = (n_comp>1) ? 24 : 8;
  FrameHeader->Length     = 0;	/* not required in v1.0 */
  FrameHeader->Type       = 0;	/* old one */
  FrameHeader->CMapType   = 0;	/* truecolor */
  FrameHeader->CMapLength = 0;	/* none */

  fwrite(FrameHeader, sizeof(sunraster), 1, fo);
  for (i=0; i<y_size; i++)
    fwrite(FrameBuffer+n_comp*i*x_size, n_comp, FrameHeader->Width, fo);
}

