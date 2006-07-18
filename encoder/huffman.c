#pragma argsused
/*
Only encoder
This version works correctly, it is tested with testcase.jpg
The translation into real huffman codes works.
Changed: If huffman wants to send 0xFFxx (FF in one byte) than there must be 0x00 inserted between FF and xx
possible fault in finish send:
-must it be filled up with zeros?          YES
-must it be filled up to one bye? or 2 byte? --> in this code there is filled up to 2 bytes, but I (joris) thinks this must be filled up to 1 byte.
 still dont know
- 24-11-05 code clean up
- 24-11-05 tables added for color



Block numbers:
Y = 0
cb =1
cr= 2
*/
//---------------------------------------------------------------------------
#pragma hdrstop
#include <stdio.h>
#ifdef __MICROBLAZE
#include "xup2pro.h"
#endif
//===========================================================================
void PrintMatrix(char* pixelmatrix)
{
int i;
//        printf("MATRIX= \n");
//	for (i=0;i<64;i++) printf("%d |",pixelmatrix[i]);
//       	printf("\n");
}
//===========================================================================
void FillMatrix( char* pixelmatrix,int select)
{
        int i;
	if (select== 0)
	{
		for (i=0;i<64;i++) pixelmatrix[i]=0;
                 pixelmatrix[0]=6;
                 pixelmatrix[1]=-4;
                 pixelmatrix[2]=15;
                 pixelmatrix[3]=4;
                 pixelmatrix[7]=-1;
                 pixelmatrix[11]=2;
                 pixelmatrix[12]=2;
                 pixelmatrix[20]=-1;

	}
	else if (select == 1 )
	{
        for (i=0;i<64;i++) pixelmatrix[i]=0;
		pixelmatrix[0]=36;
		pixelmatrix[1]=-1;
		pixelmatrix[2]=1;
		pixelmatrix[3]=-2;
		pixelmatrix[4]=3;
		pixelmatrix[5]=-5;
		pixelmatrix[6]=0;
		pixelmatrix[7]=7;
		pixelmatrix[8]=4;
		pixelmatrix[9]=-10;
		pixelmatrix[12]=8;
       		pixelmatrix[13]=-16;
		pixelmatrix[14]=17;
		pixelmatrix[33]=30;
		pixelmatrix[34]=-63;
		pixelmatrix[35]=-32;
		pixelmatrix[36]=50;
		pixelmatrix[40]=-100;
		pixelmatrix[41]=121;
		pixelmatrix[42]=100;
		pixelmatrix[47]=-3;
		pixelmatrix[48]=-2;
		pixelmatrix[49]=-7;
		pixelmatrix[50]=-15;
		pixelmatrix[51]=3;
		pixelmatrix[52]=7;
	}
}
//===========================================================================
void ConvertDCMagnitudeC(unsigned char magnitude,unsigned short int *out, unsigned short int *lenght)
{
        switch (magnitude) {
                case 0x00 : *out=0x0000; *lenght=2; break;
                case 0x01 : *out=0x0001; *lenght=2; break;
		case 0x02 : *out=0x0002; *lenght=2; break;
		case 0x03 : *out=0x0006; *lenght=3; break;
		case 0x04 : *out=0x000e; *lenght=4; break;
		case 0x05 : *out=0x001e; *lenght=5; break;
		case 0x06 : *out=0x003e; *lenght=6; break;
		case 0x07 : *out=0x007e; *lenght=7; break;
		case 0x08 : *out=0x00fe; *lenght=8; break;
		case 0x09 : *out=0x01fe; *lenght=9; break;
		case 0x0a : *out=0x03fe; *lenght=10; break;
		case 0x0b : *out=0x07fe; *lenght=11; break;
        }
}
//===========================================================================
void ConvertACMagnitudeC(unsigned char magnitude,unsigned short int *out, unsigned short int *lenght)
{
        switch (magnitude) {
case 0x00 :  *lenght=0x02; *out=0x0000; break; //1010
case 0x01 :  *lenght=0x02; *out=0x0001; break; //00
case 0x02 :  *lenght=0x03; *out=0x0004; break; //01
case 0x03 :  *lenght=0x04; *out=0x000a; break; //100
case 0x04 :  *lenght=0x05; *out=0x0018; break; //1011
case 0x05 :  *lenght=0x05; *out=0x0019; break; //11010
case 0x06 :  *lenght=0x06; *out=0x0038; break; //1111000
case 0x07 :  *lenght=0x07; *out=0x0078; break; //11111000
case 0x08 :  *lenght=0x09; *out=0x01f4; break; //1111110110
case 0x09 :  *lenght=0x0a; *out=0x03f6; break; //1111111110000010
case 0x0A :  *lenght=0x0c; *out=0x0ff4; break; //1111111110000011
case 0x11 :  *lenght=0x04; *out=0x000b; break; //1100
case 0x12 :  *lenght=0x06; *out=0x0039; break; //11011
case 0x13 :  *lenght=0x08; *out=0x00f6; break; //1111001
case 0x14 :  *lenght=0x09; *out=0x01f5; break; //111110110
case 0x15 :  *lenght=0x0b; *out=0x07f6; break; //11111110110
case 0x16 :  *lenght=0x0c; *out=0x0ff5; break; //1111111110000100
case 0x17 :  *lenght=0x10; *out=0xff88; break; //1111111110000101
case 0x18 :  *lenght=0x10; *out=0xff89; break; //1111111110000110
case 0x19 :  *lenght=0x10; *out=0xff8a; break; //1111111110000111
case 0x1A :  *lenght=0x10; *out=0xff8b; break; //1111111110001000
case 0x21 :  *lenght=0x05; *out=0x001a; break; //11100
case 0x22 :  *lenght=0x08; *out=0x00f7; break; //11111001
case 0x23 :  *lenght=0x0a; *out=0x03f7; break; //1111110111
case 0x24 :  *lenght=0x0c; *out=0x0ff6; break; //111111110100
case 0x25 :  *lenght=0x0f; *out=0x7fc2; break; //1111111110001001
case 0x26 :  *lenght=0x10; *out=0xff8c; break; //1111111110001010
case 0x27 :  *lenght=0x10; *out=0xff8d; break; //1111111110001011
case 0x28 :  *lenght=0x10; *out=0xff8e; break; //1111111110001100
case 0x29 :  *lenght=0x10; *out=0xff8f; break; //1111111110001101
case 0x2A :  *lenght=0x10; *out=0xff90; break; //1111111110001110
case 0x31 :  *lenght=0x05; *out=0x001b; break; //111010
case 0x32 :  *lenght=0x08; *out=0x00f8; break; //111110111
case 0x33 :  *lenght=0x0a; *out=0x03f8; break; //111111110101
case 0x34 :  *lenght=0x0c; *out=0x0ff7; break; //1111111110001111
case 0x35 :  *lenght=0x10; *out=0xff91; break; //1111111110010000
case 0x36 :  *lenght=0x10; *out=0xff92; break; //1111111110010001
case 0x37 :  *lenght=0x10; *out=0xff93; break; //1111111110010010
case 0x38 :  *lenght=0x10; *out=0xff94; break; //1111111110010011
case 0x39 :  *lenght=0x10; *out=0xff95; break; //1111111110010100
case 0x3A :  *lenght=0x10; *out=0xff96; break; //1111111110010101
case 0x41 :  *lenght=0x06; *out=0x003a; break; //111011
case 0x42 :  *lenght=0x09; *out=0x01f6; break; //1111111000
case 0x43 :  *lenght=0x10; *out=0xff97; break; //1111111110010110
case 0x44 :  *lenght=0x10; *out=0xff98; break; //1111111110010111
case 0x45 :  *lenght=0x10; *out=0xff99; break; //1111111110011000
case 0x46 :  *lenght=0x10; *out=0xff9a; break; //1111111110011001
case 0x47 :  *lenght=0x10; *out=0xff9b; break; //1111111110011010
case 0x48 :  *lenght=0x10; *out=0xff9c; break; //1111111110011011
case 0x49 :  *lenght=0x10; *out=0xff9d; break; //1111111110011100
case 0x4A :  *lenght=0x10; *out=0xff9e; break; //1111111110011101
case 0x51 :  *lenght=0x06; *out=0x003b; break; //1111010
case 0x52 :  *lenght=0x0a; *out=0x03f9; break; //11111110111
case 0x53 :  *lenght=0x10; *out=0xff9f; break; //1111111110011110
case 0x54 :  *lenght=0x10; *out=0xffa0; break; //1111111110011111
case 0x55 :  *lenght=0x10; *out=0xffa1; break; //1111111110100000
case 0x56 :  *lenght=0x10; *out=0xFFA2; break; //1111111110100001
case 0x57 :  *lenght=0x10; *out=0xFFA3; break; //1111111110100010
case 0x58 :  *lenght=0x10; *out=0xFFA4; break; //1111111110100011
case 0x59 :  *lenght=0x10; *out=0xFFA5; break; //1111111110100100
case 0x5A :  *lenght=0x10; *out=0xFFA6; break; //1111111110100101
case 0x61 :  *lenght=0x07; *out=0x0079; break; //1111011
case 0x62 :  *lenght=0x0b; *out=0x07f7; break; //111111110110
case 0x63 :  *lenght=0x10; *out=0xffa7; break; //1111111110100110
case 0x64 :  *lenght=0x10; *out=0xffa8; break; //1111111110100111
case 0x65 :  *lenght=0x10; *out=0xffa9; break; //1111111110101000
case 0x66 :  *lenght=0x10; *out=0xffaa; break; //1111111110101001
case 0x67 :  *lenght=0x10; *out=0xffab; break; //1111111110101010
case 0x68 :  *lenght=0x10; *out=0xFFAc; break; //1111111110101011
case 0x69 :  *lenght=0x10; *out=0xFFAf; break; //1111111110101100
case 0x6A :  *lenght=0x10; *out=0xFFAe; break; //1111111110101101
case 0x71 :  *lenght=0x07; *out=0x007a; break; //11111010
case 0x72 :  *lenght=0x0b; *out=0x07f8; break; //111111110111
case 0x73 :  *lenght=0x10; *out=0xffaf; break; //1111111110101110
case 0x74 :  *lenght=0x10; *out=0xffb0; break; //1111111110101111
case 0x75 :  *lenght=0x10; *out=0xFFB1; break; //1111111110110000
case 0x76 :  *lenght=0x10; *out=0xFFB2; break; //111111110110001
case 0x77 :  *lenght=0x10; *out=0xFFB3; break; //111111110110010
case 0x78 :  *lenght=0x10; *out=0xFFB4; break; //111111110110011
case 0x79 :  *lenght=0x10; *out=0xFFB5; break; //1111111110110100
case 0x7A :  *lenght=0x10; *out=0xFFB6; break; //1111111110110101
case 0x81 :  *lenght=0x08; *out=0x00f9; break; //111111000
case 0x82 :  *lenght=0x10; *out=0xffb7; break; //111111111000000
case 0x83 :  *lenght=0x10; *out=0xFFB8; break; //1111111110110110
case 0x84 :  *lenght=0x10; *out=0xFFB9; break; //1111111110110111
case 0x85 :  *lenght=0x10; *out=0xFFBa; break; //1111111110111000
case 0x86 :  *lenght=0x10; *out=0xFFBb; break; //1111111110111001
case 0x87 :  *lenght=0x10; *out=0xFFBc; break; //1111111110111010
case 0x88 :  *lenght=0x10; *out=0xFFBd; break; //1111111110111011
case 0x89 :  *lenght=0x10; *out=0xFFBe; break; //1111111110111100
case 0x8A :  *lenght=0x10; *out=0xFFBf; break; //1111111110111101
case 0x91 :  *lenght=0x09; *out=0x01f7; break; //111111001
case 0x92 :  *lenght=0x10; *out=0xffc0; break; //1111111110111110
case 0x93 :  *lenght=0x10; *out=0xffc1; break; //1111111110111111
case 0x94 :  *lenght=0x10; *out=0xFFC2; break; //1111111111000000
case 0x95 :  *lenght=0x10; *out=0xFFC3; break; //1111111111000001
case 0x96 :  *lenght=0x10; *out=0xFFC4; break; //1111111111000010
case 0x97 :  *lenght=0x10; *out=0xFFC5; break; //1111111111000011
case 0x98 :  *lenght=0x10; *out=0xFFC6; break; //1111111111000100
case 0x99 :  *lenght=0x10; *out=0xFFC7; break; //1111111111000101
case 0x9A :  *lenght=0x10; *out=0xFFC8; break; //1111111111000110
case 0xA1 :  *lenght=0x09; *out=0x01f8; break; //111111010
case 0xA2 :  *lenght=0x10; *out=0xffc9; break; //1111111111000111
case 0xA3 :  *lenght=0x10; *out=0xFFCa; break; //1111111111001000
case 0xA4 :  *lenght=0x10; *out=0xFFCb; break; //1111111111001001
case 0xA5 :  *lenght=0x10; *out=0xFFCc; break; //1111111111001010
case 0xA6 :  *lenght=0x10; *out=0xFFCd; break; //1111111111001011
case 0xA7 :  *lenght=0x10; *out=0xFFCe; break; //1111111111001100
case 0xA8 :  *lenght=0x10; *out=0xFFCf; break; //1111111111001101
case 0xA9 :  *lenght=0x10; *out=0xFFd0; break; //1111111111001110
case 0xAA :  *lenght=0x10; *out=0xFFd1; break; //1111111111001111
case 0xB1 :  *lenght=0x09; *out=0x01f9; break; //1111111001
case 0xB2 :  *lenght=0x10; *out=0xFFD2; break; //1111111111010000
case 0xB3 :  *lenght=0x10; *out=0xFFD3; break; //1111111111010001
case 0xB4 :  *lenght=0x10; *out=0xFFD4; break; //1111111111010010
case 0xB5 :  *lenght=0x10; *out=0xFFD5; break; //1111111111010011
case 0xB6 :  *lenght=0x10; *out=0xFFD6; break; //1111111111010100
case 0xB7 :  *lenght=0x10; *out=0xFFD7; break; //1111111111010101
case 0xB8 :  *lenght=0x10; *out=0xFFD8; break; //1111111111010110
case 0xB9 :  *lenght=0x10; *out=0xFFD9; break; //1111111111010111
case 0xBA :  *lenght=0x10; *out=0xFFDa; break; //1111111111011000
case 0xC1 :  *lenght=0x09; *out=0x01fa; break; //1111111010
case 0xC2 :  *lenght=0x10; *out=0xFFDb; break; //1111111111011001
case 0xC3 :  *lenght=0x10; *out=0xFFDc; break; //1111111111011010
case 0xC4 :  *lenght=0x10; *out=0xFFDd; break; //1111111111011011
case 0xC5 :  *lenght=0x10; *out=0xFFDe; break; //1111111111011100
case 0xC6 :  *lenght=0x10; *out=0xFFDf; break; //1111111111011101
case 0xC7 :  *lenght=0x10; *out=0xFFe0; break; //1111111111011110
case 0xC8 :  *lenght=0x10; *out=0xFFe1; break; //1111111111011111
case 0xC9 :  *lenght=0x10; *out=0xFFe2; break; //1111111111100000
case 0xCA :  *lenght=0x10; *out=0xFFe3; break; //1111111111100001
case 0xD1 :  *lenght=0x0b; *out=0x07f9; break; //11111111000
case 0xD2 :  *lenght=0x10; *out=0xFFE4; break; //1111111111100010
case 0xD3 :  *lenght=0x10; *out=0xFFE5; break; //1111111111100011
case 0xD4 :  *lenght=0x10; *out=0xFFE6; break; //1111111111100100
case 0xD5 :  *lenght=0x10; *out=0xFFE7; break; //1111111111100101
case 0xD6 :  *lenght=0x10; *out=0xFFE8; break; //1111111111100110
case 0xD7 :  *lenght=0x10; *out=0xFFE9; break; //1111111111100111
case 0xD8 :  *lenght=0x10; *out=0xFFEa; break; //1111111111101000
case 0xD9 :  *lenght=0x10; *out=0xFFEb; break; //1111111111101001
case 0xDA :  *lenght=0x10; *out=0xFFEc; break; //1111111111101010
case 0xE1 :  *lenght=0x0e; *out=0x3fe0; break; //1111111111101011
case 0xE2 :  *lenght=0x10; *out=0xffed; break; //1111111111101100
case 0xE3 :  *lenght=0x10; *out=0xFFEe; break; //1111111111101101
case 0xE4 :  *lenght=0x10; *out=0xFFEf; break; //1111111111101110
case 0xE5 :  *lenght=0x10; *out=0xFFf0; break; //1111111111101111
case 0xE6 :  *lenght=0x10; *out=0xFFF1; break; //1111111111110000
case 0xE7 :  *lenght=0x10; *out=0xFFF2; break; //1111111111110001
case 0xE8 :  *lenght=0x10; *out=0xFFF3; break; //1111111111110010
case 0xE9 :  *lenght=0x10; *out=0xFFF4; break; //1111111111110011
case 0xEA :  *lenght=0x10; *out=0xFFF5; break; //1111111111110100
case 0xF0 :  *lenght=0x0a; *out=0x03fa; break; //11111111001
case 0xF1 :  *lenght=0x0f; *out=0x7fc3; break; //1111111111110101
case 0xF2 :  *lenght=0x10; *out=0xFFF6; break; //1111111111110110
case 0xF3 :  *lenght=0x10; *out=0xFFF7; break; //1111111111110111
case 0xF4 :  *lenght=0x10; *out=0xFFF8; break; //1111111111111000
case 0xF5 :  *lenght=0x10; *out=0xFFF9; break; //1111111111111001
case 0xF6 :  *lenght=0x10; *out=0xFFFA; break; //1111111111111010
case 0xF7 :  *lenght=0x10; *out=0xFFFB; break; //1111111111111011
case 0xF8 :  *lenght=0x10; *out=0xFFFC; break; //1111111111111100
case 0xF9 :  *lenght=0x10; *out=0xFFFD; break; //1111111111111101
case 0xFA :  *lenght=0x10; *out=0xFFFE; break; //1111111111111110
#ifndef __MICROBLAZE
default : printf("WAARDE STAAT NIET IN TABEL!!!!!!!!!!!!!!!!!!!!\n");break;
#endif
        }
  //      printf("magnitude= %x out= %x lenght= %d \n",magnitude,*out,*lenght);
        return;
}
//===========================================================================
void ConvertDCMagnitudeY(unsigned char magnitude,unsigned short int *out, unsigned short int *lenght)
{
        switch (magnitude) {
                case 0x00 : *out=0x0000; *lenght=2; break;
                case 0x01 : *out=0x0002; *lenght=3; break;
		case 0x02 : *out=0x0003; *lenght=3; break;
		case 0x03 : *out=0x0004; *lenght=3; break;
		case 0x04 : *out=0x0005; *lenght=3; break;
		case 0x05 : *out=0x0006; *lenght=3; break;
		case 0x06 : *out=0x000e; *lenght=4; break;
		case 0x07 : *out=0x001e; *lenght=5; break;
		case 0x08 : *out=0x003e; *lenght=6; break;
		case 0x09 : *out=0x007e; *lenght=7; break;
		case 0x0a : *out=0x00fe; *lenght=8; break;
		case 0x0b : *out=0x01fe; *lenght=9; break;
        }
}
//===========================================================================
void ConvertACMagnitudeY(unsigned char magnitude,unsigned short int *out, unsigned short int *lenght)
{
        switch (magnitude) {
case 0x00 :  *lenght=4;  *out=0xFFFA; break; //1010
case 0x01 :  *lenght=2;  *out=0xFFF0; break; //00
case 0x02 :  *lenght=2;  *out=0xFFF1; break; //01
case 0x03 :  *lenght=3;  *out=0xFFF4; break; //100
case 0x04 :  *lenght=4;  *out=0xFFFB; break; //1011
case 0x05 :  *lenght=5;  *out=0xFFFA; break; //11010
case 0x06 :  *lenght=7;  *out=0xFFF8; break; //1111000
case 0x07 :  *lenght=8;  *out=0xFFF8; break; //11111000
case 0x08 :  *lenght=10; *out=0xFFF6; break; //1111110110
case 0x09 :  *lenght=16; *out=0xFF82; break; //1111111110000010
case 0x0A :  *lenght=16; *out=0xFF83; break; //1111111110000011
case 0x11 :  *lenght=4;  *out=0xFFFC; break; //1100
case 0x12 :  *lenght=5;  *out=0xFFFB; break; //11011
case 0x13 :  *lenght=7;  *out=0xFFF9; break; //1111001
case 0x14 :  *lenght=9;  *out=0xFFF6; break; //111110110
case 0x15 :  *lenght=11; *out=0xFFF6; break; //11111110110
case 0x16 :  *lenght=16; *out=0xFF84; break; //1111111110000100
case 0x17 :  *lenght=16; *out=0xFF85; break; //1111111110000101
case 0x18 :  *lenght=16; *out=0xFF86; break; //1111111110000110
case 0x19 :  *lenght=16; *out=0xFF87; break; //1111111110000111
case 0x1A :  *lenght=16; *out=0xFF88; break; //1111111110001000
case 0x21 :  *lenght=5;  *out=0xFFFC; break; //11100
case 0x22 :  *lenght=8;  *out=0xFFF9; break; //11111001
case 0x23 :  *lenght=10; *out=0xFFF7; break; //1111110111
case 0x24 :  *lenght=12; *out=0xFFF4; break; //111111110100
case 0x25 :  *lenght=16; *out=0xFF89; break; //1111111110001001
case 0x26 :  *lenght=16; *out=0xFF8A; break; //1111111110001010
case 0x27 :  *lenght=16; *out=0xFF8B; break; //1111111110001011
case 0x28 :  *lenght=16; *out=0xFF8C; break; //1111111110001100
case 0x29 :  *lenght=16; *out=0xFF8D; break; //1111111110001101
case 0x2A :  *lenght=16; *out=0xFF8E; break; //1111111110001110
case 0x31 :  *lenght=6;  *out=0xFFFA; break; //111010
case 0x32 :  *lenght=9;  *out=0xFFF7; break; //111110111
case 0x33 :  *lenght=12; *out=0xFFF5; break; //111111110101
case 0x34 :  *lenght=16; *out=0xFF8F; break; //1111111110001111
case 0x35 :  *lenght=16; *out=0xFF90; break; //1111111110010000
case 0x36 :  *lenght=16; *out=0xFF91; break; //1111111110010001
case 0x37 :  *lenght=16; *out=0xFF92; break; //1111111110010010
case 0x38 :  *lenght=16; *out=0xFF93; break; //1111111110010011
case 0x39 :  *lenght=16; *out=0xFF94; break; //1111111110010100
case 0x3A :  *lenght=16; *out=0xFF95; break; //1111111110010101
case 0x41 :  *lenght=6;  *out=0xFFFB; break; //111011
case 0x42 :  *lenght=10; *out=0xFFF8; break; //1111111000
case 0x43 :  *lenght=16; *out=0xFF96; break; //1111111110010110
case 0x44 :  *lenght=16; *out=0xFF97; break; //1111111110010111
case 0x45 :  *lenght=16; *out=0xFF98; break; //1111111110011000
case 0x46 :  *lenght=16; *out=0xFF99; break; //1111111110011001
case 0x47 :  *lenght=16; *out=0xFF9A; break; //1111111110011010
case 0x48 :  *lenght=16; *out=0xFF9B; break; //1111111110011011
case 0x49 :  *lenght=16; *out=0xFF9C; break; //1111111110011100
case 0x4A :  *lenght=16; *out=0xFF9D; break; //1111111110011101
case 0x51 :  *lenght=7;  *out=0xFFFA; break; //1111010
case 0x52 :  *lenght=11; *out=0xFFF7; break; //11111110111
case 0x53 :  *lenght=16; *out=0xFF9E; break; //1111111110011110
case 0x54 :  *lenght=16; *out=0xFF9F; break; //1111111110011111
case 0x55 :  *lenght=16; *out=0xFFA0; break; //1111111110100000
case 0x56 :  *lenght=16; *out=0xFFA1; break; //1111111110100001
case 0x57 :  *lenght=16; *out=0xFFA2; break; //1111111110100010
case 0x58 :  *lenght=16; *out=0xFFA3; break; //1111111110100011
case 0x59 :  *lenght=16; *out=0xFFA4; break; //1111111110100100
case 0x5A :  *lenght=16; *out=0xFFA5; break; //1111111110100101
case 0x61 :  *lenght=7;  *out=0xFFFB; break; //1111011
case 0x62 :  *lenght=12; *out=0xFFF6; break; //111111110110
case 0x63 :  *lenght=16; *out=0xFFA6; break; //1111111110100110
case 0x64 :  *lenght=16; *out=0xFFA7; break; //1111111110100111
case 0x65 :  *lenght=16; *out=0xFFA8; break; //1111111110101000
case 0x66 :  *lenght=16; *out=0xFFA9; break; //1111111110101001
case 0x67 :  *lenght=16; *out=0xFFAA; break; //1111111110101010
case 0x68 :  *lenght=16; *out=0xFFAB; break; //1111111110101011
case 0x69 :  *lenght=16; *out=0xFFAC; break; //1111111110101100
case 0x6A :  *lenght=16; *out=0xFFAD; break; //1111111110101101
case 0x71 :  *lenght=8;  *out=0xFFFA; break; //11111010
case 0x72 :  *lenght=12; *out=0xFFF7; break; //111111110111
case 0x73 :  *lenght=16; *out=0xFFAE; break; //1111111110101110
case 0x74 :  *lenght=16; *out=0xFFAF; break; //1111111110101111
case 0x75 :  *lenght=16; *out=0xFFB0; break; //1111111110110000
case 0x76 :  *lenght=16; *out=0xFFB1; break; //111111110110001
case 0x77 :  *lenght=16; *out=0xFFB2; break; //111111110110010
case 0x78 :  *lenght=16; *out=0xFFB3; break; //111111110110011
case 0x79 :  *lenght=16; *out=0xFFB4; break; //1111111110110100
case 0x7A :  *lenght=16; *out=0xFFB5; break; //1111111110110101
case 0x81 :  *lenght=9;  *out=0xFFF8; break; //111111000
case 0x82 :  *lenght=15; *out=0xFFC0; break; //111111111000000
case 0x83 :  *lenght=16; *out=0xFFB6; break; //1111111110110110
case 0x84 :  *lenght=16; *out=0xFFB7; break; //1111111110110111
case 0x85 :  *lenght=16; *out=0xFFB8; break; //1111111110111000
case 0x86 :  *lenght=16; *out=0xFFB9; break; //1111111110111001
case 0x87 :  *lenght=16; *out=0xFFBA; break; //1111111110111010
case 0x88 :  *lenght=16; *out=0xFFBB; break; //1111111110111011
case 0x89 :  *lenght=16; *out=0xFFBC; break; //1111111110111100
case 0x8A :  *lenght=16; *out=0xFFBD; break; //1111111110111101
case 0x91 :  *lenght=9;  *out=0xFFF9; break; //111111001
case 0x92 :  *lenght=16; *out=0xFFBE; break; //1111111110111110
case 0x93 :  *lenght=16; *out=0xFFBF; break; //1111111110111111
case 0x94 :  *lenght=16; *out=0xFFC0; break; //1111111111000000
case 0x95 :  *lenght=16; *out=0xFFC1; break; //1111111111000001
case 0x96 :  *lenght=16; *out=0xFFC2; break; //1111111111000010
case 0x97 :  *lenght=16; *out=0xFFC3; break; //1111111111000011
case 0x98 :  *lenght=16; *out=0xFFC4; break; //1111111111000100
case 0x99 :  *lenght=16; *out=0xFFC5; break; //1111111111000101
case 0x9A :  *lenght=16; *out=0xFFC6; break; //1111111111000110
case 0xA1 :  *lenght=9;  *out=0xFFFA; break; //111111010
case 0xA2 :  *lenght=16; *out=0xFFC7; break; //1111111111000111
case 0xA3 :  *lenght=16; *out=0xFFC8; break; //1111111111001000
case 0xA4 :  *lenght=16; *out=0xFFC9; break; //1111111111001001
case 0xA5 :  *lenght=16; *out=0xFFCA; break; //1111111111001010
case 0xA6 :  *lenght=16; *out=0xFFCB; break; //1111111111001011
case 0xA7 :  *lenght=16; *out=0xFFCC; break; //1111111111001100
case 0xA8 :  *lenght=16; *out=0xFFCD; break; //1111111111001101
case 0xA9 :  *lenght=16; *out=0xFFCE; break; //1111111111001110
case 0xAA :  *lenght=16; *out=0xFFCF; break; //1111111111001111
case 0xB1 :  *lenght=10; *out=0xFFF9; break; //1111111001
case 0xB2 :  *lenght=16; *out=0xFFD0; break; //1111111111010000
case 0xB3 :  *lenght=16; *out=0xFFD1; break; //1111111111010001
case 0xB4 :  *lenght=16; *out=0xFFD2; break; //1111111111010010
case 0xB5 :  *lenght=16; *out=0xFFD3; break; //1111111111010011
case 0xB6 :  *lenght=16; *out=0xFFD4; break; //1111111111010100
case 0xB7 :  *lenght=16; *out=0xFFD5; break; //1111111111010101
case 0xB8 :  *lenght=16; *out=0xFFD6; break; //1111111111010110
case 0xB9 :  *lenght=16; *out=0xFFD7; break; //1111111111010111
case 0xBA :  *lenght=16; *out=0xFFD8; break; //1111111111011000
case 0xC1 :  *lenght=10; *out=0xFFFA; break; //1111111010
case 0xC2 :  *lenght=16; *out=0xFFD9; break; //1111111111011001
case 0xC3 :  *lenght=16; *out=0xFFDA; break; //1111111111011010
case 0xC4 :  *lenght=16; *out=0xFFDB; break; //1111111111011011
case 0xC5 :  *lenght=16; *out=0xFFDC; break; //1111111111011100
case 0xC6 :  *lenght=16; *out=0xFFDD; break; //1111111111011101
case 0xC7 :  *lenght=16; *out=0xFFDE; break; //1111111111011110
case 0xC8 :  *lenght=16; *out=0xFFDF; break; //1111111111011111
case 0xC9 :  *lenght=16; *out=0xFFE0; break; //1111111111100000
case 0xCA :  *lenght=16; *out=0xFFE1; break; //1111111111100001
case 0xD1 :  *lenght=11; *out=0xFFF8; break; //11111111000
case 0xD2 :  *lenght=16; *out=0xFFE2; break; //1111111111100010
case 0xD3 :  *lenght=16; *out=0xFFE3; break; //1111111111100011
case 0xD4 :  *lenght=16; *out=0xFFE4; break; //1111111111100100
case 0xD5 :  *lenght=16; *out=0xFFE5; break; //1111111111100101
case 0xD6 :  *lenght=16; *out=0xFFE6; break; //1111111111100110
case 0xD7 :  *lenght=16; *out=0xFFE7; break; //1111111111100111
case 0xD8 :  *lenght=16; *out=0xFFE8; break; //1111111111101000
case 0xD9 :  *lenght=16; *out=0xFFE9; break; //1111111111101001
case 0xDA :  *lenght=16; *out=0xFFEA; break; //1111111111101010
case 0xE1 :  *lenght=16; *out=0xFFEB; break; //1111111111101011
case 0xE2 :  *lenght=16; *out=0xFFEC; break; //1111111111101100
case 0xE3 :  *lenght=16; *out=0xFFED; break; //1111111111101101
case 0xE4 :  *lenght=16; *out=0xFFEE; break; //1111111111101110
case 0xE5 :  *lenght=16; *out=0xFFEF; break; //1111111111101111
case 0xE6 :  *lenght=16; *out=0xFFF0; break; //1111111111110000
case 0xE7 :  *lenght=16; *out=0xFFF1; break; //1111111111110001
case 0xE8 :  *lenght=16; *out=0xFFF2; break; //1111111111110010
case 0xE9 :  *lenght=16; *out=0xFFF3; break; //1111111111110011
case 0xEA :  *lenght=16; *out=0xFFF4; break; //1111111111110100
case 0xF0 :  *lenght=11; *out=0xFFF9; break; //11111111001
case 0xF1 :  *lenght=16; *out=0xFFF5; break; //1111111111110101
case 0xF2 :  *lenght=16; *out=0xFFF6; break; //1111111111110110
case 0xF3 :  *lenght=16; *out=0xFFF7; break; //1111111111110111
case 0xF4 :  *lenght=16; *out=0xFFF8; break; //1111111111111000
case 0xF5 :  *lenght=16; *out=0xFFF9; break; //1111111111111001
case 0xF6 :  *lenght=16; *out=0xFFFA; break; //1111111111111010
case 0xF7 :  *lenght=16; *out=0xFFFB; break; //1111111111111011
case 0xF8 :  *lenght=16; *out=0xFFFC; break; //1111111111111100
case 0xF9 :  *lenght=16; *out=0xFFFD; break; //1111111111111101
case 0xFA :  *lenght=16; *out=0xFFFE; break; //1111111111111110
#ifndef __MICROBLAZE
default : printf("WAARDE STAAT NIET IN TABEL!!!!!!!!!!!!!!!!!!!!\n");break;
#endif
        }
  //      printf("magnitude= %x out= %x lenght= %d \n",magnitude,*out,*lenght);
        return;
}
//===========================================================================
char Extend (char additional, unsigned char magnitude)
{
        int vt= 1 << (magnitude-1);
        if ( additional < vt ) return (additional + (-1 << magnitude) + 1);
        else return additional;
}
//===========================================================================
void ReverseExtend (char value, unsigned char *magnitude, unsigned char *bits)
{
 //	printf("reverseextend value= %d\n",*magnitude);
	if (value >=0)
	{
		*bits=value;
	}
	else
	{
		value=-value;
		*bits=~value;
	}
	*magnitude=0;
	while (value !=0)
	{
		value>>=1;
		++*magnitude;
	}
 //	printf("reverseextend magnitude= %d bits= %d",magnitude,bits);
	return;
}

static int __count3=0;

//===========================================================================
void WriteRawBits16(unsigned char amount_bits, unsigned int bits, unsigned int *remaining,unsigned char *amount_remaining, FILE* file)     //*remaining needs bo be more than 8 bits because 8 bits could be added and ther ecould already be up ot 7 bits in *remaining
// this function collects bits to send
// if there less than 16 bits collected, nothing is send and these bits are stored in *remaining. In *amount_remaining there is stated how much bits are stored in *remaining
// if more than 16 bits are collected, 16 bits are send and the remaining bits are stored again
{
        unsigned short int send;
        unsigned int mask;
        unsigned char send2;
        int count;

 #ifdef __MICROBLAZE
       if (__count3<16) {
	xil_printf("%x %x, ", amount_bits, bits);
	__count3++;
	if ((__count3&0x07)==0) xil_printf("\r\n");
       	}
#else
       if (__count3<16) {
	printf(" %x %x, ", amount_bits, bits);
	__count3++;
	if ((__count3&0x07)==0) printf("\r\n");
       	}
#endif

		
        mask=0x00;  //init mask
        *remaining=(*remaining<<amount_bits);                                   //shift to make place for the new bits
        for (count=amount_bits; count>0; count--) mask=(mask<<1)|0x01;          //create mask for adding bit
        *remaining=*remaining | (bits&mask);                                    //add bits
        *amount_remaining=*amount_remaining + amount_bits;                      //change *amount_remaining to the correct new value
        if (*amount_remaining >= 16)                                            //are there more than 16 bits in buffer, send 16 bits
        {

#ifndef __MICROBLAZE
if (*amount_remaining >= 32 ) printf("ERROR, more bits to send %d",*amount_remaining);
#endif

                send=*remaining>>(*amount_remaining-16);                        //this value can be send/stored (in art this can be dony by selecting bits)
                send2=(send & 0xFF00) >>8;
#ifdef __MICROBLAZE				
               sysace_fwrite(&send2,1,1,file);
#else
                fwrite(&send2,1,1,file);
#endif
                if (send2==0xFF)
                {
                        send2=0x00;
#ifdef __MICROBLAZE				
                        sysace_fwrite(&send2,1,1,file);
#else
                        fwrite(&send2,1,1,file);
#endif
                }
                send2=send & 0xFF;
#ifdef __MICROBLAZE				
               sysace_fwrite(&send2,1,1,file);
#else
                fwrite(&send2,1,1,file);
#endif
                if (send2==0xFF)
                {
                        send2=0x00;
#ifdef __MICROBLAZE				
                       sysace_fwrite(&send2,1,1,file);
#else
                       fwrite(&send2,1,1,file);
#endif
                }
                *amount_remaining=*amount_remaining-16;                         //descrease by 16 because these are send
        }
        return;
}
//===========================================================================
void HuffmanEncodeFinishSend(unsigned int *remaining,unsigned char *amount_remaining, FILE* file)
// There are still some bits left to send at the end of the 8x8 matrix (or maybe the file),
// the remaining bits are filled up with ones and send
// possible fault: -must it be filled up with ones?
{
        unsigned short int send;
        unsigned int mask;
        int  count;
        mask=0x00;                                                              //init mask
        if (*amount_remaining >= 8)                                           //  //2 bytes to send, send first byte
        {
                send=*remaining>>(*amount_remaining-8);                         //shift so that first byte is ready to send
#ifdef __MICROBLAZE				
                sysace_fwrite(&send,1,1,file);
#else
                fwrite(&send,1,1,file);
#endif
                if (send==0xFF)                                                 //is this still needed????
                {
                        send=0x00;
#ifdef __MICROBLAZE				
                        sysace_fwrite(&send,1,1,file);
#else
                        fwrite(&send,1,1,file);
 #endif
               }
                *amount_remaining=*amount_remaining -8;                         // lower the value to the amount of bits that still needs to be send
        }
        if (*amount_remaining >= 0)                                             //there is a last byte to send
        {
                send=*remaining<<(8-*amount_remaining);                         //shift the last bits to send to the front of the byte
                mask=0x00;                                                      //init mask
                for (count=(8-*amount_remaining); count>0; count--) mask=(mask<<1)|0x01; //create mask to fill byte up with ones
                send=send | mask;                                               //add the ones to the byte
#ifdef __MICROBLAZE				
                sysace_fwrite(&send,1,1,file);
#else
                fwrite(&send,1,1,file);
#endif
                *amount_remaining=0x00;                                         //is this needed?
        }
        return;
}
//===========================================================================
void HuffmanEncodeUsingDCTable(unsigned char magnitude,unsigned int *remaining,unsigned char *amount_remaining, FILE* file)
// Translate magnitude into needed data (from table) and send it
{
        unsigned char send;
        unsigned short int huffmancode, huffmanlengt;
        ConvertDCMagnitudeY(magnitude, &huffmancode, &huffmanlengt);
        WriteRawBits16(huffmanlengt,huffmancode,remaining,amount_remaining, file);
   	//printf("Write DC magnitude= %2x \n",magnitude);
        //WriteRawBits16(0x08,magnitude,remaining,amount_remaining, file);
        return;
}
//===========================================================================
void HuffmanEncodeUsingACTable(unsigned char mag,unsigned int *remaining,unsigned char *amount_remaining, FILE* file)
// Translate magnitude into needed data (from table) and send it
{
        unsigned char send;
        unsigned short int huffmancode, huffmanlengt;
        ConvertACMagnitudeY(mag, &huffmancode, &huffmanlengt);
        WriteRawBits16(huffmanlengt,huffmancode,remaining,amount_remaining, file);
        return;
}
//===========================================================================
char EncodeDataUnit(char dataunit[64], char last_dc_value, FILE* file, unsigned int *remaining, unsigned char *amount_remaining, unsigned int color)
{
	char difference;
        unsigned char magnitude,zerorun,ii,ert;
        unsigned int bits;
	unsigned char bits_char;
                                         //init
  //    PrintMatrix(dataunit) ;
	difference = dataunit[0] - last_dc_value;
	last_dc_value=dataunit[0];
	ReverseExtend(difference, &magnitude,&bits_char);
	bits=bits_char;
	HuffmanEncodeUsingDCTable(magnitude,remaining,amount_remaining, file);
        WriteRawBits16(magnitude,bits,remaining,amount_remaining, file);
	zerorun=0;
	ii=1;
  	while ( ii < 64 )
	{
		if (dataunit[ii] != 0 )
		{
			while ( zerorun >= 16 )
			{
				HuffmanEncodeUsingACTable(0xF0,remaining,amount_remaining,file);
                                zerorun=zerorun-16;
                            //    printf("16 zeros:  %d\n",zerorun);
			}
			ReverseExtend(dataunit[ii],&magnitude,&bits_char);
			bits=bits_char;
                        ert= ((int)zerorun *16);                                     //ERROR !!!!!!!!!!!
                        ert=ert + magnitude;
			HuffmanEncodeUsingACTable(ert,remaining,amount_remaining,file);
			WriteRawBits16(magnitude,bits,remaining,amount_remaining, file);
                        zerorun=0;
		}
		else zerorun=zerorun+1;
                ii++;
	}
	if ( zerorun != 0 )
        {
                HuffmanEncodeUsingACTable(0x00,remaining,amount_remaining,file);
//                printf("NUL DE REST IS NUL\n");
        }
 //       HuffmanEncodeFinishSend(remaining,amount_remaining,file);
        return last_dc_value;
}

#if 0
//===========================================================================
void DecodeDataUnit(char *coefficients[64], char last_dc_value, char value)
{
        char ii;
        char lowbits,highbits,extrabits;
        //decode DC


        //decode AC
        //set coefficients to 0
        for (ii=0;ii<64;ii++) *coefficients[ii]=0x00;     //for the next optimu\ization step: don't write 0 before beginning, these value are overwritten. Only write 0 is you know tha value is 0, now you only write each value once, not twice is the value ain't 0.
        while (ii<64)
        {
//                value= DecodeUsingACTable();
                lowbits = value && 0x0F;
                highbits = (value && 0xF0 ) >>4;

                if (lowbits != 0x00)
                {
//                        extrabits = ReadRawBits(lowbits);
                        ii=ii+highbits;
                        *coefficients[ii]=Extend(extrabits,lowbits);
                        ii++;
                }
                else
                {
                        if (highbits==0xF) ii=ii+16; //run of 16 zeros
                        else if (highbits == 0x0) ii=64; //the rest are zeros
                }
        }
}
//===========================================================================
#endif


