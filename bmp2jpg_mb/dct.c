
#include <stdio.h>
#include "dct.h"
#include "weights.h"

#include "ejpgl.h"

signed short dctresult[MATRIX_SIZE][MATRIX_SIZE];

#ifdef MULTITASK

/******************************************/
/*
/*  DCT task
/*  for multitask RTOS implementation or
/*       multiprocessor implementation
/*
/******************************************/

void dct_task(void* dct_cfg) {
	struct dct_cfg_block* cb;

	cb = (struct dct_cfg_block*)dct_cfg;
	
	for (;;) {
		semTake();
		dct(cb->input, cb->output);
		semGive();
		}

}

#endif

int dct_init_start() {

	return 0;

}

/*
	Function Name: dct

	Operation: Find the 8x8 DCT of an array using separable DCT
	First, finds 1-d DCT along rows, storing the result in inter[][]
	Then, 1-d DCT along columns of inter[][] is found

	Input: pixels is the 8x8 input array

	Output: dct is the 8x8 output array
*/

void dct(signed char pixels[8][8], int color)
{
        FILE * file;
	int inr, inc; 		/* rows and columns of input image */
	int intr, intc;		/* rows and columns of intermediate image */
	int outr, outc;		/* rows and columns of dct */
	int f_val;		/* cumulative sum */
	int inter[8][8];	/* stores intermediate result */
	int i,j,k;
        k=0;
    //    file = fopen("weights.h","w+");
      //  fprintf(file,"double weights1[512] = {");
	/* find 1-d dct along rows */
 	for (intr=0; intr<8; intr++)
		for (intc=0; intc<8; intc++) {
			for (i=0,f_val=0; i<8; i++) {

			      	f_val += (pixels[intr][i]* weights[k]);//cos((double)(2*i+1)*(double)intc*PI/16);
                                k++;
                          //     fprintf(file, "\n%.0f,",cos((double)(2*i+1)*(double)intc*PI/16)*16384);
			}
                        if (intc!=0)
                                inter[intr][intc] =  f_val>>15;
                        else
                                inter[intr][intc] =  (11585*(f_val>>14))>>15;

                }
   //     fprintf(file,"\n};");
   //     fclose(file);
         k=0;
	/* find 1-d dct along columns */
 	for (outc=0; outc<8; outc++)
		for (outr=0; outr<8; outr++) {
			for (i=0,f_val=0; i<8; i++) {
				f_val += (inter[i][outc] *weights[k]);
                                k++;
			}
                        if (outr!=0)
			        dctresult[outr][outc] = f_val>>15;
                        else
                                dctresult[outr][outc] = (11585*(f_val>>14)>>15);
		}

	zzq_encode(dctresult, color);


}



/*****************************************************************
    UNCOMMENT THIS SECTION TO TEST 2D DCT 
*****************************************************************/

/*
main()
{
  
  unsigned char inputmatrix[8][8];
  unsigned char outputmatrix[8][8];
  unsigned int i,j;


  printf("Input Matrix (8*8) :-\n");
  for (i=0; i<8; i++){
	  printf("\n");
	  for (j=0;j<8;j++){
		   inputmatrix[i][j] = i*8+j;
		   printf("%4d",inputmatrix[i][j]);
	  }
     
  }
  


  dct(inputmatrix,outputmatrix);

  printf("\n\nOutput Matrix (8*8) :-\n");

   for (i=0; i<8; i++){
	   printf("\n");
	  for (j=0;j<8;j++){
	   printf("%4d",outputmatrix[i][j]);
		  
	  }
     
  } 
printf("\n");

}
*/
