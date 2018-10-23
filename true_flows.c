#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>
#include <errno.h>

#include "util.h"
#include "ringbuffer.h"
#include "packet.h"
#include "alg_keytbl.h"
#include "config.h"
#include "cpu.h"

#include <semaphore.h>

#include "adapter_record_ram.h"

extern conf_t* conf;
extern struct PacketStat packet_stat;

const char* alg = NULL;
int is_output = 0;
int is_pin_cpu = 0;

typedef struct Info {
    int id;
    ringbuffer_t* rb;

    pthread_t thread;
    sem_t sem;
    //pthread_mutex_t mutex;
    //pthread_cond_t cond;
} info_t;

const int n_104 = 104;
int bits_104[104];

void set_bit(unsigned char* a, int pos, int v) {
    int byte = pos / 8;
    int bit = pos % 8;
    if (v == 1) {
        a[byte] = a[byte] | (1<<(7-bit));
    }
    else {
        a[byte] = a[byte] & ~(1<<(7-bit));
    }
}

void normalize_template_key(unsigned char* key, int* bits, int n_bit) {
    for (int j=0; j<bits[0]; j++) {
        set_bit(key, j, 0);
    }
    for (int i=1; i<n_bit; i++) {
        for (int j=bits[i-1]+1; j<bits[i]; j++) {
            set_bit(key, j, 0);
        }
    }
    for (int j=bits[n_bit-1]+1; j<104; j++) {
        set_bit(key, j, 0);
    }
}

static void* thread_handler(void *arg) {
    info_t* args = (info_t*)arg;
    ringbuffer_t* rb = args->rb;

    // Modity: init
    int bit = conf_common_key_len(conf);
    double timeout = conf_keytbl_timeout(conf);
    key_tbl_t* tbl_104 = key_tbl_init(bit, 10000, 100, timeout);

    for (int i=0; i<n_104; i++) {
        bits_104[i] = i;
    }

    tuple_t t;
    memset(&t, 0, sizeof(struct Tuple));
    unsigned long long cnt_pkt = 0;
    unsigned long long cnt_byte = 0;
    unsigned long long cur_interval = 0;
    unsigned long long min_pkt = 0;
    unsigned long long min_byte = 0;
    int min_flow = 0;
    unsigned long long max_pkt = 0;
    unsigned long long max_byte = 0;
    int max_flow = 0;

    if (is_pin_cpu) {
        pin_thread(args->id);
    }
    //pthread_cond_signal(&args->cond);
    sem_post(&args->sem);
    while (1) {
	    while (read_ringbuffer(rb, &t) < 0) {}

        if (t.flag == TUPLE_TERM) {
            break;
        }
        else if (t.flag == TUPLE_PUNC) {

            // Modity: output and reset
            if (is_output) {
                char tmp[100];
                sprintf(tmp, "%s/true_flows/true_%llu", conf_common_trace_dir(conf), cur_interval);
                key_tbl_print(tbl_104, tmp);
                cur_interval++;
            }
            if (cnt_pkt > max_pkt) {
                max_pkt = cnt_pkt;
            }
            if (cnt_byte > max_byte) {
                max_byte = cnt_byte;
            }
            if (tbl_104->num_key > max_flow) {
                max_flow = tbl_104->num_key;
            }
            if (min_pkt==0 || cnt_pkt<min_pkt) {
                min_pkt = cnt_pkt;
            }
            if (min_byte==0 || cnt_byte<min_byte) {
                min_byte = cnt_byte;
            }
            if (min_flow==0 || tbl_104->num_key<min_flow) {
                min_flow = tbl_104->num_key;
            }
            cnt_pkt = 0;
            cnt_byte = 0;
            key_tbl_reset(tbl_104);

            //pthread_cond_signal(&args->cond);
            sem_post(&args->sem);
            continue;
        }

        // Modity: update
        key_tbl_entry_p_t record = key_tbl_record(tbl_104, &t, -1);
        record->tuple.size += t.size;

        cnt_pkt++;
        cnt_byte += t.size;
    }

    // Modity: output and free
    if (is_output) {
        char tmp[100];
        sprintf(tmp, "%s/true_flows/true_%llu", conf_common_trace_dir(conf), cur_interval);
        key_tbl_print(tbl_104, tmp);
    }
    key_tbl_destroy(tbl_104);

    // LOG_MSG("flow: %d - %d, pkt %llu - %llu, byte: %llu - %llu\n", min_flow, max_flow, min_pkt, max_pkt, min_byte, max_byte);

    pthread_exit((void*)cnt_pkt);
}

int main (int argc, char *argv []) {

    int num_thread = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [config file]\n", argv[0]);
        exit(-1);
    }
        
    conf = Config_Init(argv[1]);
    // num_thread = atoi(argv[2]);
    num_thread = 1;

    const char* dir = conf_common_trace_dir(conf);
    const char* filename = conf_common_record_file(conf);
    unsigned long long buf_size = conf_common_trace_bufsize(conf);
    adapter_t* adapter = adapter_init(dir, filename, buf_size);
    char tmp[100];
    sprintf(tmp, "%s/true_flows", dir);
    mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    alg = strstr(argv[0], "_") + 1;
    is_output = conf_common_is_output(conf);
    is_pin_cpu = conf_common_is_pin_cpu(conf);
    if (is_pin_cpu) {
        pin_thread(num_thread);
    }

    info_t* infos = (info_t*)calloc(num_thread, sizeof(info_t));
    for (int i=0; i<num_thread; i++) {
        char tmp[100];
        sprintf(tmp, "rb%02d", i);
        infos[i].rb = create_ringbuffer_shm(tmp, sizeof(tuple_t));
        infos[i].id = i;
    }

	pthread_attr_t attr;
	int s = pthread_attr_init(&attr);
    if (s != 0) {
		LOG_ERR("pthread_attr_init: %s\n", strerror(errno));
	}
	for (int i=0; i<num_thread; i++) {
        //pthread_mutex_init(&infos->mutex, NULL);
        //pthread_cond_init(&infos[i].cond, NULL);
        sem_init(&infos[i].sem, 1, 0);
		s = pthread_create(&infos[i].thread, &attr,
                                  &thread_handler, infos+i);
    	if (s != 0)
			LOG_ERR("pthread_create: %s\n", strerror(errno));
        // wait for ready
        sem_wait(&infos[i].sem);
        //pthread_mutex_lock(&infos[i].mutex);
        //pthread_cond_wait(&infos[i].cond, &infos[i].mutex);
        //pthread_mutex_unlock(&infos[i].mutex);
	}

    tuple_t t;
    memset(&t, 0, sizeof(struct Tuple));

    unsigned long long interval_len = conf_common_interval_len(conf);
    uint64_t start_ts = now_us();
    uint64_t cur_ts;
    int index = 0;
    unsigned long long last_interval = 0;
    while (1) {
        if (adapter_next(adapter, &t) == -1) {
            break;
        }

        unsigned long long cur_interval = (unsigned long long)(t.pkt_ts*1000/interval_len);
        if (last_interval == 0) {
            last_interval = cur_interval;
        }
        else if (cur_interval > last_interval) {
            cur_ts = now_us();
            packet_stat.used_time += cur_ts - start_ts;
    
            // notify threads
            t.flag = TUPLE_PUNC;
            for (int i=0; i<num_thread; i++) {
                //pthread_mutex_init(&infos[i].mutex, NULL);
                //pthread_cond_init(&infos[i].cond, NULL);
                while (write_ringbuffer(infos[i].rb, &t, sizeof(tuple_t)) < 0) {};
                flush_ringbuffer(infos[i].rb);
                sem_wait(&infos[i].sem);
                //pthread_mutex_lock(&infos[i].mutex);
                //pthread_cond_wait(&infos[i].cond, &infos[i].mutex);
                //pthread_mutex_unlock(&infos[i].mutex);
            }
            t.flag = TUPLE_NORMAL;

            last_interval = cur_interval;
            start_ts = now_us();
        }

        while (write_ringbuffer(infos[index].rb, &t, sizeof(tuple_t)) < 0) {};
        index = (index + 1) % num_thread;
    }

    // notify threads
    t.flag = TUPLE_TERM;
    for (int i=0; i<num_thread; i++) {
        while (write_ringbuffer(infos[i].rb, &t, sizeof(tuple_t)) < 0) {};
        flush_ringbuffer(infos[i].rb);
    }

    cur_ts = now_us();
    packet_stat.used_time += cur_ts - start_ts;
    // report_final_stat();

    for (int i=0; i<num_thread; i++) {
        unsigned long long cnt;
        pthread_join(infos[i].thread, (void**)&cnt);
        // LOG_MSG("Thread %d process %llu packets\n", i, cnt);
    }

    adapter_destroy(adapter);

    for (int i=0; i<num_thread; i++) {
        close_ringbuffer_shm(infos[i].rb);
    }
    free(infos);

    return 0;
}
