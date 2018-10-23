/*
 * key_table.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "packet.h"
#include "alg_keytbl.h"
#include "hash.h"

#define RAND_NO 0.6180339887498948482

/*
 * initialize the key table
 */
key_tbl_t* key_tbl_init(int n, int tbl_list_size, int tbl_list_len, double
		entry_to) {
	key_tbl_t* tbl;
	int i;

    tbl_list_size = calc_next_prime(tbl_list_size);

	tbl = (key_tbl_t*)calloc(1, sizeof(key_tbl_t));
	if (tbl == NULL) {
		fprintf(stderr, "ERR: cannot alloc tbl\n");
		exit(-1);
	}

	tbl->list = (key_tbl_entry_p_t*)calloc(tbl_list_size, 
			sizeof(key_tbl_entry_p_t));
	if (tbl->list == NULL) {
		fprintf(stderr, "ERR: cannot alloc list\n");
		exit(-1);
	}
	for (i=0; i<tbl_list_size; ++i) {
		tbl->list[i] = (key_tbl_entry_p_t)calloc(tbl_list_len,
				sizeof(key_tbl_entry_t));
		if (tbl->list[i] == NULL) {
			fprintf(stderr, "ERR: cannot alloc list[i]\n");
			exit(-1);
		}
	}
	tbl->list_len = (int*)calloc(tbl_list_size, sizeof(int));
	if (tbl->list_len == NULL) {
		fprintf(stderr, "ERR: cannot alloc list_len\n");
		exit(-1);
	}

	tbl->n = n;
	tbl->tbl_list_size = tbl_list_size;
	tbl->tbl_list_len = tbl_list_len;
	tbl->entry_to = entry_to;			// -1 means never expired

    char name[] = "keytbl";
    int seed = 0;
    for (uint64_t i=0; i<strlen(name); i++) {
        seed += name[i];
    }
    tbl->hash = GenHashSeed(seed+'h');
    tbl->scale = GenHashSeed(seed+'s');
    tbl->hardner = GenHashSeed(seed+'a');

	return tbl;
}


/*
 * add entries to key tbl from a file
 */
void key_tbl_read_from_file(key_tbl_t* tbl, const char* filename) {
    /*
	FILE* fp;
	int no_key;
	unsigned char key[13];
	long long value;
	key_tbl_entry_p_t key_tbl_entry;
	int i;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "ERR: cannot open %s\n", filename);
		exit(-1);
	}
	fread(&no_key, sizeof(unsigned int), 1, fp);
	for (i=0; i<no_key; ++i) {
		fread(key, tbl->n/8, 1, fp);		// read n/8 bytes
		fread(&value, sizeof(long long), 1, fp);
		key_tbl_entry = key_tbl_record(tbl, key, 0);
		key_tbl_entry->value = value;
	}
	fclose(fp);
    */
}


/*
 * record a key/value pair
 * - if no node exists, create a new node / find a replaced node, return 0
 */
key_tbl_entry_p_t key_tbl_record(key_tbl_t* tbl, tuple_t* p, 
		double ts) {

	//unsigned int a = 0, b = 0;	
	unsigned long bucket_no;					// tbl node position
	int rep_ind = -1;						// to-be-replaced entry index
	int cur_ind = -1;						// current entry index
	key_tbl_entry_p_t ret_entry = NULL;		// returned entry
	int i;

	// calculate the bucket no (for the keys that have 4 or 8 bytes)
    bucket_no = AwareHash((unsigned char*)&(p->key), tbl->n/8, tbl->hash, tbl->scale, tbl->hardner) % tbl->tbl_list_size;
    // LOG_MSG("bucket no %lu, bucket len %d\n", bucket_no, tbl->list_len[bucket_no]);

	// find if the node already exists, or find an expired node to be
	// replaced
	for (i=0; i<tbl->list_len[bucket_no]; ++i) {
		if (memcmp(&(tbl->list[bucket_no][i].tuple.key), &(p->key), tbl->n/8) == 0) {
			cur_ind = i;
			break;
		} else {
			if (tbl->entry_to != -1 && rep_ind == -1 && 
					ts - tbl->list[bucket_no][i].new_ts > tbl->entry_to) {
				rep_ind = i;
			}
		}
	}
		
	// set returned entry
	if (cur_ind == -1) {		// node does not exist
		if (rep_ind != -1) {				// replace an old one
			// reset the timestamp field
			tbl->list[bucket_no][rep_ind].new_ts = 0;
			// set return entry
			ret_entry = &(tbl->list[bucket_no][rep_ind]);
            LOG_MSG("should not here\n");
		} else {							// add a new one
			// check if the list is exhausted
			if (tbl->list_len[bucket_no] >= tbl->tbl_list_len) {
				fprintf(stderr, "ERR: tbl list exhausted\n");
                for (int k=0; k<tbl->tbl_list_size; k++) {
                    LOG_MSG("\t%d %d\n", k, tbl->list_len[k]);
                }
				exit(-1);
			}
            tbl->num_key++;
			// set returned entry
			ret_entry = &(tbl->list[bucket_no][tbl->list_len[bucket_no]]);
			// increase the list size
			tbl->list_len[bucket_no]++;
		}
	} else {						// entry exists
		// set returned entry
		ret_entry = &(tbl->list[bucket_no][cur_ind]);
	}

	// update common fields
	memcpy(&(ret_entry->tuple.key), &(p->key), tbl->n/8);
	ret_entry->old_ts = ret_entry->new_ts;  // 0 if ret_entry is new/replaced
	ret_entry->new_ts = ts;
	if (ret_entry->old_ts == 0) {
		ret_entry->tuple.size = 0;
	}

	// return the entry
	return ret_entry;
}


/*
 * Find the entry that has the key. Return the entry, or NULL if not found
 */
key_tbl_entry_p_t key_tbl_find(key_tbl_t* tbl, tuple_t* p) {

	//unsigned int a = 0, b = 0;	
	unsigned int bucket_no;					// tbl node position
	key_tbl_entry_p_t ret_entry = NULL;		// returned entry
	int i;

	// calculate the bucket no
    bucket_no = AwareHash((unsigned char*)&(p->key), tbl->n/8, tbl->hash, tbl->scale, tbl->hardner) % tbl->tbl_list_size;

	// find if the node already exists, or return NULL 
	for (i=0; i<tbl->list_len[bucket_no]; ++i) {
		if (memcmp(&(tbl->list[bucket_no][i].tuple.key), &(p->key), tbl->n/8) == 0) {
			ret_entry = &(tbl->list[bucket_no][i]);
			break;
		}
	}

	// return
	return ret_entry;
}


/*
 * write to a file
 */
void key_tbl_write(key_tbl_t* tbl, const char* outfile) {
	FILE* fp;
	int tot_no_key;
	int i, j;

	// open a file
	if ((fp = fopen(outfile, "wb")) == NULL) {
		fprintf(stderr, "ERR: cannot open %s\n", outfile);
		exit(-1);
	}

	// find the total number of key
	tot_no_key = 0;
	for (i=0; i<tbl->tbl_list_size; ++i) {
		tot_no_key += tbl->list_len[i];
	}	

	// write to a file
	fwrite(&tot_no_key, sizeof(int), 1, fp);
	for (i=0; i<tbl->tbl_list_size; ++i) {
		for (j=0; j<tbl->list_len[i]; ++j) {
			fwrite(&(tbl->list[i][j].tuple.key), tbl->n/8, 1, fp);
			fwrite(&(tbl->list[i][j].tuple.size), sizeof(long long), 1, fp);
		}
	}

	// close a file
	fclose(fp);
}


/*
 * print all values in the key tbl
 */
void key_tbl_print(key_tbl_t* tbl, const char* filename) {

    int n = 0;
    tuple_t* ret = key_tbl_get_all(tbl, &n);
    qsort (ret, n, sizeof(tuple_t), cmp);
    FILE* output = fopen(filename, "w");
    for (int i=0; i<n; i++) {
        print_tuple(output, ret + i);
    }
    fclose(output);
    free(ret);
}


/*
 * print all values in the key tbl
 */
tuple_t* key_tbl_get_all(key_tbl_t* tbl, int* tot) {

    *tot = 0;
	for (int i=0; i<tbl->tbl_list_size; ++i) {
        *tot += tbl->list_len[i];
    }

    tuple_t* ret = (tuple_t*)calloc(*tot, sizeof(tuple_t));

    int index = 0;
	for (int i=0; i<tbl->tbl_list_size; ++i) {
		for (int j=0; j<tbl->list_len[i]; ++j) {
            memcpy(ret+index, tbl->list[i]+j, sizeof(tuple_t));
            index++;
		}
	}

    return ret;
}

int key_tbl_num_key(key_tbl_t* tbl) {
    int ret = 0;
	for (int i=0; i<tbl->tbl_list_size; ++i) {
        ret += tbl->list_len[i];
	}
    return ret;
}

/*
 * reset the key table to zero values
 */
void key_tbl_reset(key_tbl_t* tbl) {
	int i;

    tbl->num_key = 0;
	for (i=0; i<tbl->tbl_list_size; ++i) {
		memset(tbl->list[i], 0, tbl->tbl_list_len * sizeof(key_tbl_entry_t));
		tbl->list_len[i] = 0;
	}
}

int key_tbl_size(key_tbl_t* tbl) {
    int admin = sizeof(key_tbl_t);
    return admin;
}

/*
 * destroy the key table
 */
void key_tbl_destroy(key_tbl_t* tbl) {
	int i;

	for (i=0; i<tbl->tbl_list_size; ++i) {
		free(tbl->list[i]);
	}
	free(tbl->list);
	free(tbl->list_len);
	free(tbl);
}

void key_tbl_copy(key_tbl_t* dst, key_tbl_t* src) {
    int n = 0;
    tuple_t* ret = key_tbl_get_all(src, &n);
    for (int i=0; i<n; i++) {
        key_tbl_entry_p_t record = key_tbl_record(dst, ret+i, -1);
        record->tuple.size += ret[i].size;
    }
    free(ret);
}


long long key_tbl_read_file(char* filename, key_tbl_t* tbl) {
    long long tot = 0;

    char tmp[100];
    tuple_t p;
    FILE* input = fopen(filename, "r");
    if (input == NULL) {
        fprintf(stderr, "cannot open file %s\n", filename);
    }
    while (1) {
        if (fgets(tmp, 100, input)==NULL) {
            break;
        }

        read_tuple(tmp, &p);

        key_tbl_entry_p_t record = key_tbl_record(tbl, &p, -1);
        record->tuple.size += p.size;
        tot += p.size;
    }

    fclose(input);

    return tot;
}

void read_opt_tuple(char* line, key_tbl_entry_t* p) {
    unsigned int ip1, ip2, ip3, ip4, ip5, ip6, ip7, ip8;
    sscanf(line, "%u.%u.%u.%u(%hu) <-> %u.%u.%u.%u(%hu) %hhu low %lld max %lld",
        &ip1, &ip2, &ip3, &ip4,
        &p->tuple.key.src_port,
        &ip5, &ip6, &ip7, &ip8,
        &p->tuple.key.dst_port, 
        &p->tuple.key.proto,
        &p->low,
        &p->high);
    p->tuple.key.src_ip = (ip4 << 24) | (ip3 << 16) | (ip2 <<8) | ip1;
    p->tuple.key.dst_ip = (ip8 << 24) | (ip7 << 16) | (ip6 <<8) | ip5;
}

void read_rb(const char* dir, const char* alg, int interval,
        key_tbl_t* tbl_rb, long long* rb_tot,
        long long* max_perflow_dec, long long* all_flows_dec, int* rb_card) {

    char tmp[100];
    sprintf(tmp, "%srb_%s_%d_opt", dir, alg, interval);
    FILE* fp = fopen(tmp, "r");
    int line = 0;
    long long ret = 0;
    key_tbl_entry_t entry;
    while (1) {
        if (fgets(tmp, 100, fp)==NULL) {
            break;
        }
        line++;
        if (line == 1) {
            char* p = strtok(tmp, " ");
            p = strtok(NULL, " ");
            ret = atoll(p);
        }
        else {
            read_opt_tuple(tmp, &entry);
            key_tbl_entry_p_t record = key_tbl_record(tbl_rb, &entry.tuple, -1);
            record->tuple.size += entry.low;
            record->low += entry.low;
            record->high += entry.high;
        }
    }
    fclose(fp);
    *rb_tot = ret;

    sprintf(tmp, "%sstat_%s_%d", dir, alg, interval);
    fp = fopen(tmp, "r");
    while (1) {
        if (fgets(tmp, 100, fp)==NULL) {
            break;
        }
        char* str = strstr(tmp, "appro keys");
        if (str != NULL) {
            char* tok = strtok(str, " ");
            tok = strtok(NULL, " ");
            tok = strtok(NULL, " ");
            *rb_card = atoi(tok);
            break;
        }
    }
    fclose(fp);
}


int read_rb_multi(const char* dir, const char* alg, int interval,
        key_tbl_t* tbl_rb, long long* rb_tot,
        long long* max_perflow_dec, long long* all_flows_dec, int* rb_card, int n_piece) {

    int num_dec = 0;
    for (int k=0; k<n_piece; k++) {
        char tmp[100];
        sprintf(tmp, "%srb_%s_%d_%d_opt", dir, alg, interval, k);
        FILE* fp = fopen(tmp, "r");
        int line = 0;
        long long tot_dec = 0;
        key_tbl_entry_t entry;
        while (1) {
            if (fgets(tmp, 100, fp)==NULL) {
                break;
            }
            line++;
            if (line == 1) {
                char* p = strtok(tmp, " ");
                p = strtok(NULL, " ");
                tot_dec = atoll(p);
                p = strtok(NULL, " ");
                p = strtok(NULL, " ");
                num_dec = atoi(p);
            }
            else {
                read_opt_tuple(tmp, &entry);
                key_tbl_entry_p_t record = key_tbl_record(tbl_rb, &entry.tuple, -1);
                record->tuple.size += entry.low;
                record->low += entry.low;
                record->high += entry.high;
            }
        }
        fclose(fp);
        *rb_tot += tot_dec;

        sprintf(tmp, "%sstat_%s_%d_%d", dir, alg, interval, k);
        fp = fopen(tmp, "r");
        while (1) {
            if (fgets(tmp, 100, fp)==NULL) {
                break;
            }
            char* str = strstr(tmp, "appro keys");
            if (str != NULL) {
                char* tok = strtok(str, " ");
                tok = strtok(NULL, " ");
                tok = strtok(NULL, " ");
                *rb_card += atoi(tok);
                break;
            }
        }
        fclose(fp);
    }

    return num_dec;
}
