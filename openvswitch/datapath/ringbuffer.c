#include <linux/skbuff.h>

#include "ringbuffer.h"

ringbuffer_t* create_ringbuffer_kshm(unsigned char* ptr, unsigned long tuple_size) {

    ringbuffer_t* ret = (ringbuffer_t*)kzalloc(sizeof(struct RingBuffer), GFP_KERNEL);

    // setup tuple size
    ret->tuple_size = tuple_size;
    ret->meta = (struct RBMeta*)ptr;
    ret->appro = (struct RBAppro*)((unsigned char*)ptr + sizeof(struct RBMeta));
    ret->data = (unsigned char*)ptr +
        sizeof(struct RBMeta) + sizeof(struct RBAppro);
    memset(ret->meta, 0, sizeof(struct RBMeta));
    memset(ret->data, 0, RB_SIZE*tuple_size);
    
    return ret;
}

void destroy_ringbuffer_kshm(ringbuffer_t* rb) {
    kfree(rb);
}

void flush_ringbuffer(ringbuffer_t* rb) {
    rb->meta->writeIndex = rb->meta->nextWriteIndex;
	rb->meta->readIndex = rb->meta->nextReadIndex;
}

// 0: success; otherwise: fail
int write_ringbuffer(ringbuffer_t* rb, void* data, unsigned long size) {
	int afterNextWriteIndex = nextVal(rb->meta->nextWriteIndex);
	if (afterNextWriteIndex == rb->meta->localReadIndex) {
		if (afterNextWriteIndex == rb->meta->readIndex) {
            return -1;
		}
		rb->meta->localReadIndex = rb->meta->readIndex;
	}
    memcpy(rb->data+rb->tuple_size*rb->meta->nextWriteIndex, data, size);
	rb->meta->nextWriteIndex = afterNextWriteIndex;
	rb->meta->wBatchCount++;
	if (rb->meta->wBatchCount >= RB_BATCH) {
		rb->meta->writeIndex = rb->meta->nextWriteIndex;
		rb->meta->wBatchCount = 0;
	}
    return 0;
}
