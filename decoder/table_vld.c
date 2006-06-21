/*-----------------------------------------*/
/* File : table_vld.c, utilities for jfif view */
/* Author : Pierre Guerrier, march 1998	   */
/*-----------------------------------------*/

#include <stdlib.h>
#include <stdio.h>


/*--------------------------------------*/
/* private huffman.c defines and macros */
/*--------------------------------------*/

/* Memory size of HTables; */
#define MAX_SIZE(hclass)		((hclass)?162:14)

/*--------------------------------------*/
/* some static structures for storage */
/*--------------------------------------*/

static unsigned char	DC_Table0[MAX_SIZE(DC_CLASS)],
			DC_Table1[MAX_SIZE(DC_CLASS)];

static unsigned char	AC_Table0[MAX_SIZE(AC_CLASS)],
			AC_Table1[MAX_SIZE(AC_CLASS)];

static unsigned char   *HTable[4] = {
				      &DC_Table0[0], &DC_Table1[0],
				      &AC_Table0[0], &AC_Table1[0]
				    };

static int	MinCode[4][16];
static int	MaxCode[4][16];
static int	ValPtr[4][16];


/*----------------------------------------------------------*/
/* Loading of Huffman table, with leaves drop ability	    */
/*----------------------------------------------------------*/

int
load_huff_tables(FILE *fi)
{
  char aux;
  int size, hclass, id, max;
  int LeavesN, LeavesT, i;
  int AuxCode;

  size = get_size(fi); /* this is the tables' size */

  size -= 2;

  while (size>0) {

    aux = fgetc(fi);
    hclass = first_quad(aux);	/* AC or DC */
    id = second_quad(aux);	/* table no */
    if (id>1) {
      fprintf(stderr, "\tERROR:\tBad HTable identity %d!\n",id);
      return -1;
    }
    id = HUFF_ID(hclass, id);
    if (verbose)
      fprintf(stderr, "\tINFO:\tLoading Table %d\n", id);
    size--;

    LeavesT = 0;
    AuxCode = 0;

    for (i=0; i<16; i++) {
      LeavesN = fgetc(fi);

      ValPtr[id][i] = LeavesT;
      MinCode[id][i] = AuxCode*2;
      AuxCode = MinCode[id][i] + LeavesN;

      MaxCode[id][i] = (LeavesN) ? (AuxCode-1) : (-1);
      LeavesT += LeavesN;
    }
    size-=16;

    if (LeavesT>MAX_SIZE(hclass)) {
      max = MAX_SIZE(hclass);
      fprintf(stderr, "\tWARNING:\tTruncating Table by %d symbols\n",
	      LeavesT-max);
    }
    else max = LeavesT;

    for (i=0; i<max; i++)
      HTable[id][i] = fgetc(fi);	/* load in raw order */

    for (i=max; i<LeavesT; i++) fgetc(fi);	/* skip if we don't load */
    size -= LeavesT;

    if (verbose)
      fprintf(stderr, "\tINFO:\tUsing %d words of table memory\n", LeavesT);

    /*
      -- this is usefule to display the content of uploaded tables --
      for(i=0; i<16; i++)
      fprintf(stderr, "size %d, valptr = %d, min = %x, max = %x\n", i,
      ValPtr[id][i],
      MinCode[id][i], MaxCode[id][i]);
      for(i=0; i<max; i++)
      fprintf(stderr, "entry %d, symbol %X\n", i, HTable[id][i]);
      */

  }	/* loop on tables */
  return 0;
}


/*-----------------------------------*/
/* extract a single symbol from file */
/* using specified huffman table ... */
/*-----------------------------------*/

unsigned char
get_symbol(FILE *fi, int select)
{
  long code = 0;
  int length;
  int index;

  for (length = 0; length < 16; length++) {
    code = (2*code) | get_one_bit(fi);
#ifdef SPY
    trace_bits(1, 0);
#endif
    if (code <= MaxCode[select][length]) break;
  }

  index = ValPtr[select][length] + code - MinCode[select][length];

#ifdef SPY
  trace_bits(0, 0);
#endif

  if (index < MAX_SIZE(select/2)) return HTable[select][index];

  fprintf(stderr, "%ld:\tWARNING:\tOverflowing symbol table !\n", ftell(fi));
  return 0;
}
