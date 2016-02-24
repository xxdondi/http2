#ifndef HPACK_H
#define HPACK_H

typedef unsigned char byte;

size_t hpack_encode_integer(int n, unsigned int number, byte * out);

unsigned int hpack_decode_integer(int n, byte * in);

size_t hpack_get_encoded_string_length(size_t length);

size_t hpack_encode_string(char* string, size_t length, byte * out);

#endif