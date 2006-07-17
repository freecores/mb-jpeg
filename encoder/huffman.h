#ifndef _HUFFMAN_H
#define _HUFFMAN_H 1

char EncodeDataUnit(char dataunit[NUMBER_OF_PIXELS], char last_dc_value, FILE* file, unsigned int *remaining, unsigned char *amount_remaining, unsigned int component);

void HuffmanEncodeFinishSend(unsigned int *remaining,unsigned char *amount_remaining, FILE* file);

#else
#error "ERROR file huffman.h multiple times included"
#endif /* --- _HUFFMAN_H --- */

