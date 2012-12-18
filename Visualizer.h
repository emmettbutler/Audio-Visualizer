#define PACKET_SIZE 256  // packet size in samples
#define BUFFER_SIZE 100  // size of buffer in packets
#define PAC_CHANNELS 2

typedef struct _packet{
    float frames[PACKET_SIZE][PAC_CHANNELS];  // PACKET_SIZE frames
    bool free;  // is this packet free to overwrite
    int order;  // where in the sequence of samples is this packet
} Packet;
