//---------------------------------------------------------------------------
#include <stdio.h>
#include "zzq.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma argsused
void zzq(signed short pixelmatrix[MATRIX_SIZE][MATRIX_SIZE], signed char *bitstream, int compression, int encode)
{
unsigned char quantization_table[MATRIX_SIZE][MATRIX_SIZE] ={
        {4, 3, 3, 4, 4, 5, 6, 6},
        {3, 3, 4, 4, 5, 6, 6, 6},
        {4, 4, 4, 4, 5, 6, 6, 6},
        {4, 4, 4, 5, 6, 6, 6, 6},
        {4, 4, 5, 6, 6, 7, 7, 6},
        {4, 5, 6, 6, 6, 7, 7, 6},
        {6, 6, 6, 6, 7, 7, 7, 7},
        {6, 6, 6, 7, 7, 7, 7, 7}
    };
    int i, x, y, jumped, deltax, deltay;
    
    x = y = deltax = deltay = jumped = 0;
  //  printf("\nZigZag order pixelmatrix:\n\n");
    for(i=0;i<NUMBER_OF_PIXELS;i++)
    {
        if (encode == 0) {
                pixelmatrix[y][x] = (bitstream[i]*quantization_table[y][x]);
        } else {
                if(pixelmatrix[y][x]>0)
                        bitstream[i] = (pixelmatrix[y][x]>>quantization_table[y][x]);
                else
                        bitstream[i] = -((-pixelmatrix[y][x])>>quantization_table[y][x]);
              //  printf("%i ",bitstream[i]);
        }

        if((y == 0) || (y == MATRIX_SIZE-1)) { //on top or bottom side of matrix
                if(!jumped) { //first jump to element on the right
                        x++;
                        jumped = 1;
                } else { //modify direction
                        if(i<(NUMBER_OF_PIXELS>>1)) {
                                deltax = -1;
                                deltay = 1;
                        } else {
                                deltax = 1;
                                deltay = -1;
                        }
                        x += deltax;
                        y += deltay;
                        jumped = 0;
                }
        } else if ((x == 0) || (x == MATRIX_SIZE-1)) { //on left or right side of matrix
                if(!jumped) { //jump to element below
                        y++;
                        jumped = 1;
                } else { //modify direction
                        if(i<(NUMBER_OF_PIXELS>>1)) {
                                deltax = 1;
                                deltay = -1;
                        } else {
                                deltax = -1;
                                deltay = 1;
                        }
                        x += deltax;
                        y += deltay;
                        jumped = 0;
                }
        }
        else {//not on the edges of the matrix
                x += deltax;
                y += deltay;
        }
    }
}
//---------------------------------------------------------------------------
 