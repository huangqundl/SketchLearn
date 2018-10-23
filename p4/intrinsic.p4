header_type intrinsic_metadata_t {
    fields {
        ingress_global_timestamp : 32;
        lf_field_list : 8;
        mcast_grp : 16;
        egress_rid : 16;
        resubmit_flag : 8;
        recirculate_flag : 8;
    }
}

metadata intrinsic_metadata_t intrinsic_metadata;
