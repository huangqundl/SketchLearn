#ifndef __KEY_TBL_H__
#define __KEY_TBL_H__

/*
 * sim_aware.h
 * - header files for AWARE
 */

#include "packet.h"

#define RAND_NO 0.6180339887498948482

// hash node
typedef struct key_tbl_entry_s * key_tbl_entry_p_t;
typedef struct key_tbl_entry_s {
    tuple_t tuple;
	double old_ts;				// second-to-last updated timestamp
	double new_ts;				// latest updated timestamp
    int drop_index;
    long long low, high;
} key_tbl_entry_t;

//typedef struct key_tbl_entry_s {
//	//unsigned char key[13];
//    tuple_t tuple;
//	double old_ts;				// second-to-last updated timestamp
//	double new_ts;				// latest updated timestamp
//	//long long value;
//} key_tbl_entry_t;

// hash table
typedef struct key_tbl_s {
	key_tbl_entry_p_t *list;		// queue list for buckets
	int *list_len;					// list length
	int n;							// # of bits in each key
	double entry_to;				// entry expiration timeout
	int tbl_list_size;				// total # of lists in the table
	int tbl_list_len;				// # of buckets in one list

    int num_key;
    unsigned long hash, scale, hardner;
} key_tbl_t;

/***** function prototypes *****/
key_tbl_t* key_tbl_init(int n, int tbl_list_size, int tbl_list_len, 
		double entry_to); 
void key_tbl_destroy(key_tbl_t* tbl);
int key_tbl_size(key_tbl_t* tbl);

void key_tbl_read_from_file(key_tbl_t* tbl, const char* filename);
void key_tbl_write(key_tbl_t* tbl, const char* outfile);
void key_tbl_print(key_tbl_t* tbl, const char* filename);
void key_tbl_reset(key_tbl_t* tbl);
void key_tbl_copy(key_tbl_t* dst, key_tbl_t* src);
tuple_t* key_tbl_get_all(key_tbl_t* tbl, int* tot);
int key_tbl_num_key(key_tbl_t* tbl);

key_tbl_entry_p_t key_tbl_record(key_tbl_t* tbl, tuple_t* p,
		double ts);
key_tbl_entry_p_t key_tbl_find(key_tbl_t* tbl, tuple_t* p);

long long key_tbl_read_file(char* filename, key_tbl_t* tbl);

void read_rb(const char* dir, const char* alg, int interval,
        key_tbl_t* tbl_rb, long long* rb_tot,
        long long* max_perflow_dec, long long* all_flows_dec, int* rb_card);

int read_rb_multi(const char* dir, const char* alg, int interval,
        key_tbl_t* tbl_rb, long long* rb_tot,
        long long* max_perflow_dec, long long* all_flows_dec, int* rb_card, int n_piece);

/***** function prototypes *****/
//key_tbl_t* key_tbl_init(int n, int tbl_list_size, int tbl_list_len, 
//		double entry_to); 
//void key_tbl_destroy(key_tbl_t* tbl);
//
//void key_tbl_read_from_file(key_tbl_t* tbl, const char* filename);
//void key_tbl_write(key_tbl_t* tbl, const char* outfile);
//void key_tbl_print_dist(key_tbl_t* tbl, const char* outfile);
//
//key_tbl_entry_p_t key_tbl_record(key_tbl_t* tbl, unsigned char* key,
//		double ts);
//key_tbl_entry_p_t key_tbl_find(key_tbl_t* tbl, unsigned char* key);
//
//void key_tbl_reset(key_tbl_t* tbl);
//int key_tbl_get_all(key_tbl_t* tbl, key_tbl_entry_t* ret);
//

#endif
