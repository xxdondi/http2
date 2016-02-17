#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "http2.h"

int get_http2_frame_size(struct Http2Frame *frame) {
	// Substract payload pointer size and add actual payload size
	return sizeof(struct Http2Frame) - 4 + frame->length;
}

void send_http2_frame(struct Http2Frame *frame, int sockfd) {
	int frameSize = get_http2_frame_size(frame);
	char * buffer = malloc(frameSize);
	int offset;
	// Length
	// Convert to network byte order
	unsigned int nlength = htonl(frame->length);
	// Copy only 1st 3 bytes of the number
	memcpy(buffer + offset, &nlength, 3);
	offset += 3;
	// Type
	memcpy(buffer + offset, &(frame->type), 1);
	offset += 1;
	// Flags
	memcpy(buffer + offset, &(frame->flags), 1);
	offset += 1;
	// Reserved bit & stream identifier
	// NBO
	unsigned long int nstreamIdentifier = htonl(frame->streamIdentifier);
	// Shift right by 1 bit to add reserved unset bit
	nstreamIdentifier = nstreamIdentifier >> 1;
	// 1 reserved bit & 31 bits of stream identifier
	memcpy(buffer + offset, &nstreamIdentifier, 4);
	offset += 4;
	// Payload
	memcpy(buffer + offset, frame->payload, frame->length);
	// Send the frame
	send(sockfd, buffer, frameSize, 0);
}