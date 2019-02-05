all: dhcp

dhcp: dhcp.o
	g++ -std=c++17 -g -o ipk-dhcpstarve dhcp.o
clean:
	-rm dhcp.o ipk-dhcpstarve
dhcp.o: dhcp.cpp dhcp.h
	g++ -std=c++17 -c -g dhcp.cpp
