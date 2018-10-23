typedef struct SketchLearn_type {
    uint64_t depth;
    uint64_t width;
    int lgn; 
    int size;
    unsigned long long *hash, *scale, *hardner;
    int32_t** counts;

    double bit_mean[104];
    double bit_std[104];
} SketchLearn_t;

extern SketchLearn_t * SKL_Init(int, int, int);
void SKL_Update(SketchLearn_t *, unsigned char*, int32_t); 
void SKL_Update_Fast(SketchLearn_t *, unsigned char*, int32_t); 
void SKL_Update_Select(SketchLearn_t * skl, unsigned char* key, 
		int32_t diff, int select, unsigned long long h, unsigned long long s, unsigned long long harder);
unsigned long SKL_Identify_Thresh(SketchLearn_t* skl, double ratio,
        unsigned char* keys, int32_t* vals, double* confs, int max_ret); 
extern void SKL_Destroy(SketchLearn_t *);
extern int SKL_Size(SketchLearn_t *);
extern int32_t SKL_Est_Size(SketchLearn_t *, unsigned char*, int* bits, int n_bit);
extern int32_t SKL_Est_Size_Prob(SketchLearn_t *, unsigned char*, int* bits, int n_bit, double* prob);

void SKL_CompleteZeroes(SketchLearn_t* skl);

void SKL_Print(SketchLearn_t *, const char* filename);
void SKL_Reset(SketchLearn_t *);
void SKL_ReadFile(SketchLearn_t* skl, const char* filename);
void SKL_ReadFile_Onerow(SketchLearn_t* skl, const char* filename, int row);

void SKL_Copy(SketchLearn_t* dst, SketchLearn_t* src);
void SKL_Copy_OneRow(SketchLearn_t* dst, SketchLearn_t* src, uint64_t r);
void SKL_Diff(SketchLearn_t* dst, SketchLearn_t* src);
void SKL_Merge(SketchLearn_t* dst, SketchLearn_t* src);

void SKL_GetMatrix(SketchLearn_t* skl, double** D);

int SKL_SerialSize(SketchLearn_t* skl);
void SKL_Serialize(SketchLearn_t* skl, unsigned char* buf, unsigned int* len);
void SKL_Deserialize(SketchLearn_t* skl, unsigned char* buf, unsigned int* len);

// Only for debug purpose
void SKL_DistInit(int width, int depth, int key_len);
void SKL_DistCount(SketchLearn_t *, unsigned char*, int32_t); 
void SKL_DistPrint(SketchLearn_t* skl, const char* filename);
void SKL_DistDestroy(SketchLearn_t* skl);

void SKL_DistInit_Select(int width, int depth, int key_len);
void SKL_DistCount_Select(SketchLearn_t* skl, unsigned char* key, int32_t diff,
        int select,
        unsigned long long h, unsigned long long s, unsigned long long harder);
void SKL_DistPrint_Select(SketchLearn_t* skl, const char* filename);
void SKL_DistDestroy_Select(SketchLearn_t* skl);

void normalize_template_key(unsigned char* key, int* bits, int n_bit);
