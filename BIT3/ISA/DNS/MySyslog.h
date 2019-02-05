#pragma once
#include <string>

const unsigned int SYSLOG_PORT = 514;
int send_msg_to_syslog(std::string,std::string);
