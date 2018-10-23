/*
 * heap.h
 */

#ifndef __PARRAY__
#define __PARRAY__

//#include "packet.h"
#include "alg_keytbl.h"
#include "tuple.h"

#define NUM_ROW_FACTOR 7
//#define CARD_HASH_MAX 100000

typedef struct parray_entry_s {
    tuple_t tuple;
    unsigned long long per_flow_dec;
    unsigned long long sum;
    unsigned long long all_flows_dec;
    unsigned long long delta;

    int belong;      // index of list this entry belongs to

    struct parray_entry_s* prev;        // row entry list
    struct parray_entry_s* next;        
    struct parray_entry_s* gprev;        // global entry list
    struct parray_entry_s* gnext; 
} parray_entry_t;

typedef struct parray_s {
    int lgn;        // length of key in bits
    int max_size;
	int num_list;	    // total # of lists in the table
    unsigned long long hash, scale, hardner;
    double error_prob;

	parray_entry_t **hash_list;
    parray_entry_t* slot, *ghead_, *gtail_; // global list - all allocated nodes
    parray_entry_t* free_entry;            // ptr to the available free node

    int cur_num_entry;
    unsigned long num_dec;
    unsigned long sum;
    unsigned long per_flow_dec;
    unsigned long all_flows_dec;
    //unsigned long tot_inc_dec;
    parray_entry_t* max_entry, *min_entry;

    int card_width, card_depth;
    unsigned long long** card; // over 1,000,000
    unsigned int* card_hash, *card_scale, *card_hardner;

    // qhuang: just to evaluate
    key_tbl_t* miss_flows;
} parray_t;

/**********************************
 *
 * function prototypes of associative array
 *
 *********************************/

parray_t* parray_init(int lgn, int max_size, int card_depth, int card_width, double error_prob);
int parray_byte(parray_t* array);
void parray_destroy(parray_t* array);

// 0: non-exist, ret is the smallest one
// 1: exist, or non-exist but empty
parray_entry_t* parray_find(parray_t* array, tuple_t* t, int is_create);
unsigned long long parray_update(parray_t* array, tuple_t* t);
//int parray_delete(parray_t* array, tuple_t* key);
//void parray_dec(parray_t* array, unsigned long val, parray_entry_t* reserve);
//void parray_inc(parray_t* array, unsigned long val, parray_entry_t* entry);

void parray_print(parray_t* array, const char* filename);
void parray_reset(parray_t* array);
tuple_t* parray_get_all(parray_t* array, int* tot);
int parray_card(parray_t* array);

int parray_serial_size(parray_t* array);
void parray_serialize(parray_t* array, unsigned char* ret, unsigned int* len);
void parray_deserialize(key_tbl_t* tbl, unsigned char* data, unsigned int* len);

#endif
