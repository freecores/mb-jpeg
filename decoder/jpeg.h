/* File : jpeg.h, header for all jpeg code */
/* Author: Pierre Guerrier, march 1998     */
/*                                         */
/* 19/01/99  Edited by Koen van Eijk       */

/*#define SPY*/
/* Leave structures in memory,output something and dump core in the event
   of a failure: */
#define DEBUG 0



/*----------------------------------*/
/* JPEG format parsing markers here */
/*----------------------------------*/

#define SOI_MK	0xFFD8		/* start of image	*/
#define APP_MK	0xFFE0		/* custom, up to FFEF */
#define COM_MK	0xFFFE		/* commment segment	*/
#define SOF_MK	0xFFC0		/* start of frame	*/
#define SOS_MK	0xFFDA		/* start of scan	*/
#define DHT_MK	0xFFC4		/* Huffman table	*/
#define DQT_MK	0xFFDB		/* Quant. table		*/
#define DRI_MK	0xFFDD		/* restart interval	*/
#define EOI_MK	0xFFD9		/* end of image		*/
#define MK_MSK	0xFFF0

#define RST_MK(x)	( (0xFFF8&(x)) == 0xFFD0 )
			/* is x a restart interval ? */



/*-------------------------------------------------------- */
/* all kinds of macros here				*/
/*-------------------------------------------------------- */

#define first_quad(c)   ((c) >> 4)        /* first 4 bits in file order */
#define second_quad(c)  ((c) & 15)

#define HUFF_ID(hclass, id)       (2 * (hclass) + (id))

#define DC_CLASS        0
#define AC_CLASS        1


/*-------------------------------------------------------*/
/* JPEG data types here					*/
/*-------------------------------------------------------*/

typedef union {		/* block of pixel-space values */
  unsigned char	block[8][8];
  unsigned char	linear[64];
} PBlock;

typedef union {		/* block of frequency-space values */
  int block[8][8];
  int linear[64];
} FBlock;


/* component descriptor structure */

typedef struct {
  unsigned char	CID;	/* component ID */
  unsigned char	IDX;	/* index of first block in MCU */

  unsigned char	HS;	/* sampling factors */
  unsigned char	VS;
  unsigned char	HDIV;	/* sample width ratios */
  unsigned char	VDIV;

  char		QT;	/* QTable index, 2bits 	*/
  char		DC_HT;	/* DC table index, 1bit */
  char		AC_HT;	/* AC table index, 1bit */
  int		PRED;	/* DC predictor value */
} cd_t;


/*--------------------------------------------*/
/* global variables here		      */
/*--------------------------------------------*/


extern cd_t   comp[3]; /* for every component, useful stuff */

extern PBlock *MCU_buff[10];  /* decoded component buffer */
			      /* between IDCT and color convert */
extern int    MCU_valid[10];  /* for every DCT block, component id then -1 */

extern PBlock *QTable[4];     /* three quantization tables */
extern int    QTvalid[4];     /* at most, but seen as four ... */

extern FILE *fi;
extern FILE *fo;

/* picture attributes */
extern int x_size, y_size;	/* Video frame size     */
extern int rx_size, ry_size;	/* down-rounded Video frame size */
				/* in pixel units, multiple of MCU */
extern int MCU_sx, MCU_sy;  	/* MCU size in pixels   */
extern int mx_size, my_size;	/* picture size in units of MCUs */
extern int n_comp;		/* number of components 1,3 */

/* processing cursor variables */
extern int in_frame, curcomp, MCU_row, MCU_column;
			/* current position in MCU unit */

/* RGB buffer storage */
extern unsigned char *ColorBuffer;   /* MCU after color conversion */
extern unsigned char *FrameBuffer;   /* complete final RGB image */
extern PBlock        *PBuff;
extern FBlock        *FBuff;

/* process statistics */
extern int stuffers;	/* number of stuff bytes in file */
extern int passed;	/* number of bytes skipped looking for markers */

extern int verbose;


/*-----------------------------------------*/
/* prototypes from utils.c		   */
/*-----------------------------------------*/

extern void show_FBlock(FBlock *S);
extern void show_PBlock(PBlock *S);
extern void bin_dump(FILE *fi);

extern int      ceil_div(int N, int D);
extern int      floor_div(int N, int D);
extern void     reset_prediction();
extern int	reformat(unsigned long S, int good);
extern void     free_structures();
extern void	suicide();
extern void	aborted_stream(FILE *fi);
extern void	RGB_save(FILE *fo);

/*-----------------------------------------*/
/* prototypes from parse.c		   */
/*-----------------------------------------*/

extern void	clear_bits();
extern unsigned long	get_bits(FILE *fi, int number);
extern unsigned char	get_one_bit(FILE *fi);
extern unsigned int	get_size(FILE *fi);
extern unsigned int     get_next_MK(FILE *fi);
extern int	load_quant_tables(FILE *fi);
extern int      init_MCU();
extern void     skip_segment(FILE *fi);
extern int      process_MCU(FILE *fi);

/*-------------------------------------------*/
/* prototypes from fast_idct.c               */
/*-------------------------------------------*/

extern void	IDCT(const FBlock *S, PBlock *T);

/*-----------------------------------------*/
/* prototypes from color.c		   */
/*-----------------------------------------*/

extern void	color_conversion();

/*-------------------------------------------*/
/* prototypes from table_vld.c or tree_vld.c */
/*-------------------------------------------*/

extern int	load_huff_tables(FILE *fi);
extern unsigned char    get_symbol(FILE *fi, int select);

/*-----------------------------------------*/
/* prototypes from huffman.c 		   */
/*-----------------------------------------*/

extern void	unpack_block(FILE *fi, FBlock *T, int comp);
		/* unpack, predict, dequantize, reorder on store */

/*-----------------------------------------*/
/* prototypes from spy.c		   */
/*-----------------------------------------*/

extern void 	trace_bits(int number, int type);
extern void	output_stats(char *dumpfile);

