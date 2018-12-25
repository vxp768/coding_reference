#ifndef _PKT_HDR_
#define _PKT_HDR_

#include <stdint.h>

#define IS_BIG_ENDIAN (!*(unsigned char *)&(uint16_t){1})

struct mac_header {
 uint8_t mac_dst[6];
 uint8_t mac_src[6];
}

uint16_t ether_type;

struct vlan_hdr {
#if IS_BIG_ENDIAN
 uint16_t tpid;        //tag protocol identifier
 uint16_t pcp:3;       //priority code point
 uint16_t dei:1;       //drop eligible indicator
 uint16_t vlanid:13;  //vlan id
#else
 uint16_t tpid;
 uint16_t vlanid:13;
 uint16_t dei:1;
 uint16_t pcp:3;
}

/*
struct foo {
  int b;
};   //normal use of struct

typedef struct {
  int e;
} bar; //anonymous struct. but can't use pointer to bar in this struct itself for link list
 
typedef struct {
 int e;
 struct bar *next;
} bar;  // NOT POSSIBLE since typedef doesn't happed till struct is defined

BUT we can forward declare like
 typedef struct bar_ bar

 struct bar {
  int e;
  bar *next;
 }; //POSSIBLE

 Forward delaration also helps in exposing opaque pointer. forward declare in 
 header file which is exposed to public so that pointer to struct can be passed and 
 received form library functions but user cannot access internal members. helps in data 
 encapsulation

typedef struct _baz {
  int b;
  struct _baz *next;
} baz; //can have pointer to struct _baz


 */

truct {

#if IS_BIG_ENDIAN
uint8_t iph_ver:4;  //big endian
uint8_t iph_ihl:4;
uint8_t iph_dscp:6;
uint8_t iph_ecn:2;
uint16_t iph_hdrlen;
uint16_t iph_id;
uint16_t iph_flags:3;
uint16_t iph_fragoffset:13;
uint8_t  iph_ttl;
uint8_t  iph_prot;
uint16_t chksum;
uint32_t iph_srcaddr;
uint32_t iph_dstaddr;
#else
uint8_t iph_ihl:4;
uint8_t iph_ver:4;
uint8_t iph_ecn:2;
uint8_t iph_dscp:6;
uint16_t iph_hdrlen;
uint16_t iph_id;
uint16_t iph_fragoffset:13;
uint16_t iph_flags:3;
uint8_t  iph_ttl;
uint8_t  iph_prot;
uint16_t chksum;
uint32_t iph_srcaddr;
uint32_t iph_dstaddr;
} ip_header;

typedef struct source_addr_ {
 uint32_t src_addr;
} source_addr;

typedef struct group_record_ {
 uint8_t rec_type;
 uint8_t aux_data_len;
 uint16_t num_srcs;
 uint32_t mc_addr;
 source_addr src[0];
} group_record;

typedef struct igmp_header_ {
 uint8_t type;
 uint8_t rsvd;
 uint16_t chksum;
 uint16_t rsvd;
 uint16_t num_grp_rec;
 group_record grp_rec[0];
} igmp_header;


#endif
                              
