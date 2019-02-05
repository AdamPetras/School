#include "MySyslog.h"
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <strings.h>
#include <iostream> 
#include <unistd.h>
// ZDROJ https://github.com/wrabcak/ISA-projekt/blob/master/main.cpp
int send_msg_to_syslog(std::string msg, std::string syslogIp)
{
  int socketHandleSyslog=0;
  struct sockaddr_in servaddr;
  struct hostent *hPtr;
  socketHandleSyslog=socket(AF_INET,SOCK_DGRAM,0);
  // get IP adress of syslog server.
  if((hPtr = gethostbyname(syslogIp.c_str())) == NULL)
  {
      std::cerr << "Error with syslog server. Cant find IP." << std::endl;
      return 2;
  }
  //load data to socket structure.
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr=inet_addr(syslogIp.c_str());
  servaddr.sin_port=htons(SYSLOG_PORT);
  
  //send data.
  sendto(socketHandleSyslog,msg.c_str(),strlen(msg.c_str()),0, (struct sockaddr *)&servaddr,sizeof(servaddr));
  
  // close socket.
  close (socketHandleSyslog);
  return 0;
}