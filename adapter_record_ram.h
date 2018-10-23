#include "tuple.h"

typedef struct {

    unsigned char* trace_buf;
    unsigned char* ptr;
    unsigned long long cnt;
    unsigned long long cur;

} adapter_t;

adapter_t* adapter_init(const char* dir, const char* file, unsigned long long buf_size);
void adapter_destroy(adapter_t*);

int adapter_next(adapter_t* adapter, tuple_t* p);
