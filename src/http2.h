enum FrameType {
	DATA = 0,
	HEADERS = 1,
	PRIORITY = 2,
	RST_STREAM = 3,
	SETTINGS = 4,
	PUSH_PROMISE = 5,
	PING = 6,
	GOAWAY = 7,
	WINDOW_UPDATE = 8
};

struct Http2Frame {
	unsigned int length: 24;
	enum FrameType type;
	unsigned char flags;
	unsigned char reserved: 1;
	unsigned long int streamIdentifier: 31;
	char * payload;
};

void send_http2_frame(struct Http2Frame *frame, int sockfd);

int get_http2_frame_size(struct Http2Frame *frame);