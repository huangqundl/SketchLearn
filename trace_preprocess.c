#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <sys/wait.h>
#include <pthread.h>
#include <errno.h>

#include <semaphore.h>

#include "util.h"
#include "ringbuffer.h"
#include "packet.h"
#include "config.h"
#include "cpu.h"

#include "hash.h"
#include "alg_keytbl.h"

#include "adapter_dir.h"

extern conf_t* conf;
extern struct PacketStat packet_stat;

const char* alg = NULL;
int is_output = 0;
int is_pin_cpu = 0;

int main (int argc, char *argv []) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [config file]\n", argv[0]);
        exit(-1);
    }
        
    conf = Config_Init(argv[1]);

    const char* dir = conf_common_trace_dir(conf);
    const char* pcap_list = conf_common_pcap_list(conf);
    adapter_t* adapter = adapter_init(dir, pcap_list);

    tuple_t t;
    memset(&t, 0, sizeof(struct Tuple));

    uint64_t start_ts = now_us();
    uint64_t cur_ts;

    const char* output_name = conf_common_record_file(conf);
    char tmp[100];
    sprintf(tmp, "%s\%s", dir, output_name);
    FILE* output = fopen(tmp, "wb");
    if (output == NULL) {
		LOG_ERR("cannot open %s: %s\n", output_name, strerror(errno));
    }
    while (1) {
        enum PACKET_STATUS status;
        if (adapter_next(adapter, &t, &status) == -1) {
            break;
        }

        if (status != STATUS_VALID) {
            continue;
        }

        fwrite(&t, sizeof(tuple_t), 1, output);
    }

    cur_ts = now_us();
    packet_stat.used_time += cur_ts - start_ts;
    // report_final_stat();

    adapter_destroy(adapter);
    fclose(output);

    return 0;
}
