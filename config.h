#ifndef __AFS_CONFIG_INCLUDED_HPP__
#define __AFS_CONFIG_INCLUDED_HPP__

#include <stdint.h>

#include <iniparser.h>

#include "util.h"

// wrap iniparser dictionary
typedef struct Config {
    dictionary* dict;
} conf_t;

// create and destroy
conf_t* Config_Init(char* ininame);
void Config_Destroy(conf_t* conf);

// Section names

#define SEC_BF "BloomFilter:"
#define SEC_COMM "Common:"
#define SEC_CM "CountMin:"
#define SEC_CS "CountSketch:"
#define SEC_DEL "Deltoid:"
#define SEC_SKL "SketchLearn:"
#define SEC_FR "FlowRadar:"
#define SEC_UM "UnivMon:"
#define SEC_KEYTBL "KeyTable:"
#define SEC_HEAP "Heap:"
#define SEC_ARRAY "Associative Array:"
#define SEC_LC "LossCount:"
#define SEC_FM "FMSketch:"
#define SEC_REV "RevSketch:"
#define SEC_DOS "DDoS:"
#define SEC_SUPER "SuperSpreader:"
#define SEC_ASM "ASM:"
#define SEC_MRAC "mrac:"
#define SEC_TRUMPET "Trumpet:"
#define SEC_RECOVERY "Recovery:"

#define MAX_FILE 1024
#define MAX_FILE_LEN 1024

// Default parameters: common

#define MAX_CONF_LEN 1024
#define DEF_INI_NAME "config.ini"

#define KEY_COMM_TRACE_DIR "trace_dir"
#define DEF_COMM_TRACE_DIR ""

#define KEY_COMM_TRACE_RECORD_FILE "trace_record_file"
#define DEF_COMM_TRACE_RECORD_FILE ""

#define KEY_COMM_PCAP_LIST "trace_pcap_list"
#define DEF_COMM_PCAP_LIST ""

#define KEY_COMM_TRACE_BUFSIZE "trace_bufsize"
#define DEF_COMM_TRACE_BUFSIZE 10000000

#define KEY_COMM_OUTPUT_TMP_DIR "output_tmp_dir"
#define DEF_COMM_OUTPUT_TMP_DIR ""

#define KEY_COMM_OUTPUT_SUFFIX "output_suffix"
#define DEF_COMM_OUTPUT_SUFFIX "_output"

#define KEY_COMM_KEY_LEN "key_len"
#define DEF_COMM_KEY_LEN 104

#define KEY_COMM_APPRO_SAMPLE_RATE "appro_sample_rate"
#define DEF_COMM_APPRO_SAMPLE_RATE 10000

#define KEY_COMM_INTERVAL_LEN "interval_len"
#define DEF_COMM_INTERVAL_LEN 1000

#define KEY_COMM_BLOCK_PUNC "block_after_punc"
#define DEF_COMM_BLOCK_PUNC 0

#define KEY_COMM_OVS_MAX_CNT "ovs_max_cnt"
#define DEF_COMM_OVS_MAX_CNT 1

#define KEY_COMM_OVS_INTERVAL_CNT "ovs_interval_cnt"
#define DEF_COMM_OVS_INTERVAL_CNT 1

#define KEY_COMM_ALG "alg_name"
#define DEF_COMM_ALG ""

#define KEY_COMM_IS_CPU "is_pin_cpu"
#define DEF_COMM_IS_CPU 0

// Default parameters: Output analysis

#define KEY_COMM_IS_OUTPUT "is_output"
#define DEF_COMM_IS_OUTPUT 1 

#define KEY_COMM_GROUND_DIR "ground_dir"
#define DEF_COMM_GROUND_DIR "./ground/"

#define KEY_COMM_NUM_INTERVAL "num_interval"
#define DEF_COMM_NUM_INTERVAL 1

#define KEY_COMM_ZMQ_DATA_SERVER "zmq_data_server"
#define DEF_COMM_ZMQ_DATA_SERVER ""

#define KEY_COMM_ZMQ_CMD_SERVER "zmq_cmd_server"
#define DEF_COMM_ZMQ_CMD_SERVER ""

#define KEY_COMM_REDIS_IP "redis_ip"
#define DEF_COMM_REDIS_IP ""

#define KEY_COMM_REDIS_PORT "redis_port"
#define DEF_COMM_REDIS_PORT 0

#define KEY_COMM_OVS "is_enable_ovs"
#define DEF_COMM_OVS 0

// Default parameters: Bloom Filter

#define KEY_BF_LEN "length"
#define DEF_BF_LEN 10000

#define KEY_BF_NUM_HASH "num_hash"
#define DEF_BF_NUM_HASH 4

// Default parameters: Count-Min Sketch

#define KEY_CM_DEPTH "depth"
#define DEF_CM_DEPTH 4

#define KEY_CM_WIDTH "width"
#define DEF_CM_WIDTH 10000

#define KEY_CM_FM_DEPTH "fm_depth"
#define DEF_CM_FM_DEPTH 4

#define KEY_CM_FM_WIDTH "fm_width"
#define DEF_CM_FM_WIDTH 1

#define KEY_CM_KMIN_DEPTH "kmin_depth"
#define DEF_CM_KMIN_DEPTH 4

#define KEY_CM_KMIN_WIDTH "kmin_width"
#define DEF_CM_KMIN_WIDTH 1

// Default parameters: CountSketch

#define KEY_CS_DEPTH "depth"
#define DEF_CS_DEPTH 4

#define KEY_CS_WIDTH "width"
#define DEF_CS_WIDTH 10000

#define KEY_CS_HEAP_SIZE "heap_size"
#define DEF_CS_HEAP_SIZE 10

// Default parameters: Deltoid

#define KEY_DEL_DEPTH "depth"
#define DEF_DEL_DEPTH 4

#define KEY_DEL_WIDTH "width"
#define DEF_DEL_WIDTH 10000

#define KEY_DEL_THRESH "thresh"
#define DEF_DEL_THRESH 1

// Default parameters: AutoSketch

#define KEY_SKL_DEPTH "depth"
#define DEF_SKL_DEPTH 1

#define KEY_SKL_WIDTH "width"
#define DEF_SKL_WIDTH 1

#define KEY_SKL_SELECT "select"
#define DEF_SKL_SELECT 0

// Default parameters: FlowRadar

#define KEY_FR_BF_LEN "bloom_filter_len"
#define DEF_FR_BF_LEN 10000

#define KEY_FR_BF_HASH "bloom_filter_num_hash"
#define DEF_FR_BF_HASH 4

#define KEY_FR_CNT_LEN "counter_len"
#define DEF_FR_CNT_LEN 10000

#define KEY_FR_CNT_HASH "counter_num_hash"
#define DEF_FR_CNT_HASH 4

// Default parameters: UnivMon

//#define KEY_UM_LEN "length"
//#define DEF_UM_LEN 64

#define KEY_UM_WIDTH "width"
#define DEF_UM_WIDTH 10000

#define KEY_UM_DEPTH "depth"
#define DEF_UM_DEPTH 4

#define KEY_UM_HEAP_SIZE "heap_size"
#define DEF_UM_HEAP_SIZE 10

// Default parameters: Key table

#define KEY_KEY_LIST_NUM "num_list"
#define DEF_KEY_LIST_NUM 10000

#define KEY_KEY_LIST_LEN "list_len"
#define DEF_KEY_LIST_LEN 10

#define KEY_KEY_TIMEOUT "timeout"
#define DEF_KEY_TIMEOUT -1

// Default parameters: Heap

#define KEY_HEAP_SIZE "size"
#define DEF_HEAP_SIZE 100

// Default parameters: Associative Array

#define KEY_ARRAY_SIZE "size"
#define DEF_ARRAY_SIZE 1

#define KEY_ARRAY_CARD_DEPTH "card_depth"
#define DEF_ARRAY_CARD_DEPTH 1

#define KEY_ARRAY_CARD_WIDTH "card_width"
#define DEF_ARRAY_CARD_WIDTH 1

#define KEY_ARRAY_ERROR_PROB "error_prob"
#define DEF_ARRAY_ERROR_PROB 1

// Default parameters: Heap

#define KEY_LC_SIZE "size"
#define DEF_LC_SIZE 1

#define KEY_LC_EPS "eps"
#define DEF_LC_EPS 1.0

#define KEY_FM_SIZE "num_of_counter"
#define DEF_FM_SIZE 1

#define KEY_REV_DEPTH "depth"
#define DEF_REV_DEPTH 4

#define KEY_REV_WIDTH "width"
#define DEF_REV_WIDTH 10000

#define KEY_REV_KEY_WORD_LEN "key_word_len"
#define DEF_REV_KEY_WORD_LEN 8

#define KEY_REV_HASH_WORD_LEN "hash_word_len"
#define DEF_REV_HASH_WORD_LEN 2

// DDoS

#define KEY_DOS_WIDTH "width"
#define DEF_DOS_WIDTH 10000

#define KEY_DOS_DEPTH "depth"
#define DEF_DOS_DEPTH 1

#define KEY_DOS_CM_WIDTH "cm_width"
#define DEF_DOS_CM_WIDTH 7

#define KEY_DOS_CM_DEPTH "cm_depth"
#define DEF_DOS_CM_DEPTH 1

#define KEY_DOS_REV_WIDTH "rev_width"
#define DEF_DOS_REV_WIDTH 65536

#define KEY_DOS_REV_DEPTH "rev_depth"
#define DEF_DOS_REV_DEPTH 2

#define KEY_DOS_KEY_WORD_LEN "key_word_len"
#define DEF_DOS_KEY_WORD_LEN 16

#define KEY_DOS_HASH_WORD_LEN "hash_word_len"
#define DEF_DOS_HASH_WORD_LEN 8

#define KEY_DOS_THRESH "thresh"
#define DEF_DOS_THRESH 1

// SUPER 

#define KEY_SUPER_WIDTH "width"
#define DEF_SUPER_WIDTH 10000

#define KEY_SUPER_DEPTH "depth"
#define DEF_SUPER_DEPTH 1

#define KEY_SUPER_CM_WIDTH "cm_width"
#define DEF_SUPER_CM_WIDTH 7

#define KEY_SUPER_CM_DEPTH "cm_depth"
#define DEF_SUPER_CM_DEPTH 1

#define KEY_SUPER_REV_WIDTH "rev_width"
#define DEF_SUPER_REV_WIDTH 65536

#define KEY_SUPER_REV_DEPTH "rev_depth"
#define DEF_SUPER_REV_DEPTH 2

#define KEY_SUPER_KEY_WORD_LEN "key_word_len"
#define DEF_SUPER_KEY_WORD_LEN 16

#define KEY_SUPER_HASH_WORD_LEN "hash_word_len"
#define DEF_SUPER_HASH_WORD_LEN 8

#define KEY_SUPER_THRESH "thresh"
#define DEF_SUPER_THRESH 1

#define KEY_ASM_N "n"
#define DEF_ASM_N 1

#define KEY_ASM_K "k"
#define DEF_ASM_K 1

#define KEY_ASM_NUM_STATE "num_state"
#define DEF_ASM_NUM_STATE 4

// Default parameters: Count-Min Sketch

#define KEY_MRAC_DEPTH "depth"
#define DEF_MRAC_DEPTH 1

#define KEY_MRAC_R "r"
#define DEF_MRAC_R 1

#define KEY_MRAC_M "m"
#define DEF_MRAC_M 1

// Default parameters: Trumpet

#define KEY_TRUMPET_SIZE "size"
#define DEF_TRUMPET_SIZE 1

// Recovery

#define KEY_RECOVER_MAX_ITER "max_iter"
#define DEF_RECOVER_MAX_ITER 1

// Get parameters: common

const char* conf_common_trace_dir(conf_t* conf);
const char* conf_common_record_file(conf_t* conf);
const char* conf_common_pcap_list(conf_t* conf);
unsigned long long conf_common_trace_bufsize(conf_t* conf);
unsigned long long conf_common_appro_sample_rate(conf_t* conf);
unsigned long long conf_common_interval_len(conf_t* conf);
int conf_common_block_punc(conf_t* conf);

unsigned long long conf_common_ovs_max_cnt(conf_t* conf);
unsigned long long conf_common_ovs_interval_cnt(conf_t* conf);

int conf_common_key_len(conf_t* conf);

// Get parameters: output analysis

int conf_common_is_output(conf_t* conf);
int conf_common_is_pin_cpu(conf_t* conf);
unsigned long long conf_common_num_interval(conf_t* conf);
const char* conf_common_zmq_data_server(conf_t* conf);
const char* conf_common_zmq_cmd_server(conf_t* conf);
const char* conf_common_redis_ip(conf_t* conf);
int conf_common_redis_port(conf_t* conf);
int conf_common_is_enable_ovs(conf_t* conf);

// Get parameters: Bloom Filter

int conf_bf_length(conf_t* conf);
int conf_bf_num_hash(conf_t* conf);

// Get parameters: Count-Min Sketch

int conf_cm_depth(conf_t* conf);
int conf_cm_width(conf_t* conf);

int conf_cm_fm_depth(conf_t* conf);
int conf_cm_fm_width(conf_t* conf);

int conf_cm_kmin_depth(conf_t* conf);
int conf_cm_kmin_width(conf_t* conf);

// Get parameters: CountSketch

int conf_cs_depth(conf_t* conf);
int conf_cs_width(conf_t* conf);
int conf_cs_heap_size(conf_t* conf);

// Get parameters: Deltoid

int conf_del_depth(conf_t* conf);
int conf_del_width(conf_t* conf);

// Get parameters: AutoSketch

int conf_skl_depth(conf_t* conf);
int conf_skl_width(conf_t* conf);
int conf_skl_select(conf_t* conf);

// Get parameters: FlowRadar

int conf_fr_bloom_filter_len(conf_t* conf);
int conf_fr_bloom_filter_num_hash(conf_t* conf);
int conf_fr_counter_len(conf_t* conf);
int conf_fr_counter_num_hash(conf_t* conf);

// Get parameters: UnivMon

// int conf_univmon_length(conf_t* conf);
int conf_univmon_width(conf_t* conf);
int conf_univmon_depth(conf_t* conf);
int conf_univmon_heap_size(conf_t* conf);

// Get parameters: Key table

int conf_keytbl_num_list(conf_t* conf);
int conf_keytbl_list_len(conf_t* conf);
double conf_keytbl_timeout(conf_t* conf);

// Get parameters: Heap

int conf_heap_size(conf_t* conf);

// Get parameters: Associative Array

int conf_array_size(conf_t* conf);
double conf_array_error_prob(conf_t* conf);
int conf_card_depth(conf_t* conf);
int conf_card_width(conf_t* conf);

// Get parameters: Loss Count 
    
int conf_losscount_size(conf_t* conf);
double conf_losscount_eps(conf_t* conf);

// Get parameters: FM Sketch
int conf_fm_num(conf_t* conf);

// Get parameters: Reversible Sketch
int conf_rev_width(conf_t* conf);
int conf_rev_depth(conf_t* conf);
int conf_rev_key_word_len(conf_t* conf);
int conf_rev_hash_word_len(conf_t* conf);

// Get parameters: DDos
int conf_dos_width(conf_t* conf);
int conf_dos_depth(conf_t* conf);
int conf_dos_cm_width(conf_t* conf);
int conf_dos_cm_depth(conf_t* conf);
int conf_dos_rev_width(conf_t* conf);
int conf_dos_rev_depth(conf_t* conf);
int conf_dos_key_word_len(conf_t* conf);
int conf_dos_hash_word_len(conf_t* conf);
int conf_dos_thresh(conf_t* conf);

// Get parameters: Super
int conf_super_width(conf_t* conf);
int conf_super_depth(conf_t* conf);
int conf_super_cm_width(conf_t* conf);
int conf_super_cm_depth(conf_t* conf);
int conf_super_rev_width(conf_t* conf);
int conf_super_rev_depth(conf_t* conf);
int conf_super_key_word_len(conf_t* conf);
int conf_super_hash_word_len(conf_t* conf);
int conf_super_thresh(conf_t* conf);

// Get parameters: Appro State Machine
int conf_asm_n(conf_t* conf);
int conf_asm_k(conf_t* conf);
int conf_asm_num_state(conf_t* conf);

// other utility
int conf_mrac_depth(conf_t* conf);
int conf_mrac_r(conf_t* conf);
int conf_mrac_m(conf_t* conf);

int conf_trumpet_size(conf_t* conf);

int conf_recovery_max_iter(conf_t* conf);

const char* strconcat(const char* a, const char* b);

#endif
