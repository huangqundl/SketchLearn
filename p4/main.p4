#include "tables.p4"

//microsecond, 3s
#define timestamp_window_max 3000000

control ingress {
	
        if (ethernet.etherType == ETHERTYPE_IPV4) { // ipv4
		    
		    // init
			apply(table_count_min_sketch_hash);
			apply(table_count_min_sketch_incr_all);
		
			apply(table_count_min_sketch_incr0);
            
            if (ipv4.srcAddr & 0x00000001 > 0) {
			    apply(table_count_min_sketch_incr1);
            }
            if (ipv4.srcAddr & 0x00000002 > 0) {
			    apply(table_count_min_sketch_incr2);
            }
            if (ipv4.srcAddr & 0x00000004 > 0) {
			    apply(table_count_min_sketch_incr3);
            }
            if (ipv4.srcAddr & 0x00000008 > 0) {
			    apply(table_count_min_sketch_incr4);
            }
            if (ipv4.srcAddr & 0x00000010 > 0) {
			    apply(table_count_min_sketch_incr5);
            }
            if (ipv4.srcAddr & 0x00000020 > 0) {
			    apply(table_count_min_sketch_incr6);
            }
            if (ipv4.srcAddr & 0x00000040 > 0) {
			    apply(table_count_min_sketch_incr7);
            }
            if (ipv4.srcAddr & 0x00000080 > 0) {
			    apply(table_count_min_sketch_incr8);
            }
            if (ipv4.srcAddr & 0x00000100 > 0) {
			    apply(table_count_min_sketch_incr9);
            }
            if (ipv4.srcAddr & 0x00000200 > 0) {
			    apply(table_count_min_sketch_incr10);
            }
            if (ipv4.srcAddr & 0x00000400 > 0) {
			    apply(table_count_min_sketch_incr11);
            }
            if (ipv4.srcAddr & 0x00000800 > 0) {
			    apply(table_count_min_sketch_incr12);
            }
            if (ipv4.srcAddr & 0x00001000 > 0) {
			    apply(table_count_min_sketch_incr13);
            }
            if (ipv4.srcAddr & 0x00002000 > 0) {
			    apply(table_count_min_sketch_incr14);
            }
            if (ipv4.srcAddr & 0x00004000 > 0) {
			    apply(table_count_min_sketch_incr15);
            }
            if (ipv4.srcAddr & 0x00008000 > 0) {
			    apply(table_count_min_sketch_incr16);
            }
            if (ipv4.srcAddr & 0x00010000 > 0) {
			    apply(table_count_min_sketch_incr17);
            }
            if (ipv4.srcAddr & 0x00020000 > 0) {
			    apply(table_count_min_sketch_incr18);
            }
            if (ipv4.srcAddr & 0x00040000 > 0) {
			    apply(table_count_min_sketch_incr19);
            }
            if (ipv4.srcAddr & 0x00080000 > 0) {
			    apply(table_count_min_sketch_incr20);
            }
            if (ipv4.srcAddr & 0x00100000 > 0) {
			    apply(table_count_min_sketch_incr21);
            }
            if (ipv4.srcAddr & 0x00200000 > 0) {
			    apply(table_count_min_sketch_incr22);
            }
            if (ipv4.srcAddr & 0x00400000 > 0) {
			    apply(table_count_min_sketch_incr23);
            }
            if (ipv4.srcAddr & 0x00800000 > 0) {
			    apply(table_count_min_sketch_incr24);
            }
            if (ipv4.srcAddr & 0x01000000 > 0) {
			    apply(table_count_min_sketch_incr25);
            }
            if (ipv4.srcAddr & 0x02000000 > 0) {
			    apply(table_count_min_sketch_incr26);
            }
            if (ipv4.srcAddr & 0x04000000 > 0) {
			    apply(table_count_min_sketch_incr27);
            }
            if (ipv4.srcAddr & 0x08000000 > 0) {
			    apply(table_count_min_sketch_incr28);
            }
            if (ipv4.srcAddr & 0x10000000 > 0) {
			    apply(table_count_min_sketch_incr29);
            }
            if (ipv4.srcAddr & 0x20000000 > 0) {
			    apply(table_count_min_sketch_incr30);
            }
            if (ipv4.srcAddr & 0x40000000 > 0) {
			    apply(table_count_min_sketch_incr31);
            }
            if (ipv4.srcAddr & 0x80000000 > 0) {
			    apply(table_count_min_sketch_incr32);
            }
        }
}

control egress {
}
