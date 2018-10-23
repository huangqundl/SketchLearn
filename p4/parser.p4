#include "headers.p4"
#include "intrinsic.p4" 

parser start {
    extract(ethernet);
    extract(ipv4);
    return ingress;
}


#define ETHERTYPE_IPV4 0x0800
#define ETHERTYPE_ARP_IPV4 0x0806
