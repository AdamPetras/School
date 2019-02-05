#ifndef DHCP_H
#define DHCP_H

#define DHCP_SERVER_NAME_LEN 64  //velikost jména serveru
#define DHCP_FILE_NAME_LEN 128     //velikost jmena souboru
#define DHCP_MTU_MAX 590    //maximální velikost DHCP
#define DHCP_OPTIONS_LEN 312    //velikost optionů dphc packetu
#define DHCP_BROADCAST_FLAG 32768
#define DHCP_SERVER_PORT 67
#define DHCP_CLIENT_PORT 68

struct dhcp_packet_struct {
    u_int8_t  opcode;
    u_int8_t  hardwareType;
    u_int8_t  hardwareAdrLen;
    u_int8_t  numOfHops;
    u_int32_t transactionId;
    u_int16_t secSinceClientStartedLooking;
    u_int16_t flags;
    struct in_addr clientIP;
    struct in_addr secondIP;
    struct in_addr serverIP;
    struct in_addr dhcpIP;
    unsigned char clientHardwareAddr [16];
    char serverName [DHCP_SERVER_NAME_LEN];
    char fileName [DHCP_FILE_NAME_LEN];
    unsigned char options [DHCP_OPTIONS_LEN];
};

#endif