#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;

size_t hpack_encode_integer(int n, unsigned int number, char * out) {
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