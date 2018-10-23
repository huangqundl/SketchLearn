#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "adapter_record_ram.h"
#include "config.h"

adapter_t* adapter_init(const char* dir, const char* file, unsigned long long buf_size) {
    adapter_t* ret = (adapter_t*)calloc(1, sizeof(adapter_t));

    ret->trace_buf = (unsigned char*)calloc(buf_size, sizeof(unsigned char));
    ret->ptr = ret->trace_buf;

	FILE* input = fopen(strconcat(dir, file), "rb");
    if (input == NULL) {
		LOG_ERR("cannot open %s: %s\n", strconcat(dir, file), strerror(errno));
	}

    uint8_t* p = ret->trace_buf;
    while (1) {
        if (p+sizeof(tuple_t) <= ret->trace_buf+buf_size) {
            int r = fread (p, sizeof(tuple_t), 1, input); 
            if (r != 1) {
                break;
            }
            ret->cnt++;
            p = p + sizeof(tuple_t);
        }
        else {
            break;
        }
    }
    fclose(input);

    // LOG_MSG("Allocate buf: %llu, actual use: %lu, read packet %llu\n",
    //        buf_size, p - ret->trace_buf, ret->cnt);

    return ret;
}

void adapter_destroy(adapter_t* adapter) {
    free(adapter->trace_buf);
}

int adapter_next(adapter_t* adapter, tuple_t* p) {
    if (adapter->cur == adapter->cnt) {
        return -1;
    }

    adapter->cur++;
    memcpy(p, adapter->ptr, sizeof(tuple_t));
    adapter->ptr += sizeof(tuple_t);

    return 0;
}
