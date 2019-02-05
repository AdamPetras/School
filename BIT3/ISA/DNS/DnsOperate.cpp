
#include "DnsOperate.h"
#include <vector>
using namespace std;
#include <bitset>
#include <sstream>

string DnsOperate::TypeToString(int type)
{
	if(type == A)
		return "A";
	else if(type == NS)
		return "NS";
	else if(type == CNAME)
		return "CNAME";
	else if(type == SOA)
		return "SOA";
	else if(type == PTR)
		return "PTR";
	else if(type ==MX)
		return "MX";
	else if(type ==TXT)
		return "TXT";
	else if(type ==AAAA)
		return "AAAA";
	else if (type == SRV)
		return "SRV";
	else if (type == NAPTR)
		return "NAPTR";
	else if(type ==RRSIG)
		return "RRSIG";
	else if(type ==NSEC)
		return "NSEC";
	else if(type == DS)
		return "DS";
	else if(type ==DNSKEY)
		return "DNSKEY";
	else if(type ==NSEC3)
		return "NSEC3";
	else if(type ==SPF)
		return "SPF";
	else if(type == ANY)
	return "ANY";
	return "";
};

u_char *DnsOperate::SkipLabel(u_char *label)
{
	u_char *tmp;
	if (!label) 
	{
		return NULL;
	}
	if (*label & 0xc0)
	{
		return label + 2;
	}
	tmp = label;
	while (*label) 
	{
		tmp += *label + 1;
		label = tmp;
	}
	return label + 1;
};

u_char * DnsOperate::LabelToString(u_char **label, u_char *dest,
                               size_t dest_size,
                               const u_char *payload,
                               const u_char *end)
{
	u_char *tmp, *dst = dest;
	if (!label || !*label || !dest)
	{
		goto laberror;
	}
	*dest = '\0';
	while (*label < end && **label) 
	{
		if (**label & 0xc0) 	//pokud je ukazatel
		{
			tmp = (u_char *)payload;
			tmp += ntohs(*(uint16_t *)(*label)) & 0x3fff;	//posunu o offset
			return LabelToString(&tmp,dst,dest_size,payload,end);		//zavolam rekurzivně funkci s offsetem
		}
		else	//pokud neni ukazatel
		{
			if ((*label + **label) >= end)
			{
				goto laberror;
			}
			if (**label + dst >= dest + dest_size)
			{
				goto laberror;
			}
			memcpy(dst, *label + 1, **label);
			dst += **label;
			if (dst > dest + dest_size)
			{
				goto laberror;
			}
			*label += **label + 1;
			*dst = '.'; 
			dst++;
		}
	}
	*(--dst) = '\0';
	return  dest;
	laberror:
	if (dest) *dest = '\0';
		return dest;
};

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
//******ZDROJ: https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c?fbclid=IwAR1Mn_CNqmJhaUPgugLuYGck5uDv29I9cwW8jZGl0keKVOPe8kUo98lHE9U
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

/*
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
NAME            an owner name, i.e., the name of the node to which this
                resource record pertains.

TYPE            two octets containing one of the RR TYPE codes.

CLASS           two octets containing one of the RR CLASS codes.

TTL             a 32 bit signed integer that specifies the time interval
                that the resource record may be cached before the source
                of the information should again be consulted.  Zero
                values are interpreted to mean that the RR can only be
                used for the transaction in progress, and should not be
                cached.  For example, SOA records are always distributed
                with a zero TTL to prohibit caching.  Zero values can
                also be used for extremely volatile data.

RDLENGTH        an unsigned 16 bit integer that specifies the length in
                octets of the RDATA field.
RDATA           a variable length string of octets that describes the
                resource.  The format of this information varies
                according to the TYPE and CLASS of the resource record.
*/

vector<DnsStructure*>* DnsOperate::Out_Dns_Packet(struct dispkt *dpkt,int lnk,const u_char *data,  struct pcap_pkthdr *hdr)
{
	struct dnshdr *dnsh;
	u_char *tmp;
	u_char *label;
	string dnsdata;
	const u_char *end;
	uint16_t len, qtype = 0;
	vector<DnsStructure*> *tmpvect = new vector<DnsStructure*>();
	int i;
	if(DissectPacket(dpkt,lnk,data,hdr))
	{
		return nullptr;
	}
	/*kontrola zda je platný packet jestli neni konec packetu menší než 
	payload(porovnání pointery)*/
	end = dpkt->payload + (hdr->len - dpkt->payload_offset);
	if (end < dpkt->payload)
		return nullptr;
	dnsh = (struct dnshdr *)(dpkt->payload);
	dnsh->id      = ntohs(dnsh->id);
	dnsh->flags   = ntohs(dnsh->flags);
	dnsh->qdcount = ntohs(dnsh->qdcount);
	dnsh->ancount = ntohs(dnsh->ancount);
	dnsh->nscount = ntohs(dnsh->nscount);
	dnsh->arcount = ntohs(dnsh->arcount);
	//pokud jsou deformované packety
	if (!dnsh->ancount || !dnsh->qdcount)
	{
		return nullptr;
	}
	tmp = (u_char *)(dpkt->payload + 12);
	for (i=0;i<dnsh->qdcount;i++) 
	{
		//získání první první otázky a jejího typu
		if (!qtype) {
			label =  reinterpret_cast<u_char*>(LabelToString(&tmp, buf, BUFSIZ, dpkt->payload, end));
			tmp++;
			qtype = ntohs(*(uint16_t *)tmp);
		} 
		else 
		{
			if (*tmp & 0xc0) 
				tmp += 2;
			else 
				tmp = SkipLabel(tmp);
		}
		//přeskočení typu a třídy
		tmp += 4;
		if (tmp >= end) 
			return nullptr;
	}
	//vyjmutí odpovědi
	if (!qtype) 
		return nullptr;
	DnsStructure* structure;
	//cout<<"#############################ANSWER SIZE = "<<dnsh->ancount<<" ###############################"<<endl;
	for (i=0;i<dnsh->ancount;i++) 
	{
		tmp = SkipLabel(tmp);
		qtype = ntohs(*(uint16_t *)tmp);
		//cout<<len;
		tmp += 8; // získání typu a přeskočení třídy a TTL
		if (tmp > end) 
			return nullptr;
		len = ntohs(*(uint16_t*)tmp);	// získání velikosti dat
		tmp +=2;
		size_t startptr = (size_t)tmp;
		//vyčtení dat které jsou u kazdého typu zprávy jinak uspořádány
		if(qtype == A)
		{
			/*
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ADDRESS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
			*/
			dnsdata = inet_ntop(AF_INET, tmp, dbuf, BUFSIZ);
		}
		else if (qtype == SOA||qtype == NS|| qtype == CNAME ||  qtype == PTR)
		{
			/*
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    /                     CNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    /                   NSDNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    /                   PTRDNAME                    /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    				
				   SOA ANSWER!
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    /                     MNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    /                     RNAME                     /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    SERIAL                     |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    REFRESH                    |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     RETRY                     |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    EXPIRE                     |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    MINIMUM                    |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
			*/

			dnsdata = (char *)LabelToString(
				&tmp, (u_char *)dbuf, BUFSIZ,
				dpkt->payload, tmp + len
			);
			dnsdata = dbuf;
		}
		else if(qtype == RRSIG)
		{
			/*
                        1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |        Type Covered           |  Algorithm    |     Labels    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                         Original TTL                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Signature Expiration                     |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Signature Inception                      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |            Key Tag            |                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+         Signer's Name         /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   /                                                               /
   /                            Signature                          /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			*/
			uint16_t typeCovered = ntohs((*(uint16_t *)tmp));
			tmp+=2;
			uint16_t algorithm = ntohs((*(uint8_t *)tmp)<<8);
			tmp++;
			uint16_t Labels = ntohs((*(uint8_t *)tmp)<<8);
			tmp++;
			uint32_t TTL = ntohl(*(uint32_t *)tmp);
			tmp+=4;
			uint32_t signatureExpiration = ntohl(*(uint32_t *)tmp);
			tmp+=4;
			uint32_t signatureInception = ntohl(*(uint32_t *)tmp);
			tmp+=4;
			uint16_t keyTag = ntohs(*(uint16_t *)tmp);
			tmp+=2;
			size_t savepointer = (size_t)tmp;
			string signerName = (char *)LabelToString(
				&tmp, (u_char *)(dbuf+i), BUFSIZ-i,
				dpkt->payload, tmp + len
			);
			size_t skip = (size_t)(tmp-startptr);
			char* buf = dbuf+i+skip-(2+1+1+4+4+4+2)-1;
			*(buf++)= ' ';
			dnsdata = to_string(typeCovered)+" "+to_string(algorithm)+" "+to_string(Labels)+
			" "+to_string(TTL)+" "+to_string(signatureExpiration)+
			" ( "+to_string(signatureInception)+" "+to_string(keyTag)+" "+signerName+". "+base64_encode((u_char *)(dbuf + skip + 3), len - 1 - skip)+" )";
		}
		else if(qtype == NSEC)
		{
			/*
				    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   /                      Next Domain Name                         /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   /                       Type Bit Maps                           /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			*/
			string domain = (char *)LabelToString(
				&tmp, (u_char *)dbuf, BUFSIZ,
				dpkt->payload, tmp + len
			);
			dnsdata = domain+". ( ";
			tmp++;
			int idx = ntohs(*(uint16_t *)tmp);
			tmp++;
			//parsování bitového pole obsaženého v NSEC záznamu
			for (int a = 0; a < idx; a++)
            	{
                tmp++;
                for (int b = 8; b >= 0; b--)
                {
                    if (*tmp & (1 << b))
                    {
					dnsdata+=TypeToString(a * 8 + (7 - b))+" ";
                    }
                }
            	}
			dnsdata+=+")";
			
		}
		else if(qtype == DS || qtype == DNSKEY) 
		{
			/*
				    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           Key Tag             |  Algorithm    |  Digest Type  |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   /                                                               /
   /                            Digest                             /
   /                                                               /
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			*/
			uint16_t keyTag = ntohs(*(uint16_t *)tmp);
			tmp+=2;
			uint16_t algorithm = ntohs((*(uint8_t *)tmp)<<8);
			tmp++;
			uint16_t diggestType = ntohs((*(uint8_t *)tmp)<<8);
			tmp++;
			string s = "0";
			size_t maxlen = (size_t)tmp+len;
			while((size_t)tmp < maxlen)
			{
				uint32_t first = ntohl(*(uint32_t *)tmp);
				ostringstream oss;
				oss<<hex<<first;
				
				s+= oss.str();
				tmp+=4;
			}
			dnsdata = to_string(keyTag)+" "+to_string(algorithm)+" "+to_string(diggestType)+" ( "+s+" )";
		}
		else if( qtype == NONE )
		{
			dnsdata = "NULL";
		}
		else if( qtype == MX)
		{
			/*
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                  PREFERENCE                   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    /                   EXCHANGE                    /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
			*/
			int j = snprintf(dbuf, 7, "%u ", ntohs(*(uint16_t *)tmp));
			tmp += 2;
			dnsdata = (char *)LabelToString(
				&tmp, (u_char *)(dbuf + j), BUFSIZ - j,
				dpkt->payload, tmp + len - 2
			);
			dnsdata = dbuf;
		}
		else if (qtype == TXT)
		{
			/*
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    /                   TXT-DATA                    /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
			*/
			if (*tmp <= len && tmp + len < end) {
				memcpy(dbuf, tmp+1, *tmp);
				dbuf[*tmp+1] = '\0';
			} else *dbuf = '\0';
			dnsdata = dbuf;
		}
		else if( qtype == AAAA)
		{
			/*
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
     /           IPv6 Address (16 octets)            /
     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
			*/
			dnsdata = inet_ntop(AF_INET6, tmp, dbuf, BUFSIZ);
		}
		else 
		{
			*dbuf = '\0';
			dnsdata = dbuf;
		}
		structure = new DnsStructure(label,TypeToString(qtype),dnsdata.c_str(),1);
		tmpvect->push_back(structure);
		//structure->Print();
		size_t stopPtr = (size_t)tmp;
		//cout<<"SKIP: "<<len <<" stop-start "<< (stopPtr-startptr)<< " SUMARUM: "<<len-(stopPtr-startptr)<<endl;
		tmp+=len-(stopPtr-startptr);
	}
	//cout<<"   len:"<<len <<endl;
	//cout<<"actPOS:"<<end-tmp<<endl;
	return tmpvect;
};


int DnsOperate::DissectPacket(struct dispkt *dpkt,int lnk,const u_char *data,  struct pcap_pkthdr *hdr)
{
	const u_char *pkt = data;
	uint16_t protocol = 0;
	/* získání protokolu z linkové vrstvy */
	if(lnk == DLT_EN10MB)
	{
		protocol = ntohs(*(uint16_t *)(pkt + ETH_HLEN - 2));
		pkt += ETH_HLEN;
	}
	else if(lnk == DLT_IPV4)
	{
		protocol = ETH_P_IP;
	}
	else if(lnk == DLT_IPV6)
	{
		protocol = ETH_P_IPV6;
	}

	if (check_packet_ptr(pkt, data, hdr->len) || !protocol)
		return 1;
	
	if(protocol == ETH_P_IP)
	{
		protocol = ((struct iphdr *)pkt)->protocol;
		dpkt->sa.ip  = ((struct iphdr *)pkt)->saddr;
		dpkt->da.ip  = ((struct iphdr *)pkt)->daddr;
		dpkt->family = AF_INET;
		pkt = pkt + (((struct iphdr *)pkt)->ihl << 2);
	}
	else if(protocol==ETH_P_IPV6)
	{
		dpkt->sa.ip6 = ((struct ip6_hdr *)pkt)->ip6_src;
		dpkt->da.ip6 = ((struct ip6_hdr *)pkt)->ip6_dst;
		dpkt->family = AF_INET6;
		protocol = ((struct ip6_hdr *)pkt)->ip6_nxt;
		pkt += 40; //ipv6 délka hlavičky
	}
	else
		protocol = 0;

	if (check_packet_ptr(pkt, data, hdr->len) || !protocol)
		return 1;

	//obsluha TCP/UDP vrstvy
	dpkt->ip_proto = protocol & 0xff;
	if(dpkt->ip_proto == IPPROTO_UDP)
	{
		dpkt->sp = ntohs(((struct udphdr *)pkt)->source);
		dpkt->dp = ntohs(((struct udphdr *)pkt)->dest);
		pkt += 8;
	}
	else if( dpkt->ip_proto == IPPROTO_TCP)
	{
		dpkt->sp = ntohs(((struct tcphdr *)pkt)->source);
		dpkt->dp = ntohs(((struct tcphdr *)pkt)->dest);
		pkt += (((struct tcphdr *)pkt)->doff << 2) + 1;
	}
	else protocol = 0;
	if (check_packet_ptr(pkt, data, hdr->len) || !protocol)
		return 1;
	dpkt->payload = pkt;
	dpkt->payload_offset = pkt - data;
	return 0;
};

