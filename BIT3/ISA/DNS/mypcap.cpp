#include "mypcap.h"
#include <chrono>
using namespace std;
     MyPcap::MyPcap()
     {
          timer = 60;
          statistics = new vector<DnsStructure*>;
          operate = DnsOperate();
     };
     pcap_t * MyPcap::OpenOffline(const char* filename)
     {
          pcap_t * handle;
          if(strcmp(filename,"")== 0)   //pokud neni vloženo jmeno souboru
          {
               fprintf(stderr, "Filename was not set.\n");
               return nullptr;
          }
          if ((handle = pcap_open_offline(filename,errbuf)) == NULL)  //otevření spojení offline
          {
               err(1,"Can't open file %s for reading",filename);
               return nullptr;
          }
          link_type = pcap_datalink(handle); //získání typu linkové vrstvy
          if (link_type != DLT_LINUX_SLL && link_type != DLT_EN10MB && link_type != DLT_IPV4 && link_type != DLT_IPV6) 
          {
		     fprintf(stderr, "Unsupported link type: %d\n", link_type);
               return nullptr;
          }
          isOnline = false;
          if(!SetFilter(handle))   //nastavení filtrů
               return nullptr;
          return handle;
     };
     pcap_t * MyPcap::OpenOnline(const char *device)
     {
          pcap_t * handle;
          if(strcmp(device,"")== 0)     //pokud neni vložené zařízení
          {
               fprintf(stderr, "Device was not set.\n");
               return nullptr;
          }
          handle = pcap_open_live(device, 2048, 0, 1000, errbuf);     //otevření online
          if (handle == NULL) {
               fprintf(stderr, "Couldn't open device %s: %s\n", device, errbuf);
               return nullptr;
          }
          link_type = pcap_datalink(handle); //získání typu linkové vrstvy
          if (link_type != DLT_LINUX_SLL && link_type != DLT_EN10MB && link_type != DLT_IPV4 && link_type != DLT_IPV6) 
          {
		     fprintf(stderr, "Unsupported link type: %d\n", link_type);
               return nullptr;
          }
          if (pcap_lookupnet(device, &net, &mask, errbuf) == -1)
          {
               fprintf(stderr, "Can't get netmask for device %s\n", device);
               net = 0;
               mask = 0;
               return nullptr;
          }
          isOnline = true;
          if(!SetFilter(handle)) //nastavení filtrů
               return nullptr;
          return handle;
     };
     int MyPcap::SetFilter(pcap_t * handle)
     {
          if (pcap_compile(handle, &fp, filter, 0, mask) == -1)  //kompilace stringového filteru
          {
               fprintf(stderr, "Couldn't parse filter %s: %s\n", filter, pcap_geterr(handle));
               return 0;
          }
          if (pcap_setfilter(handle, &fp) == -1)  //nastavení filteru
          {
               fprintf(stderr, "Couldn't install filter %s: %s\n", filter, pcap_geterr(handle));
               return 0;
          }
          return 1;
     };
     void MyPcap::SetTimer(int seconds)
     {
          timer = seconds;
     };

     int MyPcap::GetTimer()
     {
          return timer;
     };
     void MyPcap::SetSyslog(std::string ip)
     {
          syslogIP = ip;
     };
     std::string MyPcap::GetSyslog()
     {
          return syslogIP;
     };
     void MyPcap::SetDeviceIp(std::string ip)
     {
          deviceIP = ip;
     };
          
     std::string MyPcap::GetDeviceIp()
     {
          return deviceIP;
     };
     int MyPcap::Begin(pcap_t * handle)
     {
          struct dispkt dpkt;
          struct pcap_pkthdr *header = NULL;
          const u_char *packet = NULL;
          bool added = false;
          vector<DnsStructure*> *packt;
          int i = pcap_next_ex(handle, &header, &packet);   //otevření packetu
          if(i < 0 && !isOnline)   //pokud je konec pcap souboru
               return 1;
          if (i < 0) 
          {
               pcap_perror(handle, (char*)"Error capturing packet: ");
               return 1;
          }
          else if (!i) 
               return 0;

          if (!header || !packet)
          {
               fprintf(stderr, "Dropping corrupted packet\n");
               return 1;
          }
          if (header->caplen > header->len)
          {
               fprintf(stderr, "Dropping corrupted packet\n");
               return 1;
          }
          packt = operate.Out_Dns_Packet(&dpkt,link_type,packet, header);  //parsování packetu
          timeval tp;
          gettimeofday(&tp, 0);
          	time_t now = time(0);
          tm *ltm = localtime(&now);
          std::string strtime = to_string(1900+ltm->tm_year)
          +"-"+to_string(1+ltm->tm_mon)+"-"+to_string(ltm->tm_mday)+"T"+to_string(1 + ltm->tm_hour)
          +":"+to_string(ltm->tm_min)+":"+to_string(ltm->tm_sec)+"."+to_string(tp.tv_usec/1000)+"Z";
          if(!statistics->empty() && packt != nullptr) //pokud je již packet ve vectoru tak se zvýší čas
               for (std::vector<DnsStructure*>::const_iterator i = statistics->begin(); i != statistics->end(); ++i)
               {
                    for(std::vector<DnsStructure*>::const_iterator j = packt->begin(); j != packt->end(); ++j)
                    {
                         if((*j)!= nullptr)
                         {
                              if((*i)->name == (*j)->name && (*i)->answer == (*j)->answer && (*i)->type==(*j)->type)
                              {
                                   (*i)->count++;
                                   (*i)->time = strtime;
                                   added = true;
                              }
                         }
                    }
               }
          if(!added && packt != nullptr)     //přídání do vectoru
          {
               for(std::vector<DnsStructure*>::const_iterator i = packt->begin(); i != packt->end(); ++i)
               {
                    if((*i)!= nullptr)
                    {
                         (*i)->time = strtime;
                         statistics->push_back((*i));
                    }  
               }            
          }
          return 0;
     };