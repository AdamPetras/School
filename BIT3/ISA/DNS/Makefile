all: dns

dns: dns.o mypcap.o DnsOperate.o DnsStructure.o MySyslog.o
	g++ -std=c++11 -g -o dns-export dns.o mypcap.o DnsOperate.o DnsStructure.o MySyslog.o -lpcap
mypcap.o: mypcap.cpp mypcap.h
	g++ -std=c++11 -c -g mypcap.cpp -lpcap
DnsOperate.o: DnsOperate.cpp DnsOperate.h
	g++ -std=c++11 -c -g DnsOperate.cpp -lpcap
DnsStructure.o: DnsStructure.cpp DnsStructure.h
	g++ -std=c++11 -c -g DnsStructure.cpp -lpcap
MySyslog.o: MySyslog.cpp MySyslog.h
	g++ -std=c++11 -c -g MySyslog.cpp -lpcap
clean:
	-rm dns.o mypcap.o DnsStructure.o DnsOperate.o dns-export
dns.o: dns.cpp dns.h
	g++ -std=c++11 -c -g dns.cpp -lpcap
zip:
	-zip xpetra19.zip *.cpp *.h Makefile manual.pdf dns-export.1