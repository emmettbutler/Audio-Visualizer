#define PACKET_SIZE 256  // packet size in samples
#define BUFFER_SIZE 100  // size of buffer in packets

typedef struct _packet{
    float samples[PACKET_SIZE];  // PACKET_SIZE samples
    int free;  // is this packet free to overwrite
    int order;  // where in the sequence of samples is this packet
} Packet;
