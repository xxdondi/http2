#ifndef HPACK_H
#define HPACK_H

typedef unsigned char byte;

size_t hpack_encode_integer(int n, unsigned int number, byte * out);
unsigned int hpack_decode_integer(int n, byte * in);

#endif