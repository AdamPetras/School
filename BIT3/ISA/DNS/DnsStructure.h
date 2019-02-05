#pragma once

#include <string>
#include <iostream>
#include <unistd.h>
class DnsStructure
{
     public:
          DnsStructure( u_char*,std::string, const char*, int);
          std::string name;
          std::string type;
          std::string answer;
          std::string time;
          int count;
          void Print();
          std::string SendToSyslog(std::string);
};