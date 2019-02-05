/**
 * Adam Petráš (xpetra19)
 * FIT VUT v Brně 2018
 * Zdroj: https://www.geeksforgeeks.org/socket-programming-cc/
**/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

bool myConnect(int* _socket, string *host,string *port)
{
    struct hostent *hostPtr;
    struct sockaddr_in _sin;
	if( (*_socket = socket(AF_INET, SOCK_STREAM, 0 ) ) < 0)
	{
        cerr << "Error: nastala chyba pri vytvareni socketu" << endl;
		return false;
	}
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(atoi((*port).c_str()));
	if( (hostPtr =  gethostbyname((*host).c_str()) ) == NULL)
	{
        cerr << "Error: nastala chyba pri funkci gethostbyname" << (*host).c_str() << endl;
		return false;
	}
	memcpy( &_sin.sin_addr, hostPtr->h_addr, hostPtr->h_length);
	if( connect(*_socket, (struct sockaddr *)&_sin, sizeof(_sin) ) < 0 )
	{
        cerr << "Error: spatne pripojeni" << endl;
		return false;
	}
return true;
}

void FindAndReplace(string& source, string const& find, string const& replace)
{
    for(string::size_type i = 0; (i = source.find(find, i)) != string::npos;)
    {
        source.replace(i, find.length(), replace);
        i += replace.length();
    }
}

int Client(string host, string port, string moznosti)
{
    int _socket;
    string task; //přijmací zpráva ze serveru
    static const int MAX = 4096;
    char buffer[MAX];
    int numberOfRecvFromServer = 0; //pokud nám recv něco vrátil tak proměnná pro počet
    string prnt;
    
    if(!myConnect(&_socket,&host,&port))
    {
        cerr << "Error: nepripojeno" << endl;
        return -1;
    }  
    //*******************ODESÍLÁNÍ ZPRÁV SERVERU***************
    if(send(_socket, moznosti.c_str(), strlen(moznosti.c_str()) + 1, 0) < 0 ) //odešlu socket na server
    {
        cerr << "Error: nastala vyjimka pri posilani dat" << endl;
        return(-1);
    }
    
    //********************PŘÍJEM ZPRÁV OD SERVERU***************
    task = "";
    while(true)
    {
        numberOfRecvFromServer = recv(_socket, buffer, sizeof(buffer), 0);
        if(numberOfRecvFromServer < 0)
        {
            cerr << "Error: nastala vyjimka pri prijimani dat" << endl;
            return(-1);
        }
        task.append(buffer,numberOfRecvFromServer);
        if(strstr(buffer,"[End xpetra19 myProtocol]"))
        {
            break;
        }
    }
    //******************VÝPIS PŘI ZÍSKÁNÍ INFORMACÍ ZE SERVERU*********
    istringstream parsovanie(task.c_str()); 
    while(getline(parsovanie, prnt))
    {
        if(strstr(prnt.c_str(),"[End xpetra19 myProtocol]"))
        {
            FindAndReplace(prnt,"[End xpetra19 myProtocol]","");
            break;
        }
        if(prnt.find("Error: ") == string::npos) //print pokud neni chyba
        {
            cout<< prnt+"\n";
        }
        else //jinak pošleme na cerr
        {
            cerr << prnt << endl;
            return -1;
        }
    }
    if (close(_socket) < 0) //uzavření socketu
    { 
        cerr << "Error: nepodarilo se uzavrit socket" << endl;
        return(-1);
    }
    
    return(0);
}


int main (int argc, char *argv[] )
{
    string host = "";     //-h
    string port = "";     //-p
    string regex = "";    //-l
    int hcounter = 0;
    int pcounter = 0;
    int ncounter = 0;
    int fcounter = 0;
    int lcounter = 0;
    string strSettings = "[xpetra19 myProtocol]\n";

    int args;
    while ((args = getopt(argc, argv, "h:p:n:f:l::")) != -1) //ziskam argumenty getoptem
    {
        switch (args)
        {
        case 'h':
            if(hcounter >= 1)
            {
                cerr << "Error: velky pocet prepinacu -h" << endl;
                exit(-1);
            }
            host = optarg;
            hcounter++;
            break;
        case 'p':
            if(pcounter >= 1)
            {
                cerr << "Error: velky pocet prepinacu -p" << endl;
                exit(-1);
            }
            port = optarg;
            pcounter++;
            break;
        case 'n':
            if(ncounter >= 1)
            {
                cerr << "Error: velky pocet prepinacu -n" << endl;
                exit(-1);
            }
            strSettings.append("n");
            regex.append(optarg);
            ncounter++;
            break;
        case 'f':
            if(fcounter >= 1)
            {
                cerr << "Error: velky pocet prepinacu -f" << endl;
                exit(-1);
            }
            strSettings.append("f");
            regex.append(optarg);
            fcounter++;
            break;
        case 'l':
            if(lcounter >= 1)
            {
                cerr << "Error: velky pocet prepinacu -l" << endl;
                exit(-1);
            }
            strSettings.append("l");
            if(argv[optind]!= NULL)
                regex.append(argv[optind]);
            lcounter++;
            break;      
        default:
            cerr << "Error: Spatne argumenty default" << endl;
            return(-1);
            break;
        }
    }

    if(host.compare("") == 0 || port.compare("") == 0) //pokud nejsou potřebné argumenty
    {
        cerr << "Error: Spatne argumenty nektery je prazdny" << endl;
        return(-1);
    }

    if((port.find_first_not_of("0123456789") == string::npos) != true) //pokud port neni číslo
    {
        cerr << "Error: Spatne argumenty" << endl;
        return(-1);
    }
    if(atoi(port.c_str()) > 65535 || atoi(port.c_str()) < 1024) //port neni v rozsahu
    {
        cerr << "Error: Port neni v rozsahu 1024 - 65535" << endl;
        return(-1);
    }
    strSettings.append("\n"+regex);
    int end = Client(host, port, strSettings) != 0 ? -1 :0;
    return (end);
}