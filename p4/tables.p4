#include "parser.p4"

header_type my_metadata_t {
    fields {
        hash_index: 16;
        count_val: 16;
    }
}

header my_metadata_t my_metadata;

register sketch_register0 {
    width : 16;
    instance_count : 256;
}

register sketch_register1 {
    width : 16;
    instance_count : 256;
}

register sketch_register2 {
    width : 16;
    instance_count : 256;
}

register sketch_register3 {
    width : 16;
    instance_count : 256;
}

register sketch_register4 {
    width : 16;
    instance_count : 256;
}

register sketch_register5 {
    width : 16;
    instance_count : 256;
}

register sketch_register6 {
    width : 16;
    instance_count : 256;
}

register sketch_register7 {
    width : 16;
    instance_count : 256;
}

register sketch_register8 {
    width : 16;
    instance_count : 256;
}

register sketch_register9 {
    width : 16;
    instance_count : 256;
}

register sketch_register10 {
    width : 16;
    instance_count : 256;
}

register sketch_register11 {
    width : 16;
    instance_count : 256;
}

register sketch_register12 {
    width : 16;
    instance_count : 256;
}

register sketch_register13 {
    width : 16;
    instance_count : 256;
}

register sketch_register14 {
    width : 16;
    instance_count : 256;
}

register sketch_register15 {
    width : 16;
    instance_count : 256;
}

register sketch_register16 {
    width : 16;
    instance_count : 256;
}

register sketch_register17 {
    width : 16;
    instance_count : 256;
}

register sketch_register18 {
    width : 16;
    instance_count : 256;
}

register sketch_register19 {
    width : 16;
    instance_count : 256;
}

register sketch_register20 {
    width : 16;
    instance_count : 256;
}

register sketch_register21 {
    width : 16;
    instance_count : 256;
}

register sketch_register22 {
    width : 16;
    instance_count : 256;
}

register sketch_register23 {
    width : 16;
    instance_count : 256;
}

register sketch_register24 {
    width : 16;
    instance_count : 256;
}

register sketch_register25 {
    width : 16;
    instance_count : 256;
}

register sketch_register26 {
    width : 16;
    instance_count : 256;
}

register sketch_register27 {
    width : 16;
    instance_count : 256;
}

register sketch_register28 {
    width : 16;
    instance_count : 256;
}

register sketch_register29 {
    width : 16;
    instance_count : 256;
}

register sketch_register30 {
    width : 16;
    instance_count : 256;
}

register sketch_register31 {
    width : 16;
    instance_count : 256;
}

register sketch_register32 {
    width : 16;
    instance_count : 256;
}

field_list hash_fields { // ip 3 tuple
    ipv4.srcAddr;
    ipv4.dstAddr;
    ipv4.protocol;
}

field_list_calculation sketch_hash { // hash function
    input { 
        hash_fields;
    }
    algorithm : csum8;
    output_width : 8;
}

// count min sketch hash
action action_get_hash_val() {
    modify_field_with_hash_based_offset(my_metadata.hash_index, 0, sketch_hash, 256);
}

table table_count_min_sketch_hash {
	actions {
        action_get_hash_val;
    }
}

// count min sketch increase count

action action_count_min_sketch_incr0 () {
    register_read(my_metadata.count_val, sketch_register0, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register0, my_metadata.hash_index, my_metadata.count_val);

        register_read(my_metadata.count_val, sketch_register1, my_metadata.hash_index);
    
        add_to_field(my_metadata.count_val, 1);
    
        register_write(sketch_register1, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr0 {
    actions {
        action_count_min_sketch_incr0;
    }
}

action action_count_min_sketch_incr1 () {
    register_read(my_metadata.count_val, sketch_register1, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register1, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr1 {
    actions {
        action_count_min_sketch_incr1;
    }
}

action action_count_min_sketch_incr2 () {
    register_read(my_metadata.count_val, sketch_register2, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register2, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr2 {
    actions {
        action_count_min_sketch_incr2;
    }
}

action action_count_min_sketch_incr3 () {
    register_read(my_metadata.count_val, sketch_register3, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register3, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr3 {
    actions {
        action_count_min_sketch_incr3;
    }
}

action action_count_min_sketch_incr4 () {
    register_read(my_metadata.count_val, sketch_register4, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register4, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr4 {
    actions {
        action_count_min_sketch_incr4;
    }
}

action action_count_min_sketch_incr5 () {
    register_read(my_metadata.count_val, sketch_register5, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register5, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr5 {
    actions {
        action_count_min_sketch_incr5;
    }
}

action action_count_min_sketch_incr6 () {
    register_read(my_metadata.count_val, sketch_register6, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register6, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr6 {
    actions {
        action_count_min_sketch_incr6;
    }
}

action action_count_min_sketch_incr7 () {
    register_read(my_metadata.count_val, sketch_register7, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register7, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr7 {
    actions {
        action_count_min_sketch_incr7;
    }
}

action action_count_min_sketch_incr8 () {
    register_read(my_metadata.count_val, sketch_register8, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register8, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr8 {
    actions {
        action_count_min_sketch_incr8;
    }
}


action action_count_min_sketch_incr9 () {
    register_read(my_metadata.count_val, sketch_register9, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register9, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr9 {
    actions {
        action_count_min_sketch_incr9;
    }
}


action action_count_min_sketch_incr10 () {
    register_read(my_metadata.count_val, sketch_register10, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register10, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr10 {
    actions {
        action_count_min_sketch_incr10;
    }
}


action action_count_min_sketch_incr11 () {
    register_read(my_metadata.count_val, sketch_register11, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register11, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr11 {
    actions {
        action_count_min_sketch_incr11;
    }
}


action action_count_min_sketch_incr12 () {
    register_read(my_metadata.count_val, sketch_register12, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register12, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr12 {
    actions {
        action_count_min_sketch_incr12;
    }
}


action action_count_min_sketch_incr13 () {
    register_read(my_metadata.count_val, sketch_register13, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register13, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr13 {
    actions {
        action_count_min_sketch_incr13;
    }
}

action action_count_min_sketch_incr14 () {
    register_read(my_metadata.count_val, sketch_register14, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register14, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr14 {
    actions {
        action_count_min_sketch_incr14;
    }
}

action action_count_min_sketch_incr15 () {
    register_read(my_metadata.count_val, sketch_register15, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register15, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr15 {
    actions {
        action_count_min_sketch_incr15;
    }
}

action action_count_min_sketch_incr16 () {
    register_read(my_metadata.count_val, sketch_register16, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register16, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr16 {
    actions {
        action_count_min_sketch_incr16;
    }
}

action action_count_min_sketch_incr17 () {
    register_read(my_metadata.count_val, sketch_register17, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register17, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr17 {
    actions {
        action_count_min_sketch_incr17;
    }
}

action action_count_min_sketch_incr18 () {
    register_read(my_metadata.count_val, sketch_register18, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register18, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr18 {
    actions {
        action_count_min_sketch_incr18;
    }
}

action action_count_min_sketch_incr19 () {
    register_read(my_metadata.count_val, sketch_register19, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register19, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr19 {
    actions {
        action_count_min_sketch_incr19;
    }
}

action action_count_min_sketch_incr20 () {
    register_read(my_metadata.count_val, sketch_register20, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register20, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr20 {
    actions {
        action_count_min_sketch_incr20;
    }
}

action action_count_min_sketch_incr21 () {
    register_read(my_metadata.count_val, sketch_register21, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register21, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr21 {
    actions {
        action_count_min_sketch_incr21;
    }
}

action action_count_min_sketch_incr22 () {
    register_read(my_metadata.count_val, sketch_register22, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register22, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr22 {
    actions {
        action_count_min_sketch_incr22;
    }
}

action action_count_min_sketch_incr23 () {
    register_read(my_metadata.count_val, sketch_register23, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register23, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr23 {
    actions {
        action_count_min_sketch_incr23;
    }
}

action action_count_min_sketch_incr24 () {
    register_read(my_metadata.count_val, sketch_register24, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register24, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr24 {
    actions {
        action_count_min_sketch_incr24;
    }
}

action action_count_min_sketch_incr25 () {
    register_read(my_metadata.count_val, sketch_register25, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register25, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr25 {
    actions {
        action_count_min_sketch_incr25;
    }
}

action action_count_min_sketch_incr26 () {
    register_read(my_metadata.count_val, sketch_register26, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register26, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr26 {
    actions {
        action_count_min_sketch_incr26;
    }
}

action action_count_min_sketch_incr27 () {
    register_read(my_metadata.count_val, sketch_register27, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register27, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr27 {
    actions {
        action_count_min_sketch_incr27;
    }
}

action action_count_min_sketch_incr28 () {
    register_read(my_metadata.count_val, sketch_register28, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register28, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr28 {
    actions {
        action_count_min_sketch_incr28;
    }
}

action action_count_min_sketch_incr29 () {
    register_read(my_metadata.count_val, sketch_register29, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register29, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr29 {
    actions {
        action_count_min_sketch_incr29;
    }
}

action action_count_min_sketch_incr30 () {
    register_read(my_metadata.count_val, sketch_register30, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register30, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr30 {
    actions {
        action_count_min_sketch_incr30;
    }
}

action action_count_min_sketch_incr31 () {
    register_read(my_metadata.count_val, sketch_register31, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register31, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr31 {
    actions {
        action_count_min_sketch_incr31;
    }
}

action action_count_min_sketch_incr32 () {
    register_read(my_metadata.count_val, sketch_register32, my_metadata.hash_index);
    
    add_to_field(my_metadata.count_val, 1);
    
    register_write(sketch_register32, my_metadata.hash_index, my_metadata.count_val);
}

table table_count_min_sketch_incr32 {
    actions {
        action_count_min_sketch_incr32;
    }
}

table table_count_min_sketch_incr_all {
    actions {
        action_count_min_sketch_incr0;
        // if (ipv4.srcAddr & 0x00000001 > 0) {
        //     action_count_min_sketch_incr1;
        // }
        // if (ipv4.srcAddr & 0x00000002 > 0) {
        //     action_count_min_sketch_incr2;
        // }
        // if (ipv4.srcAddr & 0x00000004 > 0) {
        //     action_count_min_sketch_incr3;
        // }
        // if (ipv4.srcAddr & 0x00000008 > 0) {
        //     action_count_min_sketch_incr4;
        // }
        // if (ipv4.srcAddr & 0x00000010 > 0) {
        //     action_count_min_sketch_incr5;
        // }
        // if (ipv4.srcAddr & 0x00000020 > 0) {
        //     action_count_min_sketch_incr6;
        // }
        // if (ipv4.srcAddr & 0x00000040 > 0) {
        //     action_count_min_sketch_incr7;
        // }
        // if (ipv4.srcAddr & 0x00000080 > 0) {
        //     action_count_min_sketch_incr8;
        // }
        // if (ipv4.srcAddr & 0x00000100 > 0) {
        //     action_count_min_sketch_incr9;
        // }
        // if (ipv4.srcAddr & 0x00000200 > 0) {
        //     action_count_min_sketch_incr10;
        // }
        // if (ipv4.srcAddr & 0x00000400 > 0) {
        //     action_count_min_sketch_incr11;
        // }
        // if (ipv4.srcAddr & 0x00000800 > 0) {
        //     action_count_min_sketch_incr12;
        // }
        // if (ipv4.srcAddr & 0x00001000 > 0) {
        //     action_count_min_sketch_incr13;
        // }
        // if (ipv4.srcAddr & 0x00002000 > 0) {
        //     action_count_min_sketch_incr14;
        // }
        // if (ipv4.srcAddr & 0x00004000 > 0) {
        //     action_count_min_sketch_incr15;
        // }
        // if (ipv4.srcAddr & 0x00008000 > 0) {
        //     action_count_min_sketch_incr16;
        // }
        // if (ipv4.srcAddr & 0x00010000 > 0) {
        //     action_count_min_sketch_incr17;
        // }
        // if (ipv4.srcAddr & 0x00020000 > 0) {
        //     action_count_min_sketch_incr18;
        // }
        // if (ipv4.srcAddr & 0x00040000 > 0) {
        //     action_count_min_sketch_incr19;
        // }
        // if (ipv4.srcAddr & 0x00080000 > 0) {
        //     action_count_min_sketch_incr20;
        // }
        // if (ipv4.srcAddr & 0x00100000 > 0) {
        //     action_count_min_sketch_incr21;
        // }
        // if (ipv4.srcAddr & 0x00200000 > 0) {
        //     action_count_min_sketch_incr22;
        // }
        // if (ipv4.srcAddr & 0x00400000 > 0) {
        //     action_count_min_sketch_incr23;
        // }
        // if (ipv4.srcAddr & 0x00800000 > 0) {
        //     action_count_min_sketch_incr24;
        // }
        // if (ipv4.srcAddr & 0x01000000 > 0) {
        //     action_count_min_sketch_incr25;
        // }
        // if (ipv4.srcAddr & 0x02000000 > 0) {
        //     action_count_min_sketch_incr26;
        // }
        // if (ipv4.srcAddr & 0x04000000 > 0) {
        //     action_count_min_sketch_incr27;
        // }
        // if (ipv4.srcAddr & 0x08000000 > 0) {
        //     action_count_min_sketch_incr28;
        // }
        // if (ipv4.srcAddr & 0x10000000 > 0) {
        //     action_count_min_sketch_incr29;
        // }
        // if (ipv4.srcAddr & 0x20000000 > 0) {
        //     action_count_min_sketch_incr30;
        // }
        // if (ipv4.srcAddr & 0x40000000 > 0) {
        //     action_count_min_sketch_incr31;
        // }
        // if (ipv4.srcAddr & 0x80000000 > 0) {
        //     action_count_min_sketch_incr32;
        // }
    }
}
