#ifdef __MICROBLAZE

#include <stdio.h>
#include <stdlib.h>

#include "xup2pro.h"

#include "zzq.h" 
#include "io.h"
#include "huffman.h"
#include "dct.h"

char* bmpimage;
int bmpsize;

INFOHEADER _bmpheader;

static  signed char pixelmatrix[MATRIX_SIZE][MATRIX_SIZE*3],YMatrix[MATRIX_SIZE][MATRIX_SIZE],CrMatrix[MATRIX_SIZE][MATRIX_SIZE],CbMatrix[MATRIX_SIZE][MATRIX_SIZE];
static  signed short temp[MATRIX_SIZE][MATRIX_SIZE], dctresult[MATRIX_SIZE][MATRIX_SIZE];
static  signed char output[MATRIX_SIZE][MATRIX_SIZE];
static  signed char bitstream[NUMBER_OF_PIXELS] ;
static  unsigned char header[389];

int ejpgl_error(int errno, void* remark);

int main()
{
  SYSACE_FILE *infile;
  SYSACE_FILE *outfile;
  SYSACE_FILE* outfile2;
  
  int i;
  INFOHEADER *bmpheader;
  JPEGHEADER *jpegheader;
  unsigned int col, cols, row, rows, remaining,component;
  unsigned char amount_remaining, Ydcvalue, Cbdcvalue, Crdcvalue ;
  int encode, compression;

  encode = 1;
  compression = 0;

  bmpimage=(unsigned char*)0x70000000;
  bmpsize=0;

  xil_printf("\r\nBMP2JPG Code Compiled at %s %s\r\n", __DATE__, __TIME__);

  bmpheader=&_bmpheader;

  if ((infile = sysace_fopen("image01.bmp", "r")) == NULL) {
  	ejpgl_error(eOPENINPUT_FILE, 0);
  	}

  bmpsize = sysace_fread(bmpimage, 1, 65536, infile);
  xil_printf("bmpsize %d\r\n", bmpsize);
  if (bmpsize==65536) {
  	ejpgl_error(eLARGE_INPUTFILE, 0);
  	}

  if ((outfile2 = sysace_fopen("image01b.bmp", "w")) == NULL) {
	ejpgl_error(eOPENOUTPUT_FILE, 0);
  	}
  sysace_fwrite(bmpimage, 1, bmpsize, outfile2);
  sysace_fclose(outfile2);
  
  if (getbmpheader(infile,bmpheader) == 0) { //File is a valid BMP
  	ejpgl_error(eINVALID_BMP, 0);
  	}
  
  xil_printf("Image width: %d pixels\r\n", bmpheader->width);
  xil_printf("Image height: %d pixels\r\n", bmpheader->height);

  rows = bmpheader->height>>3;
  cols = bmpheader->width>>3;
  remaining=0x00;
  amount_remaining=0x00;
  Ydcvalue = 0x00;
  Crdcvalue = 0x00;
  Cbdcvalue = 0x00;

  if ((outfile = sysace_fopen("image01.jpg", "w")) == NULL) {
  	ejpgl_error(eOPENOUTPUT_FILE, 0);
 	} 
  
  writejpegheader(outfile,bmpheader);

  for (row = 0; row < rows; row++) {
      for (col = 0; col < cols; col++) {
          readbmpfile(infile,pixelmatrix,row,col,bmpheader);
          RGB2YCrCb(pixelmatrix,YMatrix,CrMatrix,CbMatrix);
          for(component=0;component<3;component++)  {
		  	switch (component) {
				case 0 ://Y-encoding
				dct(YMatrix,dctresult);
                            zzq(dctresult,bitstream,compression,encode);
                            Ydcvalue = EncodeDataUnit(bitstream,Ydcvalue,outfile, &remaining, &amount_remaining,component);
                            break;
                            case 1 ://Cr-encoding
                            dct(CrMatrix,dctresult);
                            zzq(dctresult,bitstream,compression,encode);
                            Crdcvalue = EncodeDataUnit(bitstream,Crdcvalue,outfile, &remaining, &amount_remaining,component);
                            break;
                            case 2 ://Cb-encoding
                            dct(CbMatrix,dctresult);
                            zzq(dctresult,bitstream,compression,encode);
                            Cbdcvalue = EncodeDataUnit(bitstream,Cbdcvalue,outfile, &remaining, &amount_remaining,component);
                            break;
                            }
                      }
            }
   }
   HuffmanEncodeFinishSend(&remaining,&amount_remaining,outfile);
   xil_printf("\r\nProcessed %d %dx%d-blocks.\r\n",(row-1)*cols+col,MATRIX_SIZE,MATRIX_SIZE);
   writejpegfooter(outfile);
   
   
   sysace_fclose(outfile);
   sysace_fclose(infile);
   return 0;

}

int ejpgl_error(int errno, void* remark) {

	xil_printf("--> Error %d\r\n", errno);
	exit(1);

}



#else

//---------------------------------------------------------------------------
typedef union {		/* block of pixel-space values */
  unsigned char	block[8][8];
  unsigned char	linear[64];
} PBlock;

typedef union {		/* block of frequency-space values */
  int block[8][8];
  int linear[64];
} FBlock;

#include <stdio.h>
#include <stdlib.h>
#ifndef __MICROBLAZE
#include <windows.h>
#endif
#include "zzq.h" 
#include "io.h"
#include "huffman.h"
#include "dct.h"
#pragma hdrstop


//---------------------------------------------------------------------------

INFOHEADER _bmpheader;

#pragma argsused
int main(int argc, char* argv[])
{
  int encode, compression,i;
  signed char pixelmatrix[MATRIX_SIZE][MATRIX_SIZE*3],YMatrix[MATRIX_SIZE][MATRIX_SIZE],CrMatrix[MATRIX_SIZE][MATRIX_SIZE],CbMatrix[MATRIX_SIZE][MATRIX_SIZE];
  /*= {{124, 105, 139, 95, 143, 98, 132, 114},
{105, 157, 61, 187, 51, 176, 80, 132},
{139, 61, 205, 17, 221, 32, 176, 98},
{95, 187, 17, 239, 0, 221, 51, 143},
{143, 51, 221, 0, 239, 17, 187, 95},
{98, 176, 32, 221, 17, 205, 61, 139},
{132, 80, 176, 51, 187, 61, 157, 105},
{114, 132, 98, 143, 95, 139, 105, 124}};  */
  signed short temp[MATRIX_SIZE][MATRIX_SIZE], dctresult[MATRIX_SIZE][MATRIX_SIZE];
  signed char output[MATRIX_SIZE][MATRIX_SIZE];
  signed char bitstream[NUMBER_OF_PIXELS] ;//= {15,0,-2,-1,-1,-1,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  unsigned char header[389];
  FILE *infile, *outfile;
  INFOHEADER *bmpheader;
  JPEGHEADER *jpegheader;
#ifndef __MICROBLAZE
  LARGE_INTEGER st, en;
#endif  
  unsigned int col, cols, row, rows, remaining,component;
  unsigned char amount_remaining, Ydcvalue, Cbdcvalue, Crdcvalue ;


//  bmpheader = (INFOHEADER *)malloc(sizeof(INFOHEADER));
  bmpheader=&_bmpheader;

  switch (argc) {
        case 3 :
                encode = 0;
        break;
        case 4 :
                encode = 1;
        break;
        case 5 :
                encode = 1;
                compression = atoi(argv[4]);
        break;
        default :
                printf("invalid number of parameters\n\nUSAGE: jpegcodec source_file destination_file [/E] [compression_rate]\n");
                printf("\nsource_file \t\t location of original file");
                printf("\ndestination_file \t location of the new file");
                printf("\n/E \t\t\t When set the source_file will be encoded to JPEG \n\t\t\t otherwise it will be decoded to BMP.");
                printf("\ncompression_rate \t specifies the compression ratio, defaults to the\n\t\t\t standard compression ratio. (Only valid when /D is set)\n");
                system("PAUSE");
                return 0;
  }

  infile = fopen(argv[1],"rb");
  if (infile == NULL) {
          printf("Input file %s does not exist!\n\nUSAGE: jpegcodec source_file destination_file [/E] [compression_rate]\n",argv[1]);
          printf("\nsource_file \t\t location of original file");
          printf("\ndestination_file \t location of the new file");
          printf("\n/E \t\t\t When set the source_file will be encoded to JPEG \n\t\t\t otherwise it will be decoded to BMP.");
          printf("\ncompression_rate \t specifies the compression ratio, defaults to the\n\t\t\t standard compression ratio. (Only valid when /D is set)\n");
          system("PAUSE");
  }
  else { //start codec
          outfile = fopen(argv[2],"wb");
#ifndef __MICROBLAZE		  
          QueryPerformanceCounter(&st);
#endif			 
          if(encode) { //encode infile to JPEG
                if (getbmpheader(infile,bmpheader)) { //File is a valid BMP
                        printf("\nImage width: %d pixels", bmpheader->width);
                        printf("\nImage height: %d pixels", bmpheader->height);
                        rows = bmpheader->height>>3;
                        cols = bmpheader->width>>3;
                        remaining=0x00;
                        amount_remaining=0x00;
                        Ydcvalue = 0x00;
                        Crdcvalue = 0x00;
                        Cbdcvalue = 0x00;
                        writejpegheader(outfile,bmpheader);

                        for (row = 0; row < rows; row++) {
                                for (col = 0; col < cols; col++) {
                                        readbmpfile(infile,pixelmatrix,row,col,bmpheader);
                                        RGB2YCrCb(pixelmatrix,YMatrix,CrMatrix,CbMatrix);
                                        for(component=0;component<3;component++)
                                        {
                                                switch (component) {
                                                        case 0 ://Y-encoding
                                                                dct(YMatrix,dctresult);
                                                                zzq(dctresult,bitstream,compression,encode);
                                                                Ydcvalue = EncodeDataUnit(bitstream,Ydcvalue,outfile, &remaining, &amount_remaining,component);
                                                                break;
                                                        case 1 ://Cr-encoding
                                                                dct(CrMatrix,dctresult);
                                                                zzq(dctresult,bitstream,compression,encode);
                                                                Crdcvalue = EncodeDataUnit(bitstream,Crdcvalue,outfile, &remaining, &amount_remaining,component);
                                                                break;
                                                        case 2 ://Cb-encoding
                                                                dct(CbMatrix,dctresult);
                                                                zzq(dctresult,bitstream,compression,encode);
                                                                Cbdcvalue = EncodeDataUnit(bitstream,Cbdcvalue,outfile, &remaining, &amount_remaining,component);
                                                                break;
                                                }
                                        }
                                }
                        }
                        HuffmanEncodeFinishSend(&remaining,&amount_remaining,outfile);
                        printf("\nProcessed %d %dx%d-blocks.",(row-1)*cols+col,MATRIX_SIZE,MATRIX_SIZE);
                        //write JPEG footer
                        writejpegfooter(outfile);

                }
                else {
                     printf("\n%s is not a valid BMP-file",argv[1]);
                }

          } else { //decode infile to BMP

            //    readjpegfile(infile,inputbuffer);
            //    huffmancodec(inputbuffer,bitstream,encode);
              //  zzq(pixelmatrix,bitstream,compression,encode);
            //    DCT(dctmatrix,outputbuffer,encode);
            //    writebmp(outputbuffer);*/
          }

        fclose(outfile);
        fclose(infile);
      /*  infile = fopen("test.jpg","rb");
        outfile = fopen("header.h","wb");
        fread(header,389,1,infile);
        for(i=0;i<389;i++)
         fprintf(outfile,"0x%x, ",header[i]);
        fclose(outfile);
        fclose(infile); */
#ifndef __MICROBLAZE		  
        QueryPerformanceCounter(&en);
        printf("\nExecution time: %f seconds",(double)(en.QuadPart-st.QuadPart)/1000000);
#endif		  
        //      free(bmpheader);
  }
  printf("\n\nHit ENTER to close this window.");
  getchar();
  return 0;
}
//---------------------------------------------------------------------------

#endif

