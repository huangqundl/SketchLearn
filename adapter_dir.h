// #ifdef TRACE_DIR

#include <pcap.h>
#include "config.h"
#include "packet.h"

typedef struct {

    int file_num, file_cur;
    char filename[MAX_FILE][MAX_FILE_LEN];

	pcap_t* pcap;

} adapter_t;

adapter_t* adapter_init(const char* dir, const char* file);
void adapter_destroy(adapter_t*);

int adapter_next(adapter_t* adapter, tuple_t* p, enum PACKET_STATUS* status);

// #endif
