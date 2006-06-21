/*-----------------------------------------*/
/* File : parse.c, utilities for jfif view */
/* Author : Pierre Guerrier, march 1998	   */
/*-----------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*---------------------------------------------------------------------*/


/* utility and counter to return the number of bits from file */
/* right aligned, masked, first bit towards MSB's		*/

static unsigned char bit_count;	/* available bits in the window */
static unsigned char window;

unsigned long
get_bits(FILE *fi, int number)
{
  int i, newbit;
  unsigned long result = 0;
  unsigned char aux, wwindow;

  if (!number)
    return 0;

  for (i = 0; i < number; i++) {
    if (bit_count == 0) {
      wwindow = fgetc(fi);

      if (wwindow == 0xFF)
	switch (aux = fgetc(fi)) {	/* skip stuffer 0 byte */
	case EOF:
	case 0xFF:
	  fprintf(stderr, "%ld:\tERROR:\tRan out of bit stream\n", ftell(fi));
	  aborted_stream(fi);
	  break;

	case 0x00:
	  stuffers++;
	  break;

	default:
	  if (RST_MK(0xFF00 | aux))
	    fprintf(stderr, "%ld:\tERROR:\tSpontaneously found restart!\n",
		    ftell(fi));
	  fprintf(stderr, "%ld:\tERROR:\tLost sync in bit stream\n",
		  ftell(fi));
	  aborted_stream(fi);
	  break;
	}

      bit_count = 8;
    }
    else wwindow = window;
    newbit = (wwindow>>7) & 1;
    window = wwindow << 1;
    bit_count--;
    result = (result << 1) | newbit;
  }
  return result;
}


void
clear_bits(void)
{
  bit_count = 0;
}


unsigned char
get_one_bit(FILE *fi)
{
  int newbit;
  unsigned char aux, wwindow;

  if (bit_count == 0) {
    wwindow = fgetc(fi);

    if (wwindow == 0xFF)
      switch (aux = fgetc(fi)) {	/* skip stuffer 0 byte */
      case EOF:
      case 0xFF:
	fprintf(stderr, "%ld:\tERROR:\tRan out of bit stream\n", ftell(fi));
	aborted_stream(fi);
	break;

      case 0x00:
	stuffers++;
	break;

      default:
	if (RST_MK(0xFF00 | aux))
	  fprintf(stderr, "%ld:\tERROR:\tSpontaneously found restart!\n",
		  ftell(fi));
	fprintf(stderr, "%ld:\tERROR:\tLost sync in bit stream\n",
		ftell(fi));
	aborted_stream(fi);
	break;
      }

    bit_count = 8;
  }
  else
    wwindow = window;

  newbit = (wwindow >> 7) & 1;
  window = wwindow << 1;
  bit_count--;
  return newbit;
}

/*----------------------------------------------------------*/


unsigned int
get_size(FILE *fi)
{
  unsigned char aux;

  aux = fgetc(fi);
  return (aux << 8) | fgetc(fi);	/* big endian */
}


/*----------------------------------------------------------*/


void
skip_segment(FILE *fi)	/* skip a segment we don't want */
{
  unsigned int size;
  char	tag[5];
  int i;

  size = get_size(fi);
  if (size > 5) {
    for (i = 0; i < 4; i++)
      tag[i] = fgetc(fi);
    tag[4] = '\0';
    if (verbose)
      fprintf(stderr, "\tINFO:\tTag is %s\n", tag);
    size -= 4;
  }
  fseek(fi, size-2, SEEK_CUR);
}


/*----------------------------------------------------------------*/
/* find next marker of any type, returns it, positions just after */
/* EOF instead of marker if end of file met while searching ...	  */
/*----------------------------------------------------------------*/

unsigned int
get_next_MK(FILE *fi)
{
  unsigned int c;
  int ffmet = 0;
  int locpassed = -1;

  passed--;	/* as we fetch one anyway */

  while ((c = fgetc(fi)) != (unsigned int) EOF) {
    switch (c) {
    case 0xFF:
      ffmet = 1;
      break;
    case 0x00:
      ffmet = 0;
      break;
    default:
      if (locpassed > 1)
	fprintf(stderr, "NOTE: passed %d bytes\n", locpassed);
      if (ffmet)
	return (0xFF00 | c);
      ffmet = 0;
      break;
    }
    locpassed++;
    passed++;
  }

  return (unsigned int) EOF;
}


/*----------------------------------------------------------*/
/* loading and allocating of quantization table             */
/* table elements are in ZZ order (same as unpack output)   */
/*----------------------------------------------------------*/

int
load_quant_tables(FILE *fi)
{
  char aux;
  unsigned int size, n, i, id, x;

  size = get_size(fi); /* this is the tables' size */
  n = (size - 2) / 65;

  for (i = 0; i < n; i++) {
    aux = fgetc(fi);
    if (first_quad(aux) > 0) {
      fprintf(stderr, "\tERROR:\tBad QTable precision!\n");
      return -1;
    }
    id = second_quad(aux);
    if (verbose)
      fprintf(stderr, "\tINFO:\tLoading table %d\n", id);
    QTable[id] = (PBlock *) malloc(sizeof(PBlock));
    if (QTable[id] == NULL) {
      fprintf(stderr, "\tERROR:\tCould not allocate table storage!\n");
      exit(1);
    }
    QTvalid[id] = 1;
    for (x = 0; x < 64; x++)
      QTable[id]->linear[x] = fgetc(fi);
      /*
         -- This is useful to print out the table content --
         for (x = 0; x < 64; x++)
         fprintf(stderr, "%d\n", QTable[id]->linear[x]);
      */
  }
  return 0;
}


/*----------------------------------------------------------*/
/* initialise MCU block descriptors	                    */
/*----------------------------------------------------------*/

int
init_MCU(void)
{
  int i, j, k, n, hmax = 0, vmax = 0;

  for (i = 0; i < 10; i++)
    MCU_valid[i] = -1;

  k = 0;

  for (i = 0; i < n_comp; i++) {
    if (comp[i].HS > hmax)
      hmax = comp[i].HS;
    if (comp[i].VS > vmax)
      vmax = comp[i].VS;
    n = comp[i].HS * comp[i].VS;

    comp[i].IDX = k;
    for (j = 0; j < n; j++) {
      MCU_valid[k] = i;
      MCU_buff[k] = (PBlock *) malloc(sizeof(PBlock));
      if (MCU_buff[k] == NULL) {
	fprintf(stderr, "\tERROR:\tCould not allocate MCU buffers!\n");
	exit(1);
      }
      k++;
      if (k == 10) {
	fprintf(stderr, "\tERROR:\tMax subsampling exceeded!\n");
	return -1;
      }
    }
  }

  MCU_sx = 8 * hmax;
  MCU_sy = 8 * vmax;
  for (i = 0; i < n_comp; i++) {
    comp[i].HDIV = (hmax / comp[i].HS > 1);	/* if 1 shift by 0 */
    comp[i].VDIV = (vmax / comp[i].VS > 1);	/* if 2 shift by one */
  }

  mx_size = ceil_div(x_size,MCU_sx);
  my_size = ceil_div(y_size,MCU_sy);
  rx_size = MCU_sx * floor_div(x_size,MCU_sx);
  ry_size = MCU_sy * floor_div(y_size,MCU_sy);

  return 0;
}


/*----------------------------------------------------------*/
/* this takes care for processing all the blocks in one MCU */
/*----------------------------------------------------------*/

int
process_MCU(FILE *fi)
{
  int  i;
  long offset;
  int  goodrows, goodcolumns;

  if (MCU_column == mx_size) {
    MCU_column = 0;
    MCU_row++;
    if (MCU_row == my_size) {
      in_frame = 0;
      return 0;
    }
    if (verbose)
      fprintf(stderr, "%ld:\tINFO:\tProcessing stripe %d/%d\n", ftell(fi),
	      MCU_row+1, my_size);
  }

  for (curcomp = 0; MCU_valid[curcomp] != -1; curcomp++) {
    unpack_block(fi, FBuff, MCU_valid[curcomp]); /* pass index to HT,QT,pred */
    IDCT(FBuff, MCU_buff[curcomp]);
  }

  /* YCrCb to RGB color space transform here */
  if (n_comp > 1)
    color_conversion();
  else
    memmove(ColorBuffer, MCU_buff[0], 64);

  /* cut last row/column as needed */
  if ((y_size != ry_size) && (MCU_row == (my_size - 1)))
    goodrows = y_size - ry_size;
  else
    goodrows = MCU_sy;

  if ((x_size != rx_size) && (MCU_column == (mx_size - 1)))
    goodcolumns = x_size - rx_size;
  else
    goodcolumns = MCU_sx;

  offset = n_comp * (MCU_row * MCU_sy * x_size + MCU_column * MCU_sx);

  for (i = 0; i < goodrows; i++)
    memmove(FrameBuffer + offset + n_comp * i * x_size,
	    ColorBuffer + n_comp * i * MCU_sx,
	    n_comp * goodcolumns);

  MCU_column++;
  return 1;
}
