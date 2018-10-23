/********************************************************************
Qun Huang 2017,2018

Last modified: 2018-10-10

*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>
#include <math.h>
#include "hash.h"
#include "alg_sketchlearn.h"
#include "alg_keytbl.h"

#include "util.h"

/******************************************************************/

#define min(x,y)	((x) < (y) ? (x) : (y))
#define max(x,y)	((x) > (y) ? (x) : (y))

double gaussian_prob(double d) {
    return 1 - erf(d/sqrt(2));
}

const double min_conf = 0.95;
const double allow_bad_ratio = 0.1;

typedef struct BitInfo {
    int pos;
    int guess;
    double prob;
} bit_info_t;

int cmp_bit_info (const void * a, const void * b) {
    bit_info_t* t1 = (bit_info_t*)a;
    bit_info_t* t2 = (bit_info_t*)b;
    
    return t1->prob > t2->prob;
}

int32_t bit_est[104];

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

int get_bit(unsigned char* a, int pos) {
    int byte = pos / 8;
    int bit = pos % 8;
    if ((a[byte] & (1<<(7-bit))) == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

void loginsert(int32_t *lists, unsigned char* key, int length, int32_t diff) {
    lists[0] += diff;
    for (int i=length/8-1; i>=0; i--) {
        unsigned char val = key[i];
        for (int j=7; j>=0; j--) {
            if ((val&1) == 1) {
                lists[length] += diff;
            }
            length--;
            val >>= 1;
        }
    }
}

int32_t values[] = {1, 2, 3, 4, 5, 6, 7, 8};

// void loginsert_simd(int32_t  lists[], unsigned char* key, int length, int32_t diff) {
//     // internal routine used in update
//     // lists is a list of 'length' counts
//     // val is the item being added
//     // diff is the amount (positive or negative)
//     //    that its count changes by
// 
//     // update the logn different tests for a particular item  
//     lists[0] += diff;
// 
//     __m256i value_256;
//     __mmask8 mask_8;
//     __m256i mask_256 = _mm256_set_epi32(1<<31, 1<<31, 1<<31, 1<<31, 1<<31, 1<<31, 1<<31, 1<<31);
//     __m256i diff_256;
// 
//     mask_8 = key[0];
//     // __m256i diff_256 = _mm256_maskz_set1_epi32(mask_8, diff);
//     diff_256 = _mm256_set1_epi32(diff);
//     value_256 = _mm256_maskload_epi32(lists+1, mask_256);
//     value_256 = _mm256_add_epi32(value_256, diff_256);
//     _mm256_maskstore_epi32(lists+1, mask_256, value_256);
// 
//     mask_8 = key[1];
//     diff_256 = _mm256_set1_epi32(diff);
//     value_256 = _mm256_maskload_epi32(lists+9, mask_256);
//     value_256 = _mm256_add_epi32(value_256, diff_256);
//     _mm256_maskstore_epi32(lists+9, mask_256, value_256);
// 
//     mask_8 = key[2];
//     diff_256 = _mm256_set1_epi32(diff);
//     value_256 = _mm256_maskload_epi32(lists+17, mask_256);
//     value_256 = _mm256_add_epi32(value_256, diff_256);
//     _mm256_maskstore_epi32(lists+17, mask_256, value_256);
// 
//     mask_8 = key[3];
//     diff_256 = _mm256_set1_epi32(diff);
//     value_256 = _mm256_maskload_epi32(lists+25, mask_256);
//     value_256 = _mm256_add_epi32(value_256, diff_256);
//     _mm256_maskstore_epi32(lists+25, mask_256, value_256);
// 
//     mask_8 = key[4];
//     diff_256 = _mm256_set1_epi32(diff);
//     value_256 = _mm256_maskload_epi32(lists+33, mask_256);
//     value_256 = _mm256_add_epi32(value_256, diff_256);
//     _mm256_maskstore_epi32(lists+33, mask_256, value_256);
// 
//     mask_8 = key[5];
//     diff_256 = _mm256_set1_epi32(diff);
//     value_256 = _mm256_maskload_epi32(lists+41, mask_256);
//     value_256 = _mm256_add_epi32(value_256, diff_256);
//     _mm256_maskstore_epi32(lists+41, mask_256, value_256);
// 
//     mask_8 = key[6];
//     diff_256 = _mm256_set1_epi32(diff);
//     value_256 = _mm256_maskload_epi32(lists+49, mask_256);
//     value_256 = _mm256_add_epi32(value_256, diff_256);
//     _mm256_maskstore_epi32(lists+49, mask_256, value_256);
// 
//     // mask_8 = key[7];
//     // diff_256 = _mm256_set1_epi32(diff);
//     // value_256 = _mm256_maskload_epi32(lists+57, mask_256);
//     // value_256 = _mm256_add_epi32(value_256, diff_256);
//     // _mm256_maskstore_epi32(lists+57, mask_256, value_256);
// 
//     // mask_8 = key[8];
//     // diff_256 = _mm256_set1_epi32(diff);
//     // value_256 = _mm256_maskload_epi32(lists+65, mask_256);
//     // value_256 = _mm256_add_epi32(value_256, diff_256);
//     // _mm256_maskstore_epi32(lists+65, mask_256, value_256);
// 
//     // mask_8 = key[9];
//     // diff_256 = _mm256_set1_epi32(diff);
//     // value_256 = _mm256_maskload_epi32(lists+73, mask_256);
//     // value_256 = _mm256_add_epi32(value_256, diff_256);
//     // _mm256_maskstore_epi32(lists+73, mask_256, value_256);
// 
//     // mask_8 = key[10];
//     // diff_256 = _mm256_set1_epi32(diff);
//     // value_256 = _mm256_maskload_epi32(lists+81, mask_256);
//     // value_256 = _mm256_add_epi32(value_256, diff_256);
//     // _mm256_maskstore_epi32(lists+81, mask_256, value_256);
// 
//     // mask_8 = key[11];
//     // diff_256 = _mm256_set1_epi32(diff);
//     // value_256 = _mm256_maskload_epi32(lists+89, mask_256);
//     // value_256 = _mm256_add_epi32(value_256, diff_256);
//     // _mm256_maskstore_epi32(lists+89, mask_256, value_256);
// 
//     // mask_8 = key[12];
//     // diff_256 = _mm256_set1_epi32(diff);
//     // value_256 = _mm256_maskload_epi32(lists+97, mask_256);
//     // value_256 = _mm256_add_epi32(value_256, diff_256);
//     // _mm256_maskstore_epi32(lists+97, mask_256, value_256);
// }

// query primitive 1:

double skl_find_key_bits(int32_t *count, int len, double ratio, double* bit_mean, double* bit_std, unsigned char* ret, bit_info_t* bit_infos, int* n_bits) {
    unsigned long j;
    double conf = 1;
  
    j=1;

    int32_t V = count[0];      
    unsigned char k = 0;
    for (int i=len; i>0; i--) {
        double p = 1.0*count[i]/V;
  
        int n_byte = (i-1) / 8;
        int n_bit = (i-1) % 8;
  
        if(count[i]>=(1-ratio)*V) {
            k+=j;
            bit_infos[*n_bits].pos = i-1;
            bit_infos[*n_bits].guess = 1;
            bit_infos[*n_bits].prob = 1;
            *n_bits += 1;
        }
        else if (count[i+len]>=(1-ratio)*V) {
            bit_infos[*n_bits].pos = i-1;
            bit_infos[*n_bits].guess = 0;
            bit_infos[*n_bits].prob = 1;
            *n_bits += 1;
        }
        else {
            double r1 = (p-ratio)/(1-ratio);
            double r2 = p/(1-ratio);
            double d1 = fabs(r1-bit_mean[i-1]) / bit_std[i-1];
            double d2 = fabs(r2-bit_mean[i-1]) / bit_std[i-1];
            double p1 = gaussian_prob(d1)*bit_mean[i-1];
            double p2 = gaussian_prob(d2)*(1-bit_mean[i-1]);
            // double t = 6;
            // if (p1 > t*p2) {
            if (p1 > p2) {
                k+=j;
                // LOG_MSG("\t\tguess 1: %d r1 %lf d1 %lf p1 %lf r2 %lf d2 %lf p2 %lf\n", k, r1, d1, p1, r2, d2, p2);
                conf *= p1/(p1+p2);
                bit_infos[*n_bits].pos = i-1;
                bit_infos[*n_bits].guess = 1;
                bit_infos[*n_bits].prob = p1/(p1+p2);
                // LOG_MSG("1: %lf\n", p1/(p1+p2));
                *n_bits += 1;
            }
            else if (p1 < p2) {
                // LOG_MSG("\t\tguess 0: %d r1 %lf d1 %lf p1 %lf r2 %lf d2 %lf p2 %lf\n", k, r1, d1, p1, r2, d2, p2);
                conf *= p2/(p1+p2);
                bit_infos[*n_bits].pos = i-1;
                bit_infos[*n_bits].guess = 0;
                bit_infos[*n_bits].prob = p2/(p1+p2);
                // LOG_MSG("0: %lf\n", p2/(p1+p2));
                *n_bits += 1;
            }
        }
  
        // build the binary representation of the item
        j=j<<1;
  
        if (n_bit == 0) {
            ret[n_byte] = k;
            k = 0;
            j = 1;
        }
    }
    return conf;
}



int32_t skl_est_bucket_size(int32_t *count, int len, unsigned char* key, double* bit_mean, double* bit_std, int* bits, int n_bit, bit_info_t* ret_bit_infos) {
 
    int32_t V = count[0];
    int32_t naive = V;
    for (int i=0; i<n_bit; i++) {
        int pos = bits[i];
        int bit = get_bit(key, pos);
        
        if(bit == 1) {
            if (count[pos+1] < naive) {
                naive = count[pos+1];
            }
        }
        else {
            if (count[pos+1+len] < naive) {
                naive = count[pos+1+len];
            }
        }
    }
    for (int i=0; i<n_bit; i++) {
        int pos = bits[i];
        int bit = get_bit(key, pos);
        
        if(bit == 1) {
            if (bit_mean[pos] < 1 && bit_mean[pos] > 0) {
                bit_est[i] = count[pos+1]-count[pos+1+len]/(1-bit_mean[pos])*bit_mean[pos];
                if (bit_est[i] < 0) {
                    bit_est[i] = count[pos+1];
                }
            }
            else {
                bit_est[i] = count[pos+1];
            }
        }
        else {
            if (bit_mean[pos] < 1 && bit_mean[pos] > 0) {
                bit_est[i] = count[pos+1+len]-count[pos+1]/bit_mean[pos]*(1-bit_mean[pos]);
                if (bit_est[i] < 0) {
                    bit_est[i] = count[pos+1+len];
                }
            }
            else {
                bit_est[i] = count[pos+1+len];
            }
        }
    }

    qsort(bit_est, n_bit, sizeof(int32_t), cmp_int32);

    int32_t ret = 0;
    if (n_bit / 2 == 1) {
        ret = bit_est[n_bit/2];
    }
    else {
        ret = (bit_est[n_bit/2-1]+bit_est[n_bit/2-1])/2;
    }
    if (ret > naive) {
        ret = naive;
    }

    for (int i=0; i<n_bit; i++) {
        int pos = bits[i];
        int bit = get_bit(key, pos);

        double ratio = 1.0*ret / V;

        if (ret < 0) {
            ret_bit_infos[i].pos = pos;
            ret_bit_infos[i].prob = 0;
        }
        else if(count[pos+1]>=(1-ratio)*V) {
            ret_bit_infos[i].pos = pos;
            ret_bit_infos[i].guess = 1;
            ret_bit_infos[i].prob = 1;
        }
        else if (count[pos+len+1]>=(1-ratio)*V) {
            ret_bit_infos[i].pos = pos;
            ret_bit_infos[i].guess = 0;
            ret_bit_infos[i].prob = 1;
        }
        else {
            double p = 1.0*count[pos+1]/V;
            double r1 = (p-ratio)/(1-ratio);
            double r2 = p/(1-ratio);
            double d1 = fabs(r1-bit_mean[pos]) / bit_std[pos];
            double d2 = fabs(r2-bit_mean[pos]) / bit_std[pos];
            double p1 = gaussian_prob(d1)*bit_mean[pos];
            double p2 = gaussian_prob(d2)*(1-bit_mean[pos]);
        
            if (bit == 1) {
                ret_bit_infos[i].pos = pos;
                ret_bit_infos[i].guess = 1;
                ret_bit_infos[i].prob = p1 / (p1+p2);
            }
            else {
                ret_bit_infos[i].pos = pos;
                ret_bit_infos[i].guess = 0;
                ret_bit_infos[i].prob = p2 / (p1+p2);
            }
        }
    }

    return ret;
}

void skl_est_bucket_prob(int32_t *count, int lgn, unsigned char* key, double* bit_mean, double* bit_std, int* bits, int n_bit, bit_info_t* ret_bit_prob) {
 
    int32_t V = count[0];
    int ret = 0;

    for (int i=0; i<n_bit; i++) {
        int pos = bits[i];
        int bit = get_bit(key, pos);

        double ratio = 1.0*ret / V;

        if (ret < 0) {
            ret_bit_prob[i].pos = pos;
            ret_bit_prob[i].prob = 0;
        }
        else if(count[pos+1]>=(1-ratio)*V) {
            ret_bit_prob[i].pos = pos;
            ret_bit_prob[i].guess = 1;
            ret_bit_prob[i].prob = 1;
        }
        else if (count[pos+lgn+1]>=(1-ratio)*V) {
            ret_bit_prob[i].pos = pos;
            ret_bit_prob[i].guess = 0;
            ret_bit_prob[i].prob = 1;
        }
        else {
            double p = 1.0*count[pos+1]/V;
            double r1 = (p-ratio)/(1-ratio);
            double r2 = p/(1-ratio);
            double d1 = fabs(r1-bit_mean[pos]) / bit_std[pos];
            double d2 = fabs(r2-bit_mean[pos]) / bit_std[pos];
            double p1 = gaussian_prob(d1)*bit_mean[pos];
            double p2 = gaussian_prob(d2)*(1-bit_mean[pos]);
        
            if (bit == 1) {
                ret_bit_prob[i].pos = pos;
                ret_bit_prob[i].guess = 1;
                ret_bit_prob[i].prob = p1 / (p1+p2);
            }
            else {
                ret_bit_prob[i].pos = pos;
                ret_bit_prob[i].guess = 0;
                ret_bit_prob[i].prob = p2 / (p1+p2);
            }
        }
    }
}


/******************************************************************/

SketchLearn_t * SKL_Init(int width, int depth, int lgn)
{
    // initialize the data structure for finding absolute changes
    // width = 1/eps = width of hash functions
    // depth = number of independent repetitions to avoid misses
    // lgn = number of bits in representation of item indexes

    int i;
    //prng_type * prng;
    SketchLearn_t * skl;
    
    //prng=prng_Init(3152131,2);
    // use the random number generator to choose the hash functions

    width = calc_next_prime(width);
    skl=(SketchLearn_t *) calloc(1,sizeof(SketchLearn_t));
    skl->depth=depth;
    skl->width=width;
    skl->size=width*depth;
    skl->lgn=lgn;
    // take the input parameters and put them into the struct

    skl->hash = (unsigned long long *) calloc(depth,sizeof(unsigned long long));
    skl->scale = (unsigned long long *) calloc(depth,sizeof(unsigned long long));
    skl->hardner = (unsigned long long *) calloc(depth,sizeof(unsigned long long));
    quitmemory(skl->hash);
    quitmemory(skl->scale);
    quitmemory(skl->hardner);

    char name[] = "deltoid";
    unsigned long seed = AwareHash((unsigned char*)name, strlen(name), 13091204281, 228204732751, 6620830889);
    for (int i=0; i<depth; i++) {
        skl->hash[i] = GenHashSeed(seed++);
    }
    for (int i=0; i<depth; i++) {
        skl->scale[i] = GenHashSeed(seed++);
    }
    for (int i=0; i<depth; i++) {
        skl->hardner[i] = GenHashSeed(seed++);
    }
      

    skl->counts=(int32_t **) calloc(skl->size,sizeof(int32_t *));
    quitmemory(skl->counts);
    // make space for the hash functions

    for (i=0;i<skl->size;i++)
      {
        skl->counts[i]=(int32_t *) calloc(1+lgn*2,sizeof(int32_t));
        quitmemory(skl->counts[i]);
      }
    // make space for the counters

    return(skl);
}

int SKL_Size(SketchLearn_t * skl)
{
  // output the size (in bytes) used by the data structure

  int admin = sizeof(SketchLearn_t);
  int hash = 3*sizeof(unsigned int)*skl->depth;
  int pointer = skl->size * sizeof(int32_t*);
  int count = skl->size * (1+skl->lgn) * sizeof(int32_t);

  return admin + hash + pointer + count;
}

void SKL_Destroy(SketchLearn_t * skl)
{
  // free up the space that was allocated for the data structure

  int i;

  for (i=0;i<skl->size;i++)
    free(skl->counts[i]);
  free(skl->counts);
  //free(skl->testb);
  //free(skl->testa);
  free(skl->hash);
  free(skl->scale);
  free(skl->hardner);
  free(skl);
}

void SKL_Update_Fast(SketchLearn_t * skl, unsigned char* key, 
		int32_t diff) {
    // routine to update the count of an item
    // newitem is the idenfitier of the item being updated
    // diff is the change, positive or negative
    // skl is the data structure

    unsigned long hash;

    // for each set of groups, find the group that the item belongs in, update it
    for (uint64_t i=0; i<skl->depth; i++) {
        hash = AwareHash(key, skl->lgn/8,
            skl->hash[i], skl->scale[i], skl->hardner[i]);
        hash=hash % skl->width; 

        // use the hash function to find the place where the item belongs
        // loginsert_simd(skl->counts[i*skl->width+hash], key, skl->lgn, diff);
        // call external routine to update the counts
    }
}

void SKL_Update(SketchLearn_t * skl, unsigned char* key, 
		int32_t diff) {
    // routine to update the count of an item
    // newitem is the idenfitier of the item being updated
    // diff is the change, positive or negative
    // skl is the data structure

    unsigned long hash;

    // for each set of groups, find the group that the item belongs in, update it
    for (uint64_t i=0; i<skl->depth; i++) {
        hash = AwareHash(key, skl->lgn/8,
            skl->hash[i], skl->scale[i], skl->hardner[i]);
        hash=hash % skl->width; 

        // use the hash function to find the place where the item belongs
        loginsert(skl->counts[i*skl->width+hash], key, skl->lgn, diff);
        // call external routine to update the counts
    }
}

void SKL_Update_Select(SketchLearn_t * skl, unsigned char* key, 
		int32_t diff, int select, unsigned long long h, unsigned long long s, unsigned long long harder) {

    unsigned long select_hash = AwareHash(key, skl->lgn/8, h, s, harder) % skl->depth;
    for (int k=0; k<select; k++) {
        int r = (select_hash + k) % skl->depth;
        unsigned long hash = AwareHash(key, skl->lgn/8,
            skl->hash[r], skl->scale[r], skl->hardner[r]) % skl->width; 
        loginsert(skl->counts[r*skl->width+hash], key, skl->lgn, diff);
    }
}

// void SKL_Update_single(SketchLearn_t * skl, unsigned char* key, 
// 		int32_t diff) {
//     // routine to update the count of an item
//     // newitem is the idenfitier of the item being updated
//     // diff is the change, positive or negative
//     // skl is the data structure
// 
//     uint64_t hash_all = AwareHash(key, skl->lgn/8,
//             skl->hash[0], skl->scale[0], skl->hardner[0]);
// 
//     // for each set of groups, find the group that the item belongs in, update it
//     for (uint64_t i=0; i<skl->depth; i++) {
//         // hash=hash % skl->width; 
// 
//         uint32_t hash = (hash_all & 0xffffffff) % skl->width;
//         hash_all = hash_all >> 32;
//         // use the hash function to find the place where the item belongs
//         loginsert(skl->counts[i*skl->width+hash], key, skl->lgn, diff);
//         // call external routine to update the counts
//     }
// }
//

void compute_bit_dists(SketchLearn_t* skl) {
    memset(skl->bit_mean, 0, skl->lgn*sizeof(double));
    memset(skl->bit_std, 0, skl->lgn*sizeof(double));
    // long long V = 0;
    for (uint64_t i=0; i<skl->depth; i++) {
        for (uint64_t j=0; j<skl->width; j++) {
            int index = i*skl->width+j;
            for (int k=1; k<skl->lgn+1; k++) {
                double v = 1.0*skl->counts[index][k] / skl->counts[index][0];
                skl->bit_mean[k-1] += v;
            }
        }
    }
    for (int k=0; k<skl->lgn; k++) {
        skl->bit_mean[k] = skl->bit_mean[k] / skl->depth / skl->width;
    }

    for (uint64_t i=0; i<skl->depth; i++) {
        for (uint64_t j=0; j<skl->width; j++) {
            int index = i*skl->width+j;
            for (int k=1; k<skl->lgn+1; k++) {
                double v = 1.0*skl->counts[index][k] / skl->counts[index][0];
                skl->bit_std[k-1] += (v-skl->bit_mean[k-1])
                    *(v-skl->bit_mean[k-1]);
            }
        }
    }
    for (int k=0; k<skl->lgn; k++) {
        skl->bit_std[k] = sqrt(skl->bit_std[k]/skl->depth/skl->width)*sqrt(skl->depth*skl->width/(skl->depth*skl->width-1));
    }
}

void SKL_CompleteZeroes(SketchLearn_t* skl) {
	for (int i=0; i<skl->size; ++i) {
        int32_t V = skl->counts[i][0];
		for (int k=1; k<=skl->lgn; ++k) {
            skl->counts[i][k+skl->lgn] = V - skl->counts[i][k];
		}
	}
    compute_bit_dists(skl);
}

void enumerate_keys_search(unsigned char* orig, int key_len, bit_info_t* bit_infos, int n_bit, unsigned char* ret, int* n_ret) {
    if (n_bit == 0) {
        memcpy(ret+(*n_ret)*key_len, orig, key_len);
        *n_ret += 1;
        return;
    }
    int pos = bit_infos[n_bit-1].pos;
    set_bit(orig, pos, 0);
    enumerate_keys_search(orig, key_len, bit_infos, n_bit-1, ret, n_ret);
    set_bit(orig, pos, 1);
    enumerate_keys_search(orig, key_len, bit_infos, n_bit-1, ret, n_ret);
}


double enumerate_keys(unsigned char* orig, int key_len, bit_info_t* bit_infos, int n_bit_info, unsigned char* ret, int* n_ret, int lgn) {

    double conf = 1;
    if (n_bit_info <= lgn*allow_bad_ratio) {
        enumerate_keys_search(orig, key_len, bit_infos, n_bit_info, ret, n_ret);
    }
    else {
        *n_ret = 0;
        return conf;
    }

    return conf;
}

unsigned char template_key[13];
bit_info_t bit_infos[104];

unsigned long SKL_Identify_Thresh(SketchLearn_t* skl, double ratio, 
        unsigned char* keys, int32_t* vals, double* confs, int max_ret) {
    // take output from the data structure
    // thresh is the threshold for being a deltoid
    // skl is the data structure that holds the changes
  
    int ret = 0;
  
    compute_bit_dists(skl);
    
    for (uint64_t i=0;i<skl->depth;i++) {
        for (uint64_t j=0; j<skl->width; j++) {

            // go over all the different tests and see if there is a 
            // deltoid within each test
            int n_bit = 0;
            // LOG_MSG("bucket %d %d index %d\n", i, j, index);
            skl_find_key_bits(skl->counts[i*skl->width+j], skl->lgn, ratio, skl->bit_mean, skl->bit_std, template_key, bit_infos, &n_bit);
            // filter_bit_infos(bit_infos, &n_bit, bit_array, n_bit_array);
            // normalize_template_key(template_key, bit_array, n_bit_array);

            qsort(bit_infos, n_bit, sizeof(bit_info_t), cmp_bit_info);
            int pos = n_bit-1;
            for (; pos>=0; pos--) {
                if (bit_infos[pos].prob < min_conf) {
                    break;
                }
            }

            unsigned char* guess_keys = (unsigned char*)calloc(1<<(pos+1), sizeof(tuple_t));
            int n_guess = 0;
            // double conf2 = enumerate_keys(template_key, skl->lgn/8, bit_infos, n_bit, guess_keys, &n_guess);
            enumerate_keys(template_key, skl->lgn/8, bit_infos, pos+1, guess_keys, &n_guess, skl->lgn);
            // LOG_MSG("\tguess: %d\n",n_guess);
            for (int t=0; t<n_guess; t++) {
                unsigned char* guess = guess_keys+t*skl->lgn/8;
                unsigned long hash = AwareHash(guess, skl->lgn/8,
                        skl->hash[i], skl->scale[i], skl->hardner[i]);
                hash = hash % (skl->width); 

                // check item does hash into that bucket... 
                if (hash==j) {
                    // supress duplicates in output
                    // may be a little slow for large lists
                    int is_new = 1;
                    for (int k=0; k<ret; k++) {
                        if (memcmp(guess, keys+k*skl->lgn/8, skl->lgn/8) == 0) {
                            is_new = 0;
                            break;
                        }
                    }


                    if (is_new) {
                        // LOG_MSG("is new %d\n", is_new);
                        if (ret == max_ret) {
                            fprintf(stderr, "Reach MAX ret %d\n", max_ret);
                            exit(-1);
                        }

                        // LOG_MSG("try %d\n", t);
                        int32_t ret_est = skl->counts[i*skl->width+j][0];
                        for (uint64_t k=0; k<skl->depth; k++) {
                            unsigned long hash = AwareHash(guess, skl->lgn/8,
                                 skl->hash[k], skl->scale[k], skl->hardner[k]);
                            hash=hash % skl->width; 
                            int bucket = k*skl->width+hash;
                            int32_t est = skl->counts[bucket][0];
 
                            int length = skl->lgn;
                            for (int i=skl->lgn/8-1; i>=0; i--) {
                                unsigned char val = guess[i];
                                for (int j=7; j>=0; j--) {
                                    int32_t v = est;
                                    if ((val&1) == 1) {
                                        v = skl->counts[bucket][length];
                                    }
                                    else {
                                        v = skl->counts[bucket][length+skl->lgn];
                                    }
                                    if (v < est) {
                                        est = v;
                                    }
                                    length--;
                                    val >>= 1;
                                }
                            }
                            // LOG_MSG("row %d hash %d est %d\n", k, hash, est);
                            if (est < ret_est) {
                                ret_est = est;
                            }
                        }

                        if (ret_est >= ratio*skl->counts[i*skl->width+j][0]) {
                            memcpy(keys+ret*skl->lgn/8, guess, skl->lgn/8);
                            vals[ret] = ret_est;
                            for (int t=0; t<skl->lgn; t++) {
                                confs[ret*skl->lgn+t] = 1;
                            }
                            for (int t=0; t<n_bit; t++) {
                                // LOG_MSG("%d %d\n", t, bit_infos[t].pos);
                                int v = get_bit(guess, bit_infos[t].pos);
                                if (v == bit_infos[t].guess) {
                                    confs[ret*skl->lgn+bit_infos[t].pos] = bit_infos[t].prob;
                                }
                                else {
                                    confs[ret*skl->lgn+bit_infos[t].pos] = 1-bit_infos[t].prob;
                                }
                            }
                            ret++;
                        }
                    }
                }
            }
            free(guess_keys);
        }
    }
  
    return ret;
}  

void SKL_Print(SketchLearn_t* skl, const char* filename) {
	FILE* fp;

	fp = fopen(filename, "w");
    fprintf(fp, "depth %lu width %lu\n", skl->depth, skl->width);
	for (int i=0; i<skl->size; ++i) {
        int r = i / skl->width;
        int w = i % skl->width;
        fprintf(fp, "%d %d total %d\n", r, w, skl->counts[i][0]);
		for (int j=1; j<1+skl->lgn; ++j) {
            fprintf(fp, "\tbit %d: %d, %d\n", j, skl->counts[i][j], skl->counts[i][j+skl->lgn]);
		}
	}
	fclose(fp);
}

void SKL_GetMatrix(SketchLearn_t* skl, double** D) {
    for (uint64_t i=0; i<skl->width; i++) {
        for (uint64_t j=0; j<skl->depth; j++) {
            int sk_index = j*skl->width+i;
            for (int k=0; k<1+skl->lgn*2; k++) {
                D[i][j*(1+skl->lgn)+k] = skl->counts[sk_index][k];
            }
        }
    }
}

void SKL_ReadFile(SketchLearn_t* skl, const char* filename) {
    int line = 0;
    int row, col;
    char tmp[100];
	FILE* fp;
	fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "cannot open file %s\n", filename);
    }
    while (1) {
        if (fgets(tmp, 100, fp)==NULL) {
            break;
        }
        line++;
        if (line == 1) {
            uint64_t depth, width;
            sscanf(tmp, "depth %lu width %lu", &depth, &width);
            if (depth != skl->depth || width != skl->width) {
                fprintf(stderr, "size unmatch: config %lu %lu, actual %lu %lu\n",
                        skl->width, skl->depth,
                        width, depth);
                exit(-1);
            }
        }
        else if (tmp[0]>='0' && tmp[0]<='9') {
            int32_t size;
            sscanf(tmp, "%d %d total %d\n", &row, &col, &size);
            skl->counts[row*skl->width+col][0] = size;
        }
        else {
            int index;
            int32_t size1, size2;
            sscanf(tmp, "\tbit %d: %d, %d\n", &index, &size1, &size2);
            if (skl->counts[row*skl->width+col][0] != size1+size2) {
                LOG_ERR("Read error at row %d col %d bit %d: tot %d 1-bit %d 0-bit %d\n", row, col, index, skl->counts[row*skl->width+col][0], size1, size2);
            }
            skl->counts[row*skl->width+col][index] = size1;
            skl->counts[row*skl->width+col][index+skl->lgn] = size2;
        }
    }
	fclose(fp);
}

void SKL_ReadFile_Onerow(SketchLearn_t* skl, const char* filename, int row) {
    int line = 0;
    int col;
    char tmp[100];
	FILE* fp;
	fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "cannot open file %s\n", filename);
    }
    while (1) {
        if (fgets(tmp, 100, fp)==NULL) {
            break;
        }
        line++;
        if (line == 1) {
            uint64_t depth, width;
            sscanf(tmp, "depth %lu width %lu", &depth, &width);
            if (width != skl->width) {
                fprintf(stderr, "size unmatch: config %lu %lu, actual %lu %lu\n",
                        skl->width, skl->depth,
                        width, depth);
                exit(-1);
            }
        }
        else if (tmp[0]>='0' && tmp[0]<='9') {
            int32_t size;
            int tmp_row;
            sscanf(tmp, "%d %d total %d\n", &tmp_row, &col, &size);
            skl->counts[row*skl->width+col][0] = size;
        }
        else {
            int index;
            int32_t size1, size2;
            sscanf(tmp, "\tbit %d: %d, %d\n", &index, &size1, &size2);
            if (skl->counts[row*skl->width+col][0] != size1+size2) {
                LOG_ERR("Read error at row %d col %d bit %d: tot %d 1-bit %d 0-bit %d\n", row, col, index, skl->counts[row*skl->width+col][0], size1, size2);
            }
            skl->counts[row*skl->width+col][index] = size1;
            skl->counts[row*skl->width+col][index+skl->lgn] = size2;
        }
    }
	fclose(fp);
}

void SKL_Reset(SketchLearn_t* skl) {
	for (int i=0; i<skl->size; ++i) {
        memset(skl->counts[i], 0, (1+skl->lgn*2)*sizeof(int32_t));
	}
}

int32_t SKL_Est_Size(SketchLearn_t* skl, unsigned char* key, int* bits, int n_bit) {
    int32_t ret = 0;

    for (uint64_t k=0; k<skl->depth; k++) {
        unsigned long hash = AwareHash(key, skl->lgn/8,
            skl->hash[k], skl->scale[k], skl->hardner[k]);
        hash=hash % skl->width; 
        int bucket = k*skl->width+hash;

        int32_t est = skl_est_bucket_size(skl->counts[bucket], skl->lgn, key, skl->bit_mean, skl->bit_std, bits, n_bit, bit_infos);

        if (k==0 || est<ret) {
            ret = est;
        }
    }

    return ret;
}

// int32_t SKL_Est_Size_Prob(SketchLearn_t* skl, unsigned char* key, int* bits, int n_bit, double* prob) {
//     int32_t ret = 0;
// 
//     if (n_bit == skl->lgn) {
//         for (uint64_t k=0; k<skl->depth; k++) {
//             unsigned long hash = AwareHash(key, skl->lgn/8,
//                 skl->hash[k], skl->scale[k], skl->hardner[k]);
//             hash=hash % skl->width; 
//             int bucket = k*skl->width+hash;
// 
//             int32_t est = skl_est_size_and_prob(skl->counts[bucket], skl->lgn, key, skl->bit_mean, skl->bit_std, bits, n_bit, bit_infos);
// 
//             if (k==0 || est<ret) {
//                 ret = est;
//             }
//             /*
//             for (int i=0; i<n_bit; i++) {
//                 int pos = bit_infos[i].pos;
//                 // if (k==0 || bit_infos[i].prob < prob[pos]) {
//                 prob[pos] = bit_infos[i].prob;
//                 //}
//             }
//             */
//         }
//     }
//     else {
//         for (uint64_t k=0; k<skl->depth; k++) {
//             int32_t est_row = 0;
//             for (uint64_t bucket=0; bucket<skl->width; bucket++) {
//                 // int32_t est = skl_est_size_and_prob(skl->counts[bucket], skl->lgn, key, bit_mean, bit_std, bits, n_bit, bit_infos);
//                 // qsort(bit_infos, n_bit, sizeof(bit_info_t), cmp_bit_info);
//             }
// 
//             if (k==0 || est_row<ret) {
//                 ret = est_row;
//             }
//         }
//     }
// 
//     return ret;
// }

int32_t SKL_Est_old(SketchLearn_t* skl, unsigned char* key, int* bits, int n_bit) {
    int32_t ret = 0;
    if (n_bit == skl->lgn) {
        for (uint64_t k=0; k<skl->depth; k++) {
            unsigned long hash = AwareHash(key, skl->lgn/8,
                skl->hash[k], skl->scale[k], skl->hardner[k]);
            hash=hash % skl->width; 
            int bucket = k*skl->width+hash;
            int32_t est = skl->counts[bucket][0];
            for (int i=0; i<n_bit; i++) {
                int pos = bits[i];
                int32_t v = est;
                if (get_bit(key, pos) == 1) {
                    v = skl->counts[bucket][pos+1];
                }
                else {
                    v = skl->counts[bucket][pos+1+skl->lgn];
                }
                if (v < est) {
                    est = v;
                }
            }

            if (k==0 || est<ret) {
                ret = est;
            }
        }
    }
    else {
        // double* prob = (double*)calloc(n_bit, sizeof(double));
        compute_bit_dists(skl);
        for (uint64_t k=0; k<skl->depth; k++) {
            int32_t est_row = 0;
            for (uint64_t bucket=0; bucket<skl->width; bucket++) {
                int32_t est_bucket = skl->counts[bucket][0];
                int n_low_prob = 0;
                int32_t V = skl->counts[bucket][0];      
                for (int i=0; i<n_bit; i++) {
                    int pos = bits[i];
                    int bit = get_bit(key, pos);
                    int32_t v = est_bucket;

                    if(bit == 1) {
                        v = skl->counts[bucket][pos+1];
                    }
                    else {
                        v = skl->counts[bucket][pos+1+skl->lgn];
                    }

                    if (v < est_bucket) {
                        est_bucket = v;
                    }
                }
                if (est_bucket <= 0) {
                    continue;
                }
                double ratio = 1.0*est_bucket / V;
                for (int i=0; i<n_bit; i++) {
                    int pos = bits[i];
                    double p = 1.0*skl->counts[bucket][pos+1]/V;
                    int bit = get_bit(key, pos);

                    if(skl->counts[bucket][pos+1]>=(1-ratio)*V) {
                    }
                    else if (skl->counts[bucket][pos+skl->lgn+1]>=(1-ratio)*V) {
                    }
                    else {
                        double r1 = (p-ratio)/(1-ratio);
                        double r2 = p/(1-ratio);
                        double d1 = fabs(r1-skl->bit_mean[i-1]) / skl->bit_std[i-1];
                        double d2 = fabs(r2-skl->bit_mean[i-1]) / skl->bit_std[i-1];
                        double p1 = gaussian_prob(d1)*skl->bit_mean[i-1];
                        double p2 = gaussian_prob(d2)*(1-skl->bit_mean[i-1]);

                        if (bit == 1) {
                            // LOG_MSG("\tpos %d bit %d prob %lf\n", pos+1, bit, p1 / (p1+p2));
                            if (p1 / (p1+p2) < min_conf) {
                                n_low_prob++;
                            }
                        }
                        else {
                            // LOG_MSG("\tpos %d bit %d prob %lf\n", pos+1, bit, p2 / (p1+p2));
                            if (p2 / (p1+p2) < min_conf) {
                                n_low_prob++;
                            }
                        }
                    }

                    if (n_low_prob > n_bit*allow_bad_ratio) {
                        break;
                    }
                }
                if (n_low_prob < n_bit*allow_bad_ratio) {
                    est_row += est_bucket;
                    // LOG_MSG("Add at bucket %lu %lu: low prob %d est bucket %d est row %d\n", k, bucket, n_low_prob, est_bucket, est_row);
                }
            }

            if (k==0 || est_row<ret) {
                ret = est_row;
            }
        }
    }

    return ret;
}

void SKL_Copy(SketchLearn_t* dst, SketchLearn_t* src) {
    if (dst->width != src->width || dst->depth != src->depth) {
        fprintf(stderr, "Copy instances have different size\n");
        exit(-1);
    }
	for (int i=0; i<src->size; ++i) {
        memcpy(dst->counts[i], src->counts[i], (1+src->lgn*2)*sizeof(int32_t));
	}
}

void SKL_Copy_OneRow(SketchLearn_t* dst, SketchLearn_t* src, uint64_t r) {
    if (dst->width != src->width || dst->depth != 1 || r>=src->depth) {
        fprintf(stderr, "Copy instances have different size:\n"
                "dst width %lu src width %lu\n"
                "dst depth %lu src depth %lu\n",
                dst->width, src->width,
                dst->depth, src->depth
                );
        exit(-1);
    }
	for (uint64_t c=0; c<src->width; ++c) {
        uint64_t index = r*src->width + c;
        memcpy(dst->counts[c], src->counts[index], (1+src->lgn*2)*sizeof(int32_t));
	}
    dst->hash[0] = src->hash[r];
    dst->scale[0] = src->scale[r];
    dst->hardner[0] = src->hardner[r];
}

void SKL_Merge(SketchLearn_t* dst, SketchLearn_t* src) {
    if (dst->width != src->width || dst->depth != src->depth) {
        fprintf(stderr, "Copy instances have different size\n");
        exit(-1);
    }
	for (int i=0; i<src->size; ++i) {
        for (int j=0; j<=src->lgn*2; j++) {
            dst->counts[i][j] += src->counts[i][j];
        }
	}
}

void SKL_Diff(SketchLearn_t* dst, SketchLearn_t* src) {
    if (dst->width != src->width || dst->depth != src->depth) {
        fprintf(stderr, "Copy instances have different size\n");
        exit(-1);
    }
	for (int i=0; i<src->size; ++i) {
        for (int j=0; j<(1+src->lgn*2); j++) {
            dst->counts[i][j] -= src->counts[i][j];
        }
	}
}

int SKL_SerialSize(SketchLearn_t* skl) {
    return sizeof(unsigned int)+
        (2*sizeof(int)+sizeof(int32_t))*skl->width*skl->depth*(1+skl->lgn*2);
}

void SKL_Serialize(SketchLearn_t* skl, unsigned char* buf, unsigned int* len) {
    unsigned char* ptr = buf + sizeof(unsigned int);

    unsigned int cnt = 0;
    for (uint64_t i=0; i<skl->depth*skl->width; i++) {
        for (int j=0; j<(1+skl->lgn*2); j++) {
            if (skl->counts[i][j] != 0) {
                *(int*)ptr = i;
                ptr += sizeof(int);
                *(int*)ptr = j;
                ptr += sizeof(int);
                *(int32_t*)ptr = skl->counts[i][j];
                ptr += sizeof(int32_t);
                cnt++; 
            }
        }
    } 

    *len = sizeof(unsigned int) + (2*sizeof(int)+sizeof(int32_t))*cnt;
    *(unsigned int*)buf = *len;
}

void SKL_Deserialize(SketchLearn_t* skl, unsigned char* buf, unsigned int* len) {
    unsigned char* ptr = buf;
    unsigned int tot_size = *(unsigned int*)ptr;
    ptr += sizeof(unsigned int);

    int cnt = 0;
    while (ptr < buf+tot_size) {
        int i = *(int*)ptr;
        ptr += sizeof(int);
        int j = *(int*)ptr;
        ptr += sizeof(int);
        int32_t v = *(int32_t*)ptr;
        ptr += sizeof(int32_t);

        skl->counts[i][j] = v;
        cnt++;
    }
    if (tot_size != sizeof(unsigned int) + (2*sizeof(int)+sizeof(int32_t))*cnt) {
        fprintf(stderr, "tot_size %u, actual %lu (cnt %d)\n",
            tot_size,
            (2*sizeof(int)+sizeof(int32_t))*cnt, cnt);
    }
    *len = tot_size;
}

key_tbl_t** bucket_tbl = NULL;
key_tbl_t** bucket_tbl_select = NULL;
void SKL_DistInit(int width, int depth, int key_len) {
    int bit = key_len;
    double timeout = -1;
    bucket_tbl = (key_tbl_t**)calloc(width*depth, sizeof(key_tbl_t*));
    for (int i=0; i<depth; i++) {
        for (int j=0; j<width; j++) {
            bucket_tbl[i*width+j] = key_tbl_init(bit, 1000, 20, timeout);
        }
    }
}

void SKL_DistInit_Select(int width, int depth, int key_len) {
    int bit = key_len;
    double timeout = -1;
    bucket_tbl_select = (key_tbl_t**)calloc(depth, sizeof(key_tbl_t*));
    for (int i=0; i<depth; i++) {
        bucket_tbl_select[i] = key_tbl_init(bit, 10000, 30, timeout);
    }
}

void SKL_DistCount(SketchLearn_t* skl, unsigned char* key, int32_t diff) {
    for (uint64_t i=0; i<skl->depth; i++) {
        unsigned long hash = AwareHash(key, skl->lgn/8,
                skl->hash[i], skl->scale[i], skl->hardner[i]);
        hash=hash % skl->width; 
        // LOG_MSG("i %lu width %d hash %lu index %lu\n", i, skl->width, hash, i*skl->width+hash);
        key_tbl_entry_p_t record = key_tbl_record(bucket_tbl[i*skl->width+hash], (tuple_t*)key, -1);
        record->tuple.size += diff;
        // if (record == NULL) {
        //     LOG_MSG("NULL record\n");
        // }
    }
}

void SKL_DistCount_Select(SketchLearn_t* skl, unsigned char* key, int32_t diff,
        int select,
        unsigned long long h, unsigned long long s, unsigned long long harder) {
    unsigned long select_hash = AwareHash(key, skl->lgn/8, h, s, harder) % skl->depth;
    for (int k=0; k<select; k++) {
        int r = (select_hash + k) % skl->depth;
        key_tbl_entry_p_t record = key_tbl_record(bucket_tbl_select[r], (tuple_t*)key, -1);
        record->tuple.size += diff;
    }
}

void SKL_DistPrint(SketchLearn_t* skl, const char* filename) {
    FILE* dist_output = fopen(filename, "w");
    for (uint64_t i=0; i<skl->depth; i++) {
        for (uint64_t j=0; j<skl->width; j++) {
            int index = i*skl->width + j;
            fprintf(dist_output, "%lu %lu keys %d total %d\n", i, j, bucket_tbl[index]->num_key, skl->counts[index][0]);
            int n;
            tuple_t* flows = key_tbl_get_all(bucket_tbl[index], &n);
            qsort (flows, n, sizeof(tuple_t), cmp);
            for (int k=0; k<n; k++) {
                print_tuple(dist_output, flows + k);
            }
            free(flows);
            key_tbl_reset(bucket_tbl[i*skl->width+j]);
        }
    }
    fclose(dist_output);
}

void SKL_DistPrint_Select(SketchLearn_t* skl, const char* filename) {
    FILE* dist_output = fopen(filename, "w");
    for (uint64_t i=0; i<skl->depth; i++) {
        fprintf(dist_output, "%lu keys %d\n", i, bucket_tbl_select[i]->num_key);
        int n;
        tuple_t* flows = key_tbl_get_all(bucket_tbl_select[i], &n);
        qsort (flows, n, sizeof(tuple_t), cmp);
        for (int k=0; k<n; k++) {
            print_tuple(dist_output, flows + k);
        }
        free(flows);
        key_tbl_reset(bucket_tbl_select[i]);
    }
    fclose(dist_output);
}

void SKL_DistDestroy(SketchLearn_t* skl) {
    LOG_MSG("width %lu, depth %lu\n", skl->width, skl->depth);
    for (uint64_t i=0; i<skl->depth; i++) {
        for (uint64_t j=0; j<skl->width; j++) {
            key_tbl_destroy(bucket_tbl[i*skl->width+j]);
        }
    }
    free(bucket_tbl);
}

void SKL_DistDestroy_Select(SketchLearn_t* skl) {
    LOG_MSG("width %lu, depth %lu\n", skl->width, skl->depth);
    for (uint64_t i=0; i<skl->depth; i++) {
        key_tbl_destroy(bucket_tbl_select[i]);
    }
    free(bucket_tbl_select);
}
