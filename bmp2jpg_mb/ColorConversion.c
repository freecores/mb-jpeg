#include <stdio.h>
#include "io.h"

#if 0
/*USED VALUES OF ROY's CODE THIS MIGHT LEAD TO PROBLEMS*/
signed char RGB2Y (int r, int g, int b) {
   return ((66*r + 129*g + 25*b + 128)>>8)+128;
}
/*USED VALUES OF ROY's CODE THIS MIGHT LEAD TO PROBLEMS*/
signed char RGB2Cr (int r, int g, int b) {
   return ((-38*r - 74*g + 112*b + 128)>>8)+128;
}
/*USED VALUES OF ROY's CODE THIS MIGHT LEAD TO PROBLEMS*/
signed char RGB2Cb (int r, int g, int b) {
   return ((112*r - 94*g - 18*b + 128)>>8)+128;
}

void RGB2YCrCb(signed char pixelmatrix[MATRIX_SIZE][MATRIX_SIZE*3],signed char YMatrix[MATRIX_SIZE][MATRIX_SIZE],signed char CrMatrix[MATRIX_SIZE][MATRIX_SIZE],signed char CbMatrix[MATRIX_SIZE][MATRIX_SIZE])
{
        unsigned int row, col;
        for(row = 0;row < MATRIX_SIZE; row++) {
                for(col = 0; col < MATRIX_SIZE; col++) {
                        YMatrix[row][col] = RGB2Y(pixelmatrix[row][col*3+2],pixelmatrix[row][col*3+1],pixelmatrix[row][col*3]) - 128;
                        CrMatrix[row][col] = RGB2Cr(pixelmatrix[row][col*3+2],pixelmatrix[row][col*3+1],pixelmatrix[row][col*3]) - 128;
                        CbMatrix[row][col] = RGB2Cb(pixelmatrix[row][col*3+2],pixelmatrix[row][col*3+1],pixelmatrix[row][col*3]) - 128;
                }
        }
}
#endif

#define RGB2Y(r, g, b)     (((66*r + 129*g + 25*b + 128)>>8)+128)
#define RGB2Cr(r, g, b)    (((-38*r - 74*g + 112*b + 128)>>8)+128)
#define RGB2Cb(r, g, b)   (((112*r - 94*g - 18*b + 128)>>8)+128)


void RGB2Y_matrix(signed char pixelmatrix[MATRIX_SIZE][MATRIX_SIZE*3],signed char YUVMatrix[MATRIX_SIZE][MATRIX_SIZE])
{
        unsigned int row, col;
        for(row = 0;row < MATRIX_SIZE; row++) {
                for(col = 0; col < MATRIX_SIZE; col++) {
                        YUVMatrix[row][col] = RGB2Y(pixelmatrix[row][col*3+2],pixelmatrix[row][col*3+1],pixelmatrix[row][col*3]) - 128;
                }
        }
}
void RGB2Cr_matrix(signed char pixelmatrix[MATRIX_SIZE][MATRIX_SIZE*3],signed char YUVMatrix[MATRIX_SIZE][MATRIX_SIZE])
{
        unsigned int row, col;
        for(row = 0;row < MATRIX_SIZE; row++) {
                for(col = 0; col < MATRIX_SIZE; col++) {
                        YUVMatrix[row][col] = RGB2Cr(pixelmatrix[row][col*3+2],pixelmatrix[row][col*3+1],pixelmatrix[row][col*3]) - 128;
                }
        }
}
void RGB2Cb_matrix(signed char pixelmatrix[MATRIX_SIZE][MATRIX_SIZE*3],signed char YUVMatrix[MATRIX_SIZE][MATRIX_SIZE])
{
        unsigned int row, col;
        for(row = 0;row < MATRIX_SIZE; row++) {
                for(col = 0; col < MATRIX_SIZE; col++) {
                        YUVMatrix[row][col] = RGB2Cb(pixelmatrix[row][col*3+2],pixelmatrix[row][col*3+1],pixelmatrix[row][col*3]) - 128;
                }
        }
}



