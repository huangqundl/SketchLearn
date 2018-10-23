#include <string.h>

#include "config.h"

const char* strconcat(const char* a, const char* b) {
    static char l[MAX_CONF_LEN+1];

    if (a == NULL)
        return b;
    if (b == NULL)
        return NULL;

    if (strlen(a) + strlen(b) >= MAX_CONF_LEN) {
        LOG_ERR("error: concat names too long\n");
    }

    strcpy(l, a);
    strcat(l, b);

    return l;
}

/*****************
 * 
 * Wrap iniparser
 *
 ****************/

void check_instance() {
    /*
    if (conf == NULL) {
        conf = Config_Init(NULL);
    }
    */
}

char* getstring( dictionary* dict, const char* sec, const char* key, const char* def) {
    check_instance();
    return (char*)iniparser_getstring(dict, strconcat(sec, key), (char*)def);
}

int getint(dictionary* dict, const char* sec, const char* key, int def) {
    check_instance();
    return iniparser_getint(dict, strconcat(sec, key), def);
}

unsigned long long getull(dictionary* dict, const char* sec, const char* key, 
        unsigned long long def) {

    check_instance();
    const char* str = iniparser_getstring(dict, strconcat(sec, key), NULL);
    if (str == NULL) return def;
    return strtoull(str, NULL, 10);
}

double getdouble(dictionary* dict, const char* sec, const char* key, double def) {
    check_instance();
    return iniparser_getdouble(dict, strconcat(sec, key), def);
}

int getboolean(dictionary* dict, const char* sec, const char* key, int def) {
    check_instance();
    return iniparser_getboolean(dict, strconcat(sec, key), def);
}

/********************
 *
 * Create and Destroy
 *
 *******************/

conf_t* Config_Init(char* ininame) {

    conf_t* ret = (conf_t*)calloc(1, sizeof(conf_t));
    if (ret == NULL)
        LOG_ERR("error: allocate config error\n");

    if (ininame) {
        ret->dict = iniparser_load(ininame);
    }
    else {
        ret->dict = iniparser_load(DEF_INI_NAME);
    }

    if (ret->dict == 0) {
        LOG_ERR("error: iniparser_load()\n");
    }

    return ret;
}

void Config_Destroy(conf_t* conf) {
    if (conf == NULL)
        return;

    if (conf->dict) {
        iniparser_freedict(conf->dict);
    }
    free(conf);
}

/*******************************************
 *
 * rountes to get config parameters (common)
 *
 ******************************************/

const char* conf_common_trace_dir(conf_t* conf) {
    return getstring(conf->dict, SEC_COMM, KEY_COMM_TRACE_DIR, DEF_COMM_TRACE_DIR);
}

const char* conf_common_record_file(conf_t* conf) {
    return getstring(conf->dict, SEC_COMM, KEY_COMM_TRACE_RECORD_FILE, DEF_COMM_TRACE_RECORD_FILE);
}

const char* conf_common_pcap_list(conf_t* conf) {
    return getstring(conf->dict, SEC_COMM, KEY_COMM_PCAP_LIST, DEF_COMM_PCAP_LIST);
}

unsigned long long conf_common_trace_bufsize(conf_t* conf) {
    return getull(conf->dict, SEC_COMM, KEY_COMM_TRACE_BUFSIZE, DEF_COMM_TRACE_BUFSIZE);
}

int conf_common_key_len(conf_t* conf) {
    return getint(conf->dict, SEC_COMM, KEY_COMM_KEY_LEN, DEF_COMM_KEY_LEN);
}

unsigned long long conf_common_appro_sample_rate(conf_t* conf) {
    return getull(conf->dict, SEC_COMM,
            KEY_COMM_APPRO_SAMPLE_RATE,
            DEF_COMM_APPRO_SAMPLE_RATE);
}

unsigned long long conf_common_interval_len(conf_t* conf) {
    return getull(conf->dict, SEC_COMM,
            KEY_COMM_INTERVAL_LEN,
            DEF_COMM_INTERVAL_LEN);
}

int conf_common_block_punc(conf_t* conf) {
    return getint(conf->dict, SEC_COMM, KEY_COMM_BLOCK_PUNC, DEF_COMM_BLOCK_PUNC);
}

unsigned long long conf_common_ovs_max_cnt(conf_t* conf) {
    return getull(conf->dict, SEC_COMM,
            KEY_COMM_OVS_MAX_CNT,
            DEF_COMM_OVS_MAX_CNT);
}

unsigned long long conf_common_ovs_interval_cnt(conf_t* conf) {
    return getull(conf->dict, SEC_COMM,
            KEY_COMM_OVS_INTERVAL_CNT,
            DEF_COMM_OVS_INTERVAL_CNT);
}

/*******************************************
 *
 * rountes to get config parameters (output analysis)
 *
 ******************************************/

int conf_common_is_output(conf_t* conf) {
    return getull(conf->dict, SEC_COMM,
            KEY_COMM_IS_OUTPUT,
            DEF_COMM_IS_OUTPUT);
}

int conf_common_is_pin_cpu(conf_t* conf) {
    return getull(conf->dict, SEC_COMM,
            KEY_COMM_IS_CPU,
            DEF_COMM_IS_CPU);
}

unsigned long long conf_common_num_interval(conf_t* conf) {
    return getull(conf->dict, SEC_COMM,
            KEY_COMM_NUM_INTERVAL,
            DEF_COMM_NUM_INTERVAL);
}

const char* conf_common_alg(conf_t* conf) {
    return getstring(conf->dict, SEC_COMM,
            KEY_COMM_ALG, DEF_COMM_ALG);
}

const char* conf_common_zmq_data_server(conf_t* conf) {
    return getstring(conf->dict, SEC_COMM,
            KEY_COMM_ZMQ_DATA_SERVER, DEF_COMM_ZMQ_DATA_SERVER);
}

const char* conf_common_zmq_cmd_server(conf_t* conf) {
    return getstring(conf->dict, SEC_COMM,
            KEY_COMM_ZMQ_CMD_SERVER, DEF_COMM_ZMQ_CMD_SERVER);
}

const char* conf_common_redis_ip(conf_t* conf) {
    return getstring(conf->dict, SEC_COMM,
            KEY_COMM_REDIS_IP, DEF_COMM_REDIS_IP);
}

int conf_common_redis_port(conf_t* conf) {
    return getint(conf->dict, SEC_COMM,
            KEY_COMM_REDIS_PORT, DEF_COMM_REDIS_PORT);
}

int conf_common_is_enable_ovs(conf_t* conf) {
    return getint(conf->dict, SEC_COMM,
            KEY_COMM_OVS, DEF_COMM_OVS);
}

/*******************************************
 *
 * rountes to get config parameters (Count-Min Sketch)
 *
 ******************************************/

int conf_bf_length(conf_t* conf) {
    return getint(conf->dict, SEC_BF, KEY_BF_LEN, DEF_BF_LEN);
}

int conf_bf_num_hash(conf_t* conf) {
    return getint(conf->dict, SEC_BF, KEY_BF_NUM_HASH, DEF_BF_NUM_HASH);
}

/*******************************************
 *
 * rountes to get config parameters (Count-Min Sketch)
 *
 ******************************************/

int conf_cm_depth(conf_t* conf) {
    return getint(conf->dict, SEC_CM, KEY_CM_DEPTH, DEF_CM_DEPTH);
}

int conf_cm_width(conf_t* conf) {
    return getint(conf->dict, SEC_CM, KEY_CM_WIDTH, DEF_CM_WIDTH);
}

int conf_cm_fm_depth(conf_t* conf) {
    return getint(conf->dict, SEC_CM, KEY_CM_FM_DEPTH, DEF_CM_FM_DEPTH);
}

int conf_cm_fm_width(conf_t* conf) {
    return getint(conf->dict, SEC_CM, KEY_CM_FM_WIDTH, DEF_CM_FM_WIDTH);
}

int conf_cm_kmin_depth(conf_t* conf) {
    return getint(conf->dict, SEC_CM, KEY_CM_KMIN_DEPTH, DEF_CM_KMIN_DEPTH);
}

int conf_cm_kmin_width(conf_t* conf) {
    return getint(conf->dict, SEC_CM, KEY_CM_KMIN_WIDTH, DEF_CM_KMIN_WIDTH);
}

/*******************************************
 *
 * rountes to get config parameters (CountSketch)
 *
 ******************************************/

int conf_cs_depth(conf_t* conf) {
    return getint(conf->dict, SEC_CS, KEY_CS_DEPTH, DEF_CS_DEPTH);
}

int conf_cs_width(conf_t* conf) {
    return getint(conf->dict, SEC_CS, KEY_CS_WIDTH, DEF_CS_WIDTH);
}

int conf_cs_heap_size(conf_t* conf) {
    return getint(conf->dict, SEC_CS, KEY_CS_HEAP_SIZE, DEF_CS_HEAP_SIZE);
}

/*******************************************
 *
 * rountes to get config parameters (Deltoid)
 *
 ******************************************/

int conf_del_depth(conf_t* conf) {
    return getint(conf->dict, SEC_DEL, KEY_DEL_DEPTH, DEF_DEL_DEPTH);
}

int conf_del_width(conf_t* conf) {
    return getint(conf->dict, SEC_DEL, KEY_DEL_WIDTH, DEF_DEL_WIDTH);
}

long long conf_del_thresh(conf_t* conf) {
    return getint(conf->dict, SEC_DEL, KEY_DEL_THRESH, DEF_DEL_THRESH);
}

/*******************************************
 *
 * rountes to get config parameters (AutoSketch)
 *
 ******************************************/

int conf_skl_depth(conf_t* conf) {
    return getint(conf->dict, SEC_SKL, KEY_SKL_DEPTH, DEF_SKL_DEPTH);
}

int conf_skl_width(conf_t* conf) {
    return getint(conf->dict, SEC_SKL, KEY_SKL_WIDTH, DEF_SKL_WIDTH);
}

int conf_skl_select(conf_t* conf) {
    return getint(conf->dict, SEC_SKL, KEY_SKL_SELECT, DEF_SKL_SELECT);
}

/*******************************************
 *
 * rountes to get config parameters (FlowRadar)
 *
 ******************************************/

int conf_fr_bloom_filter_len(conf_t* conf) {
    return getint(conf->dict, SEC_FR, KEY_FR_BF_LEN, DEF_FR_BF_LEN);
}

int conf_fr_bloom_filter_num_hash(conf_t* conf) {
    return getint(conf->dict, SEC_FR, KEY_FR_BF_HASH, DEF_FR_BF_HASH);
}

int conf_fr_counter_len(conf_t* conf) {
    return getint(conf->dict, SEC_FR, KEY_FR_CNT_LEN, DEF_FR_CNT_LEN);
}

int conf_fr_counter_num_hash(conf_t* conf) {
    return getint(conf->dict, SEC_FR, KEY_FR_CNT_HASH, DEF_FR_CNT_HASH);
}

/*******************************************
 *
 * rountes to get config parameters (UnivMon)
 *
 ******************************************/

//int conf_univmon_length(conf_t* conf) {
//    return getint(conf->dict, SEC_UM, KEY_UM_LEN, DEF_UM_LEN);
//}

int conf_univmon_width(conf_t* conf) {
    return getint(conf->dict, SEC_UM, KEY_UM_WIDTH, DEF_UM_WIDTH);
}

int conf_univmon_depth(conf_t* conf) {
    return getint(conf->dict, SEC_UM, KEY_UM_DEPTH, DEF_UM_DEPTH);
}

int conf_univmon_heap_size(conf_t* conf) {
    return getdouble(conf->dict, SEC_UM, KEY_UM_HEAP_SIZE, DEF_UM_HEAP_SIZE);
}

/*******************************************
 *
 * rountes to get config parameters (KeyTable)
 *
 ******************************************/

int conf_keytbl_num_list(conf_t* conf) {
    return getint(conf->dict, SEC_KEYTBL, KEY_KEY_LIST_NUM, DEF_KEY_LIST_NUM);
}

int conf_keytbl_list_len(conf_t* conf) {
    return getint(conf->dict, SEC_KEYTBL, KEY_KEY_LIST_LEN, DEF_KEY_LIST_LEN);
}

double conf_keytbl_timeout(conf_t* conf) {
    return getdouble(conf->dict, SEC_KEYTBL, KEY_KEY_TIMEOUT, DEF_KEY_TIMEOUT);
}

/*******************************************
 *
 * rountes to get config parameters (Heap)
 *
 ******************************************/

int conf_heap_size(conf_t* conf) {
    return getint(conf->dict, SEC_HEAP, KEY_HEAP_SIZE, DEF_HEAP_SIZE);
}

/*******************************************
 *
 * rountes to get config parameters (Associative Array)
 *
 ******************************************/

int conf_array_size(conf_t* conf) {
    return getint(conf->dict, SEC_ARRAY, KEY_ARRAY_SIZE, DEF_ARRAY_SIZE);
}

int conf_card_depth(conf_t* conf) {
    return getint(conf->dict, SEC_ARRAY, KEY_ARRAY_CARD_DEPTH, 
            DEF_ARRAY_CARD_DEPTH);
}

int conf_card_width(conf_t* conf) {
    return getint(conf->dict, SEC_ARRAY, KEY_ARRAY_CARD_WIDTH, DEF_ARRAY_CARD_WIDTH);
}

double conf_array_error_prob(conf_t* conf) {
    return getdouble(conf->dict, SEC_ARRAY, KEY_ARRAY_ERROR_PROB, DEF_ARRAY_ERROR_PROB);
}

/*******************************************
 *
 * rountes to get config parameters (Loss Count)
 *
 ******************************************/

int conf_losscount_size(conf_t* conf) {
    return getint(conf->dict, SEC_LC, KEY_LC_SIZE, DEF_LC_SIZE);
}
    
double conf_losscount_eps(conf_t* conf) {
    return getdouble(conf->dict, SEC_LC, KEY_LC_EPS, DEF_LC_EPS);
}

int conf_fm_num(conf_t* conf) {
    return getint(conf->dict, SEC_FM, KEY_FM_SIZE, DEF_FM_SIZE);
}

int conf_rev_width(conf_t* conf) {
    return getint(conf->dict, SEC_REV, KEY_REV_WIDTH, DEF_REV_WIDTH);
}

int conf_rev_depth(conf_t* conf) {
    return getint(conf->dict, SEC_REV, KEY_REV_DEPTH, DEF_REV_DEPTH);
}

int conf_rev_key_word_len(conf_t* conf) {
    return getint(conf->dict, SEC_REV, KEY_REV_KEY_WORD_LEN, DEF_REV_KEY_WORD_LEN);
}

int conf_rev_hash_word_len(conf_t* conf) {
    return getint(conf->dict, SEC_REV,
            KEY_REV_HASH_WORD_LEN, DEF_REV_HASH_WORD_LEN);
}

int conf_dos_width(conf_t* conf) {
    return getint(conf->dict, SEC_DOS, KEY_DOS_WIDTH, DEF_DOS_WIDTH);
}

int conf_dos_depth(conf_t* conf) {
    return getint(conf->dict, SEC_DOS, KEY_DOS_DEPTH, DEF_DOS_DEPTH);
}

int conf_dos_cm_width(conf_t* conf) {
    return getint(conf->dict, SEC_DOS, KEY_DOS_CM_WIDTH, DEF_DOS_CM_WIDTH);
}

int conf_dos_cm_depth(conf_t* conf) {
    return getint(conf->dict, SEC_DOS, KEY_DOS_CM_DEPTH, DEF_DOS_CM_DEPTH);
}

int conf_dos_rev_width(conf_t* conf) {
    return getint(conf->dict, SEC_DOS, KEY_DOS_REV_WIDTH, DEF_DOS_REV_WIDTH);
}

int conf_dos_rev_depth(conf_t* conf) {
    return getint(conf->dict, SEC_DOS, KEY_DOS_REV_DEPTH, DEF_DOS_REV_DEPTH);
}

int conf_dos_key_word_len(conf_t* conf) {
    return getint(conf->dict, SEC_DOS, KEY_DOS_KEY_WORD_LEN, DEF_DOS_KEY_WORD_LEN);
}

int conf_dos_hash_word_len(conf_t* conf) {
    return getint(conf->dict, SEC_DOS,
            KEY_DOS_HASH_WORD_LEN, DEF_DOS_HASH_WORD_LEN);
}

int conf_dos_thresh(conf_t* conf) {
    return getint(conf->dict, SEC_DOS,
            KEY_DOS_THRESH, DEF_DOS_THRESH);
}

int conf_super_width(conf_t* conf) {
    return getint(conf->dict, SEC_SUPER, KEY_SUPER_WIDTH, DEF_SUPER_WIDTH);
}

int conf_super_depth(conf_t* conf) {
    return getint(conf->dict, SEC_SUPER, KEY_SUPER_DEPTH, DEF_SUPER_DEPTH);
}

int conf_super_cm_width(conf_t* conf) {
    return getint(conf->dict, SEC_SUPER, KEY_SUPER_CM_WIDTH, DEF_SUPER_CM_WIDTH);
}

int conf_super_cm_depth(conf_t* conf) {
    return getint(conf->dict, SEC_SUPER, KEY_SUPER_CM_DEPTH, DEF_SUPER_CM_DEPTH);
}

int conf_super_rev_width(conf_t* conf) {
    return getint(conf->dict, SEC_SUPER, KEY_SUPER_REV_WIDTH, DEF_SUPER_REV_WIDTH);
}

int conf_super_rev_depth(conf_t* conf) {
    return getint(conf->dict, SEC_SUPER, KEY_SUPER_REV_DEPTH, DEF_SUPER_REV_DEPTH);
}

int conf_super_key_word_len(conf_t* conf) {
    return getint(conf->dict, SEC_SUPER, KEY_SUPER_KEY_WORD_LEN, DEF_SUPER_KEY_WORD_LEN);
}

int conf_super_hash_word_len(conf_t* conf) {
    return getint(conf->dict, SEC_SUPER,
            KEY_SUPER_HASH_WORD_LEN, DEF_SUPER_HASH_WORD_LEN);
}

int conf_super_thresh(conf_t* conf) {
    return getint(conf->dict, SEC_SUPER,
            KEY_SUPER_THRESH, DEF_SUPER_THRESH);
}

int conf_asm_n(conf_t* conf) {
    return getint(conf->dict, SEC_ASM,
            KEY_ASM_N, DEF_ASM_N);
}

int conf_asm_k(conf_t* conf) {
    return getint(conf->dict, SEC_ASM,
            KEY_ASM_K, DEF_ASM_K);
}

int conf_asm_num_state(conf_t* conf) {
    return getint(conf->dict, SEC_ASM,
            KEY_ASM_NUM_STATE, DEF_ASM_NUM_STATE);
}

int conf_mrac_depth(conf_t* conf) {
    return getint(conf->dict, SEC_MRAC, KEY_MRAC_DEPTH, DEF_MRAC_DEPTH);
}

int conf_mrac_r(conf_t* conf) {
    return getint(conf->dict, SEC_MRAC, KEY_MRAC_R, DEF_MRAC_R);
}

int conf_mrac_m(conf_t* conf) {
    return getint(conf->dict, SEC_MRAC, KEY_MRAC_M, DEF_MRAC_M);
}

int conf_trumpet_size(conf_t* conf) {
    return getint(conf->dict, SEC_TRUMPET, KEY_TRUMPET_SIZE, DEF_TRUMPET_SIZE);
}

// recovery

int conf_recovery_max_iter(conf_t* conf) {
    return getint(conf->dict, SEC_RECOVERY, KEY_RECOVER_MAX_ITER, DEF_RECOVER_MAX_ITER);
}
