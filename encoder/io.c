//---------------------------------------------------------------------------
#include <stdio.h>
#ifdef __MICROBLAZE
#include "xup2pro.h"
#endif

#include "io.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma argsused

JPEGHEADER _jpegheader;

unsigned char qtable[64] = {16, 8, 8, 16, 12, 8, 16, 16, 16, 16, 16, 16, 16, 16,
16, 32, 32, 16, 16, 16, 16, 32, 32, 32, 32, 32, 64, 64, 64, 64, 64, 64, 64, 64, 64,
64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128, 64, 64, 128, 128, 128, 128, 128, 64, 64,
128, 128, 128, 128, 128, 64, 128, 128, 128};

unsigned char huffmancount[4][16] = {{0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  //standard DC table count
                                    {0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D},   //standard AC table count
                                    {0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  //standard DC table count
                                    {0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D}};  //standard AC table count

unsigned char huffDCvalues[12] ={0x00,  0x01,  0x02,  0x03,  0x04,  0x05,  0x06,  0x07,  0x08,  0x09,  0x0a,  0x0b};// {0x00, 0x02, 0x03, 0x04, 0x05, 0x06, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E};
unsigned char huffACvalues[162] = {0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22, 0x71,
                                0x14, 0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0, 0x24, 0x33, 0x62, 0x72, 0x82,
                                0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                                0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64,
                                0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85, 0x86, 0x87,
                                0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8,
                                0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9,
                                0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9,
                                0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA};

                                //old huffman table, and propably  a incorrect one.
                                /*{0x00, 0x01, 0x04, 0x0A, 0x0B, 0x0C, 0x1A, 0x1B, 0x1C, 0x3A, 0x3B, 0x78, 0x79, 0x7A, 0x7B, 0xF8, 0xF9, 0xFA, 0xF6,
                                0xF7, 0xF8, 0xF9, 0xFA, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xF6, 0xF7, 0xF8, 0xF9, 0xF4, 0xF5, 0xF6, 0xF7, 0xC0, 0x82,
                                0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95,
                                0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8,
                                0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB,
                                0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE,
                                0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1,
                                0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4,
                                0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE};*/

int getbmpheader(FILE * file, INFOHEADER *header)
{
        int retval;
#ifdef __MICROBLAZE       

//	sysace_fread(header, 1, 14, file);
//       retval = sysace_fread(header, 1, sizeof(INFOHEADER), file);

       memcpy(header, bmpimage+14, sizeof(INFOHEADER));

	header->size = htonl(header->size);
	header->width = htonl(header->width);
	header->height = htonl(header->height);
	header->planes = hton(header->planes);
	header->bits = hton(header->bits);
	header->compression = htonl(header->compression);
	header->imagesize = htonl(header->imagesize);
	header->xresolution = htonl(header->xresolution);
	header->yresolution= htonl(header->yresolution);
	header->ncolours= htonl(header->ncolours);
	header->importantcolours= htonl(header->importantcolours);

	return 1;
	
#else		
        fseek(file,14,SEEK_SET);
        retval = fread(header, sizeof(INFOHEADER), 1, file);
#endif		

        return retval;
}
 /*
int getjpegheader(FILE * file, JPEGHEADER *header)
{
        unsigned int retval;

        return retval;
}

void writebmpheader(FILE * file, BMPHEADER *header)
{
	//write header

	//fill file with 0's
	fwrite(0, 1, header->info.width*header->info.height, file);
}
*/
void writejpegheader(FILE * file, INFOHEADER *header)
{
        JPEGHEADER *jpegheader;
        unsigned int headersize, huffmantablesize, previoussize;
        unsigned char QTcount, i, j, components, id, huffmantablecount;
        unsigned short length, headerlength;

        //Number of Quatization Tables
        QTcount = 2;
        headerlength = 12; //12 bytes are needed for the markers
        huffmantablecount = 4;      //          // 2 AC and 2 DC tables
        huffmantablesize = 0;
        jpegheader = &_jpegheader;//(JPEGHEADER *)malloc(550);

        jpegheader->SOIMarker[0] = 0xff;
        jpegheader->SOIMarker[1] = 0xd8;

        //APP0 segment
        jpegheader->app0.APP0Marker[0] = 0xff;
        jpegheader->app0.APP0Marker[1] = 0xe0;

        headerlength += 16; //APP0 marker is always 16 bytes long
        jpegheader->app0.Length[0] = 0x00;
        jpegheader->app0.Length[1] = 0x10;
        jpegheader->app0.Identifier[0] = 0x4a;
        jpegheader->app0.Identifier[1] = 0x46;
        jpegheader->app0.Identifier[2] = 0x49;
        jpegheader->app0.Identifier[3] = 0x46;
        jpegheader->app0.Identifier[4] = 0x00;
        jpegheader->app0.Version[0] = 0x01;
        jpegheader->app0.Version[1] = 0x00;
        jpegheader->app0.Units = 0x00;
        jpegheader->app0.XDensity[0] = 0x00;
        jpegheader->app0.XDensity[1] = 0x01;
        jpegheader->app0.YDensity[0] = 0x00;
        jpegheader->app0.YDensity[1] = 0x01;
        jpegheader->app0.ThumbWidth = 0x00;
        jpegheader->app0.ThumbHeight = 0x00;

        //Quantization Table Segment
        jpegheader->qt.QTMarker[0] = 0xff;
        jpegheader->qt.QTMarker[1] = 0xdb;
        length = (QTcount<<6) + QTcount + 2;
        headerlength += length;
        jpegheader->qt.Length[0] = (length & 0xff00)>>8;
        jpegheader->qt.Length[1] = length & 0xff;
       // jpegheader->qt.QTInfo = 0x00; // index = 0, precision = 0
        //write Quantization table to header
        i = 0;
    /*     jpegheader->qt.QTInfo[0] = 0;
        for(i=0;i<64;i++) {
                jpegheader->qt.QTInfo[i+1] = qtable[i];
        }
        jpegheader->qt.QTInfo[65] = 1;
        for(i=0;i<64;i++) {
                jpegheader->qt.QTInfo[i+66] = qtable[i];
        }  */
        for (id=0; id<QTcount; id++) {
                jpegheader->qt.QTInfo[(id<<6)+id] = id;
                for(i=0;i<64;i++) {
                        jpegheader->qt.QTInfo[i+1+id+(id<<6)] = qtable[i];
                }
        }

        //Start of Frame segment
        jpegheader->sof0.SOF0Marker[0] = 0xff;
        jpegheader->sof0.SOF0Marker[1] = 0xc0;
        if(header->bits == 8) {
                components = 0x01;
        }
        else {
                components = 0x03;
        }
        length = 8 + 3*components;
        headerlength += length;
        jpegheader->sof0.Length[0] = (length & 0xff00) >> 8;
        jpegheader->sof0.Length[1] = length & 0xff;
        jpegheader->sof0.DataPrecision = 0x08;
        jpegheader->sof0.ImageHeight[0] = (header->height & 0xff00) >> 8;
        jpegheader->sof0.ImageHeight[1] = header->height & 0xff;
        jpegheader->sof0.ImageWidth[0] = (header->width & 0xff00) >> 8;
        jpegheader->sof0.ImageWidth[1] = header->width & 0xff;
#ifdef __MICROBLAZE
	xil_printf("--->%d %d %d %d\r\n", jpegheader->sof0.ImageHeight[0], jpegheader->sof0.ImageHeight[1], jpegheader->sof0.ImageWidth[0], jpegheader->sof0.ImageWidth[1]);
#else
	printf("--->%d %d %d %d\r\n", jpegheader->sof0.ImageHeight[0], jpegheader->sof0.ImageHeight[1], jpegheader->sof0.ImageWidth[0], jpegheader->sof0.ImageWidth[1]);
#endif
        jpegheader->sof0.Components  = components;
        for (i=0; i < components; i++) {
                jpegheader->sof0.ComponentInfo[i][0] = i+1; //color component
                jpegheader->sof0.ComponentInfo[i][1] = 0x11; //no sampling has been done
                jpegheader->sof0.ComponentInfo[i][2] = (i==0)? 0x00 : 0x01; //quantization table ID
        }
        //Start of Huffman Table Segment

        jpegheader->ht.HTMarker[0] = 0xff;
        jpegheader->ht.HTMarker[1] = 0xc4;

        //Set dummy HT segment length
        length = 0;//tablecount*17;
        jpegheader->ht.Length[0] = (length & 0xff00) >> 8;
        jpegheader->ht.Length[1] = length & 0xff;
        previoussize = 0;
        for (id=0; id < huffmantablecount; id++) {
            huffmantablesize = 0;
            switch (id) {
            case 0 : jpegheader->ht.HuffmanInfo[previoussize] = 0x00;
                     break;
            case 1 : jpegheader->ht.HuffmanInfo[previoussize] = 0x10;
                     break;
            case 2 : jpegheader->ht.HuffmanInfo[previoussize] = 0x01;
                     break;
            case 3 : jpegheader->ht.HuffmanInfo[previoussize] = 0x11;
                     break;
            }
            for (i=1; i <= 16; i++) {
                    jpegheader->ht.HuffmanInfo[i+previoussize] =  huffmancount[id][i-1];
                    huffmantablesize += huffmancount[id][i-1];
            }

            for (i=0; i < huffmantablesize; i++) {
                    jpegheader->ht.HuffmanInfo[i+previoussize+17] = (id%2 == 1)? huffACvalues[i] : huffDCvalues[i];
            }
            previoussize += huffmantablesize + 17;
        }
        //Set real HT segment length
        length = 2+previoussize;
        headerlength += length;
        jpegheader->ht.Length[0] = (length & 0xff00) >> 8;
        jpegheader->ht.Length[1] = length & 0xff;
        //Reset marker segment
      /*  jpegheader->dri.DRIMarker[0] = 0xff;
        jpegheader->dri.DRIMarker[1] = 0xdd;
        jpegheader->dri.Length[0] = 0x00;
        jpegheader->dri.Length[1] = 0x04;
        jpegheader->dri.RestartInteral[0] = 0x00; //no restart markers
        jpegheader->dri.RestartInteral[1] = 0x00; //no restart markers
        headerlength  += 6;  //length of DRI segment
       */
        //Start of Scan Header Segment
        jpegheader->sos.SOSMarker[0] = 0xff;
        jpegheader->sos.SOSMarker[1] = 0xda;
        length = 6 + (components<<1);
        headerlength += length;
        jpegheader->sos.Length[0] = (length & 0xff00) >> 8;
        jpegheader->sos.Length[1] =  length & 0xff;
        jpegheader->sos.ComponentCount = components; //number of color components in the image
        jpegheader->sos.Component[0][0] = 0x01; //Y component
        jpegheader->sos.Component[0][1] = 0x00; //indexes of huffman tables for Y-component
        if (components == 0x03) {
                jpegheader->sos.Component[1][0] = 0x02; //the CB component
                jpegheader->sos.Component[1][1] = 0x11; //indexes of huffman tables for CB-component
                jpegheader->sos.Component[2][0] = 0x03; //The CR component
                jpegheader->sos.Component[2][1] = 0x11; //indexes of huffman tables for CR-component
        }
        //following bytes are ignored since progressive scan is not to be implemented
        jpegheader->sos.Ignore[0] = 0x00;
        jpegheader->sos.Ignore[1] = 0x3f;
        jpegheader->sos.Ignore[2] = 0x00;

#ifdef __MICROBLAZE		
	sysace_fwrite(jpegheader, 1, headerlength, file);
	xil_printf("jpeg header size %x\r\n", headerlength);
#else
        fwrite(jpegheader,headerlength,1,file);
	printf("jpeg header size %x\r\n", headerlength);
#endif

}

void writejpegfooter(FILE * file)
{
        unsigned char footer[2];
        footer[0] = 0xff;
        footer[1] = 0xd9;
#ifdef __MICROBLAZE
	sysace_fwrite(footer, 1, sizeof(footer), file);
#else
//        fseek(file,0,SEEK_END);
        fwrite(footer,sizeof(footer),1,file);
#endif		
}

static  unsigned char buffer[MATRIX_SIZE*3];
static int __count2=0;

void readbmpfile(FILE * file, signed char pixelmatrix[MATRIX_SIZE][MATRIX_SIZE*3], unsigned int mrow, unsigned int mcol, INFOHEADER * header)
{
        unsigned int row, col;
	int offset;
	
        for(row = 0;row < MATRIX_SIZE; row++) {
                //Find first point of row in the matrix to be read.
#ifdef __MICROBLAZE                
//               sysace_fread(buffer, 1, MATRIX_SIZE*3, file);
		offset = bmpsize-3*header->width*(row + 1 + mrow*MATRIX_SIZE)+MATRIX_SIZE*3*mcol;
		memcpy(buffer, bmpimage + offset, MATRIX_SIZE*3);
#else
                fseek(file,-(3*header->width*(row + 1 + mrow*MATRIX_SIZE)-(MATRIX_SIZE*3)*mcol),SEEK_END);
                //Read row from matrix
                fread(buffer, 1, MATRIX_SIZE*3, file);
                //copy row into pixelmatrix
#endif                
                for(col = 0; col < MATRIX_SIZE*3; col++) {
                        pixelmatrix[row][col] = buffer[col]- 128;
#if 0						
#ifdef __MICROBLAZE
       if (__count2<4*24*8) {
	xil_printf("%x ", pixelmatrix[row][col]);
	__count2++;
	if ((__count2&0x0f)==0) xil_printf("\r\n");
       	}
#else
       if (__count2<4*24*8) {
	printf(" %x ", pixelmatrix[row][col]);
	__count2++;
	if ((__count2&0x0f)==0) printf("\r\n");
       	}
#endif
#endif						
                }
        }
 }
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

static int __count1=120;

void RGB2YCrCb(signed char pixelmatrix[MATRIX_SIZE][MATRIX_SIZE*3],signed char YMatrix[MATRIX_SIZE][MATRIX_SIZE],signed char CrMatrix[MATRIX_SIZE][MATRIX_SIZE],signed char CbMatrix[MATRIX_SIZE][MATRIX_SIZE])
{
        unsigned int row, col;
        for(row = 0;row < MATRIX_SIZE; row++) {
                for(col = 0; col < MATRIX_SIZE; col++) {
                        YMatrix[row][col] = RGB2Y(pixelmatrix[row][col*3+2],pixelmatrix[row][col*3+1],pixelmatrix[row][col*3]) - 128;
                        CrMatrix[row][col] = RGB2Cr(pixelmatrix[row][col*3+2],pixelmatrix[row][col*3+1],pixelmatrix[row][col*3]) - 128;
                        CbMatrix[row][col] = RGB2Cb(pixelmatrix[row][col*3+2],pixelmatrix[row][col*3+1],pixelmatrix[row][col*3]) - 128;
#if 0						
#ifdef __MICROBLAZE
       if (__count1<64) {
	xil_printf("-------------->%x %x %x\r\n", YMatrix[row][col], CrMatrix[row][col], CbMatrix[row][col]);
	__count1++;
       	}
#else
       if (__count1<64) {
	printf("-------------->%x %x %x\r\n", YMatrix[row][col], CrMatrix[row][col], CbMatrix[row][col]);
	__count1++;
       	}
#endif
#endif
                }
        }
}
/*
void readjpegfile(FILE * file, unsigned char bitstream[])
{
}


void writebmpfile(FILE * file, unsigned char pixelmatrix[MATRIX_SIZE][MATRIX_SIZE], unsigned int mrow, unsigned int mcol, unsigned int width)
{
        unsigned int row;
        for(row = 0;row < MATRIX_SIZE; row++) {
                //Find first point of row in the matrix to be read.
                fseek(file,-(width*(row + 1 + mrow*MATRIX_SIZE)-(MATRIX_SIZE*mcol)),SEEK_END);
                //Read row from matrix
                fwrite(pixelmatrix[row], 1, MATRIX_SIZE, file);
        }
}

void writejpegfile(FILE * file, unsigned char bitstream[])
{
}    */
