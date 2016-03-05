#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "hpack.h"

#define STRING_ENCODING_PREFIX_LENGTH 7

unsigned int clz(int x)
{
    unsigned int n = 0;
    const unsigned int bits = sizeof(x) * 8;
    for (int i = 1; i < bits; i ++) {
        if (x < 0) break;
        n ++;
        x <<= 1;
    }
    return n;
}

size_t hpack_get_encoded_integer_length(int n, unsigned int number) {
	byte nbits = 0xFF >> (8 - n);
	if(number < nbits) {
		return 1;
	}
	int bits = (sizeof(number) * 8) - clz(number - nbits); 
	size_t length = bits / 7;
	if(bits % 7 == 0) {
		return 1 + length;
	} else {
		return 2 + length;
	}
}

size_t hpack_encode_integer(int n, unsigned int number, byte * out) {
	// 2^N-1
	byte nbits = 0xFF >> (8 - n);
	// The number fits inside the prefix
	if(number < nbits) {
		*out = (byte)number;
		return 1;
	// The number doesn't fit
	} else {
		int length = 1;
		*out = nbits;
		out++;
		// Substract what we have already written
		number -= nbits;
		while(number >= 128) {
			// Set MSB to 1 (continuation bit)
			// and write to output
			// As stated in Section 5.1:
			// "encode (I % 128 + 128) on 8 bits"
			// Here bitwise operations are used instead of arithmetics
			// number & 0x7F == I % 128
			// | 0x80 == + 128
			*out = ((((byte)number) & 0x7F) | 0x80);
			out++;
			length++;
			// Shift right to next bits
			number >>= 7;
		}
		// The number is less than 128
		// thus MSB is set to 0
		// Write the number to output and return
		*out = (byte)number;
		return length + 1;
	}
}

unsigned int hpack_decode_integer(int n, byte * in) {
	unsigned int result = 0;
	byte prefix = *in;
	byte nbits = 0xFF >> (8 - n);
	// Octet with prefix might contain extra bits
	// we need to unset them
	prefix &= nbits;
	// Now we need to check if all the prefix bits
	// are set
	if((prefix ^ nbits) != 0) {
		// Not all of them are set,
		// return decoded prefix
		return prefix;
	} else {
		byte m = 0;
		result = prefix;
		in++;
		byte next; 
		do {
			next = *in;
			result += (next & 0x7F) * pow(2, m);
			in++;
			m += 7;
		// While continuation bit (MSB) is set	
		} while((next & 0x80) == 0x80);
		return result;
	}
}

size_t hpack_get_encoded_string_length(size_t length) {
	return length + hpack_get_encoded_integer_length(STRING_ENCODING_PREFIX_LENGTH, length);
}

size_t hpack_encode_string(char* str, size_t length, byte * out) {
	byte* start = out;
	size_t written = hpack_encode_integer(STRING_ENCODING_PREFIX_LENGTH, length, out);
	// Unset Huffman bit (Huffman encoding not implemented (yet?))
	*out &= 0x7F;
	out += written;
	memcpy(out, str, length);
	return (start - out + length);
}
				