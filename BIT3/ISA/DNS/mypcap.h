#pragma once
#include <pcap.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <iostream>
#include "DnsOperate.h"
#include <signal.h>
#include <vector>
#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE (256)
#endif
#define SIZE_ETHERNET (14)
class MyPcap
{
     const char *filter =
	"port 53 and ("
	"(udp and (not udp[10] & 128 = 0)) or"
	"(tcp and (not tcp[((tcp[12] & 0xf0) >> 2) + 2] & 128 = 0))"")";
     protected:
          struct bpf_program fp;
          bpf_u_int32 mask;
          bpf_u_int32 net;
          int link_type;
          int timer;
          bool isOnline;
          std::string deviceIP;
          std::string syslogIP;
          char errbuf[PCAP_ERRBUF_SIZE];
          int SetFilter(pcap_t *);
     public:
          std::vector<DnsStructure*> *statistics;
          DnsOperate operate;
          MyPcap();
          pcap_t * OpenOffline(const char*);
          pcap_t * OpenOnline(const char*);
          void SetTimer(int);
          int GetTimer();
          void SetSyslog(std::string);
          std::string GetSyslog();
          void SetDeviceIp(std::string);
          std::string GetDeviceIp();
          int Begin(pcap_t *);
};