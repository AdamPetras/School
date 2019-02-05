#include "DnsStructure.h"

using namespace std;

DnsStructure::DnsStructure(u_char* name,string type, const char* answer, int count)
{
	this->name = string(reinterpret_cast<char*>(name));
	this->type = type;
	this->answer = string(answer);
	this->count = count;
};

void DnsStructure::Print()
{
	cout<<name<<" "<<type<<" "<<answer<<" "<<count<<endl;
};
string DnsStructure::SendToSyslog(string deviceIp)
{
	static int i = 1;
	string s = "<"+to_string(getpid())+">"+to_string(i++)+" "+time+
     +" "+deviceIp+" dns-export --- "+name+" "+type+" "+answer+" "+to_string(count);
     return s;
};
