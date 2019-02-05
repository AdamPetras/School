/*
 * main.cpp
 *
 *  Created on: 10. 10. 2018
 *      Author: adam
 */
#include "mypcap.h"
#include <getopt.h>
#include <unistd.h>
#include "MySyslog.h"
using namespace std;
static void debug(string s);

MyPcap *pcap;
pcap_t *handle = nullptr;

static void sig_handle(int signum)
{
     if (signum == SIGUSR1) //pokud je signal SIGUSR1
     {
          //vypíše všechny informace o DNS odpovědích
          for (std::vector<DnsStructure*>::const_iterator i = pcap->statistics->begin(); i != pcap->statistics->end(); ++i)
               (*i)->Print();
     }
     else if(signum == SIGALRM) //pokud je signal SIGALRM
     {
          //pošle na syslog všechy DNS odpovědi
          for (std::vector<DnsStructure*>::const_iterator i = pcap->statistics->begin(); i != pcap->statistics->end(); ++i)
          {
               //(*i)->Print();
                if(pcap->GetSyslog()!="")
                  send_msg_to_syslog((*i)->SendToSyslog(pcap->GetDeviceIp()),pcap->GetSyslog());
                else 
                  (*i)->Print();
               //cout<<(*i)->SendToSyslog(pcap->GetDeviceIp())<<endl;
          }
          alarm(pcap->GetTimer());
     }
}

int main(int argc, char *argv[])
{
     /*dns-export [-r file.pcap] [-i interface] [-s syslog-server] [-t seconds]
     -r : zpracuje daný pcap soubor
     -i : naslouchá na daném síťovém rozhraní a zpracovává DNS provoz
     -s : hostname/ipv4/ipv6 adresa syslog serveru
     -t : doba výpočtu statistik, výchozí hodnota 60s
     */
     pcap = new MyPcap();
     signal(SIGUSR1, sig_handle);
     int opt;
     string filename = "";
     string interface = "";
     //getopts parsování argumentů
     while ((opt = getopt(argc, argv, "r:i:s:t:")) != -1) {
          switch (opt) {
          case 'r':
               filename = optarg;
               break;
          case 'i':
               interface = optarg;
               break;
          case 's':
               pcap->SetSyslog(optarg);
               if(pcap->GetSyslog() == "")
               {
                  std::cerr << "Error with syslog server. Cant find IP." << std::endl;
                  exit(EXIT_FAILURE);
               }
               break;
          case 't':
               pcap->SetTimer(atoi(optarg));
               break;
          default:
            std::cerr << "Argument ERROR!" << std::endl;
               exit(EXIT_FAILURE);
        }
     }
     char Ip[128];
      gethostname(Ip, 128);
      //cout<<Ip<<endl;
      //nastavení hostname
     pcap->SetDeviceIp(Ip);
     //zapnutí alarmu
     alarm(pcap->GetTimer());
     signal(SIGALRM, sig_handle);
     //pokud je offline sniff
     if(filename != "")
     {
          handle = pcap->OpenOffline(filename.c_str());
     }
     else if(interface != "") //pokud je offline sniff
     {
          handle = pcap->OpenOnline(interface.c_str());
     }
     if(handle != nullptr)  
     {
          while (!pcap->Begin(handle)); // cyklus projde všechny packety pokud je online pořád cyklí
     }
     if(handle != nullptr)  //uzavření spojení
          pcap_close(handle);
      //pokud je offline tak výpis do syslog
      for (std::vector<DnsStructure*>::const_iterator i = pcap->statistics->begin(); i != pcap->statistics->end(); ++i)
     {
          if((*i)!= nullptr)
          {
               //(*i)->Print();
                if(pcap->GetSyslog()!="")
                  send_msg_to_syslog((*i)->SendToSyslog(pcap->GetDeviceIp()),pcap->GetSyslog());
                else 
                  (*i)->Print();
                  
               //cout<<(*i)->SendToSyslog(pcap->GetDeviceIp())<<endl;
          }
     }
}



