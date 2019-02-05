#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <features.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include "DnsStructure.h"
#include <ifaddrs.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <linux/if.h>
#include <vector>
#include <iostream>

struct dnshdr {
	uint16_t id;
	uint16_t flags;
	uint16_t qdcount;
	uint16_t ancount;
	uint16_t nscount;
	uint16_t arcount;
} __attribute__((packed));

struct dispkt {
	union { /**< Source address */
		struct in6_addr ip6;
		uint32_t        ip;
	} sa;

	union { /**< Destination address */
		struct in6_addr ip6;
		uint32_t        ip;
	} da;

	uint16_t sp;           /**< Source port */
	uint16_t dp;           /**< Destination port */
	int      family;       /**< Address family */
	uint8_t  ip_proto;     /**< IP protocol */
	size_t payload_offset; /**< Payload offset within the packet */
	const u_char *payload; /**< Packet payload */
};

#define check_packet_ptr(PKT, DATA, LEN) ((PKT) >= ((DATA) + (LEN)))

enum EDnsType
{
	NONE,
	A = 1,
	NS = 2,
	CNAME = 5,
	SOA = 6,
	PTR = 12,
	MX = 15,
	TXT = 16,
	AAAA = 28,
	SRV = 33,
	NAPTR = 35,
	DS = 43,
	RRSIG = 46,
	NSEC = 47,
	DNSKEY = 48,
	NSEC3 = 50,
	SPF = 99,
	ANY = 255
};

static u_char buf[BUFSIZ]; /* Label buffer */
static char dbuf[BUFSIZ];  /* Data bufffer */

class DnsOperate
{
	public:
		std::vector<DnsStructure*>* Out_Dns_Packet(struct dispkt*,int,const u_char*,  struct pcap_pkthdr*);
	private:
		EDnsType type;
		int DissectPacket(struct dispkt*,int,const u_char*,  struct pcap_pkthdr*);
		std::string TypeToString(int);
		u_char *SkipLabel(u_char*);
		u_char *LabelToString(u_char**,u_char*, size_t,const u_char*,const u_char*);
};


