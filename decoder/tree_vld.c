/*-----------------------------------------*/
/* File : tree_vld.c, utilities for jfif view */
/* Author : Pierre Guerrier, march 1998	   */
/*-----------------------------------------*/

#include <stdlib.h>
#include <stdio.h>


/*--------------------------------------*/
/* private huffman.c defines and macros */
/*--------------------------------------*/

/* Number of HTable words sacrificed to bookkeeping: */
#define GLOB_SIZE		32

/* Memory size of HTables: */
#define MAX_SIZE(hclass)		((hclass)?384:64)

/* Available cells, top of storage: */
#define MAX_CELLS(hclass)	(MAX_SIZE(hclass) - GLOB_SIZE)

/* for Huffman tree descent */
/* lower 8 bits are for value/left son */

#define GOOD_NODE_FLAG		0x100
#define GOOD_LEAF_FLAG		0x200
#define BAD_LEAF_FLAG		0x300
#define SPECIAL_FLAG		0x000
#define HUFF_FLAG_MSK		0x300

#define HUFF_FLAG(c)		((c) & HUFF_FLAG_MSK)
#define HUFF_VALUE(c)		((unsigned char)( (c) & (~HUFF_FLAG_MSK) ))


/*--------------------------------------*/
/* some static structures for storage	*/
/*--------------------------------------*/

static unsigned int	DC_Table0[MAX_SIZE(DC_CLASS)],
			DC_Table1[MAX_SIZE(DC_CLASS)];

static unsigned int	AC_Table0[MAX_SIZE(AC_CLASS)],
			AC_Table1[MAX_SIZE(AC_CLASS)];

static unsigned int    *HTable[4] = {
  				      &DC_Table0[0], &DC_Table1[0],
				      &AC_Table0[0], &AC_Table1[0]
				    };


/*----------------------------------------------------------*/
/* Loading of Huffman table, with leaves drop ability	    */
/*----------------------------------------------------------*/

int load_huff_tables(FILE *fi)
{
  char aux;
  int size, hclass, id;
  int LeavesN, NodesN, CellsN;
  int MaxDepth, i, k, done;
  int NextCellPt;	/* where shall we put next cell */
  int NextLevelPt;	/* where shall node point to */
  unsigned int flag;

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
    CellsN = NodesN = 1;      /* the root one */
    LeavesN = 0;

    for (i=0; i<MAX_CELLS(hclass); i++)
      HTable[id][i] = SPECIAL_FLAG;	/* secure memory with crash value */

    /* first load the sizes of code elements */
    /* and compute contents of each tree level */
    /* Adress	Content		*/
    /* Top		Leaves 0	*/
    /* Top-1	Nodes  0	*/
    /* ......	.......		*/
    /* Top-2k	Leaves k	*/
    /* Top-2k-1	Nodes  k	*/

    MaxDepth = 0;
    for (i=0; i<16; i++) {
      LeavesN = HTable[id][MAX_SIZE(hclass)-2*i-1] = fgetc(fi);
      CellsN = 2*NodesN; /* nodes is old */
      NodesN = HTable[id][MAX_SIZE(hclass)-2*i-2] = CellsN-LeavesN;
      if (LeavesN) MaxDepth = i;
    }
    size-=16;

    /* build root at address 0, then deeper levels at */
    /* increasing addresses until MAX_CELLS reached */

    HTable[id][0] = 1 | GOOD_NODE_FLAG;		/* points to cell _2_ ! */
    /* we give up address one to keep left brothers on even adresses */
    NextCellPt = 2;
    i = 0;		/* this is actually length 1 */

    done = 0;

    while (i<= MaxDepth) {

      /* then load leaves for other levels */
      LeavesN = HTable[id][MAX_SIZE(hclass)-2*i-1];
      for (k = 0; k<LeavesN; k++)
	if (!done) {
	  HTable[id][NextCellPt++] = fgetc(fi) | GOOD_LEAF_FLAG;
	  if (NextCellPt >= MAX_CELLS(hclass)) {
	    done = 1;
	    fprintf(stderr, "\tWARNING:\tTruncating Table at depth %d\n", i+1);
	  }
	}
	else fgetc(fi);	/* throw it away, just to keep file sync */
      size -= LeavesN;

      if (done || (i == MaxDepth)) { i++; continue; }
      /* skip useless node building */

      /* then build nodes at that level */
      NodesN = HTable[id][MAX_SIZE(hclass)-2*i-2];

      NextLevelPt = NextCellPt+NodesN;
      for (k = 0; k<NodesN; k++) {
	if (NextCellPt >= MAX_CELLS(hclass)) { done = 1; break; }

	flag = ((NextLevelPt|1) >=
		MAX_CELLS(hclass)) ? BAD_LEAF_FLAG : GOOD_NODE_FLAG;
	/* we OR by 1 to check even right brother within range */
	HTable[id][NextCellPt++] = (NextLevelPt/2) | flag;
	NextLevelPt += 2;
      }

      i++;	/* now for next level */
    }	/* nothing left to read from file after maxdepth */

    if (verbose)
      fprintf(stderr, "\tINFO:\tUsing %d words of table memory\n", NextCellPt);

    /*
      -- this is useful for displaying the uploaded tree --
      for(i=0; i<NextCellPt; i++) {
      switch (HUFF_FLAG(HTable[id][i])) {
      case GOOD_NODE_FLAG:
      fprintf(stderr, "Cell %X: Node to %X and %X\n", i,
      HUFF_VALUE(HTable[id][i])*2,
      HUFF_VALUE(HTable[id][i])*2 +1);
      break;
      case GOOD_LEAF_FLAG:
      fprintf(stderr, "Cell %X: Leaf with value %X\n", i,
      HUFF_VALUE(HTable[id][i]) );
      break;
      case SPECIAL_FLAG:
      fprintf(stderr, "Cell %X: Special flag\n", i);
      break;
      case BAD_LEAF_FLAG:
      fprintf(stderr, "Cell %X: Bad leaf\n", i);
      break;
      }
      }
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
  int cellPt;

  cellPt = 0; /* this is the root cell */

  while (HUFF_FLAG(HTable[select][cellPt]) == GOOD_NODE_FLAG)
    cellPt = get_one_bit(fi) | (HUFF_VALUE(HTable[select][cellPt])<<1);

  switch (HUFF_FLAG(HTable[select][cellPt])) {
  case SPECIAL_FLAG:
    fprintf(stderr, "%ld:\tERROR:\tFound forbidden Huffman symbol !\n",
	    ftell(fi));
    aborted_stream(fi, fo);
    break;

  case GOOD_LEAF_FLAG:
    return HUFF_VALUE(HTable[select][cellPt]);
    break;

  case BAD_LEAF_FLAG:
    /* how do we fall back in case of truncated tree ? */
    /* suggest we send an EOB and warn */
    fprintf(stderr, "%ld:\tWARNING:\tFalling out of truncated tree !\n",
	    ftell(fi));
    return 0;
    break;

  default:
    break;
  }
  return 0;
}
