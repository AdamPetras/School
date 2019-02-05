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
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex.h>
#include <locale.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <fstream>
using namespace std;

string Parsing(string regex, string strSettings)
{
    int lines = 0;
    ifstream infile("/etc/passwd");
    string line;
    if(infile.is_open()){
        while(!infile.eof())
        {
            getline(infile,line);
            lines++;
        }
        infile.close();
    }
    
    struct passwd *pswd;
    //stringstream ss;
    //ss << lines;
    string tmp = "";
    string temp;
    setpwent(); //začátek passwd
    while (1) //procházíme celý passwd
    {
        pswd = getpwent();
        if(pswd == NULL)
        {
            
            if (tmp.length()==0)
            {
                //cerr << "Error: Cant find login";
                return "Error: Login nelze nalezt.";
            }
            else return tmp;
        }
        if(strSettings.find("l") == 0)
        {
             temp = pswd->pw_name;
            if(temp.substr(0, regex.size()).compare(regex) == 0)
            {
                tmp += pswd->pw_name;
                tmp +="\n";
            }
        }
        else if(strSettings.find("f") == 0)
        {
            temp = pswd->pw_name;
            if(temp.compare(regex) == 0)
            {
                tmp += pswd->pw_dir;
                tmp +="\n";
            }
        }
        else if(strSettings.find("n")==0)
        {
            temp = pswd->pw_name;
            if(temp.compare(regex) == 0)
            {
                tmp += pswd->pw_gecos;
                tmp +="\n";
            }
        }
        if(pswd == NULL)
        return "";

    }
    return tmp;
}

int Server(string port)
{
    int sock, sock1, sLenght;
    static const int MAX = 4096;
    char buffer[MAX];
    int countOfRecv = 0;
    struct hostent * host;
    string strSettings; //string který je pro to aby sme vedeli co chceme zjistit
    string strLogin; //string s loginem nebo UIDčkem
    string strCast;
    struct sockaddr_in sockInput;
	if( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{       
        cerr << "Error: Vytvareni socketu" << endl;
		return -1;
	}
	sockInput.sin_family = AF_INET;
	sockInput.sin_port = htons(atoi(port.c_str()));
	sockInput.sin_addr.s_addr  = INADDR_ANY;
	if(bind(sock, (struct sockaddr *)&sockInput, sizeof(sockInput)) < 0) //priradi adresu socketu do proměnné
    {
        cerr << "Error: Pridelovani adresy socketu" << endl;
        return -1;
    }
	if(listen(sock, 5)) //maximální počet spojení 5 poté vyhodí chybu
    { 
        cerr << "Error: Nadvazani komunikace" << endl;
        return -1;
    }
    sLenght = sizeof(sockInput);
    cout<<"Ahoj jsem připraven"<<endl;
    while(true) //nekonečný cyklus
    {
        string answer = "";    //string co se bude posílat
        string founded = "";    // prijatý string        
        if((sock1 = accept(sock, (struct sockaddr *) &sockInput, (socklen_t*)&sLenght)) < 0) //accept spojení
        {
            cerr << "Error: Akceptovani spojeni" << endl;
            return(-1);
        }
        pid_t process = fork(); //fork nového procesu (rozdělení na dva procesy)
        if(process == 0)
        {
            host = (struct hostent *)gethostbyaddr((char *)&sockInput.sin_addr,4,AF_INET);
            //****************PŘÍJEM ZPRÁV OD KLIENTA*************
            while((countOfRecv = recv(sock1, buffer, sizeof(buffer), 0) ) > 0) //accept zprávy od klienta
            {
                if(countOfRecv < 0) //Pokud nastala chyba přijímání dat
                {
                    cerr << "Error: Prijimani dat" << endl;
                    return(-1);
                }
                founded.append(buffer,countOfRecv);
                if(countOfRecv > MAX - 1) //pokud je plný buffer ukončím příjem
                {
                    cout<<"PlnyBuff"<<endl;
                }
                else
                    break;
            }
            //************ZPRACOVÁNÍ CO POŠLEME ZPĚT*************   
            istringstream socketParse(founded.c_str()); 
            string header = "";
            getline(socketParse,header);
            if(header.compare("[xpetra19 myProtocol]")!= 0) //kontrola položky která se předává
            {
                cerr << "Error: připojení které neznám.";
                close(sock1);
                return(-1);
            }
            while(getline(socketParse, strSettings))
            {
                getline(socketParse, strLogin);
                answer = Parsing(strLogin,strSettings);
            }      
            answer.append("[End xpetra19 myProtocol]"); 
            //******************ODPOVĚĎ**********************
            if (send(sock1, answer.c_str(), strlen(answer.c_str()), 0) < 0)
            {
                cerr << "Error: zprava nebyla odeslana klientovy" << endl;
                return(-1);
            }
            if(close(sock1) < 0) //konec socketu
            { 
                cerr << "Error: ukonceni/uzavreni socketu" << endl;
                return(-1);
            } 
            break;
        }
        else if(process < 0) //chyba při vytvoření procesu
        {
        cerr << "Error: fork procesu neuspesny" << endl;
        close(sock1);
        return(-1);
        }
        
    }
    
    if (close(sock) < 0)    //konec socketu
    {
        cerr << "Error: ukonceni/uzavreni socketu" << endl;
        return(-1);
    }
    return(0);
}


void sigHandler(int signal) 
{
    cerr << "Error: Necekane uzavreni spojeni se servrem!!!"<< endl;
    exit(0);
}

int main (int argc, char *argv[])
{  
    int args;
    string port;
    signal(SIGINT, sigHandler);
    cout<<"Startuji!!"<<endl;
    while ((args = getopt(argc, argv, "p:")) != -1) //ziskam vsecky argumenty
    {
        switch (args)
        {
        case 'p':
            port = optarg;
            break;
        default: //pokud je nejaky jiny arg
            cerr << "Error: Spatne argumenty" << endl;
            return(-1);
            break;
        }
    }
    if(port.compare("") == 0) //nebyl zadan port
    {
        cerr << "Error: Spatne argumenty" << endl;
        return(-1);
    }
    if((port.find_first_not_of("0123456789") == string::npos) != true) //pokud neni port cislo
    {
        cerr << "Error: Spatne argumenty" << endl;
        return(-1);
    }
    if(atoi(port.c_str()) > 65535 || atoi(port.c_str()) < 1024) //pokud neni port v daném rozsahu
    {
        cerr << "Error: Port neni v rozsahu 1024 - 65535" << endl;
        return(-1);
    }
    if(optind != argc) //pokud je víc argumentů
    {
        cerr << "Error: Spatne argumenty" << endl;
        return(-1);
    }
    if(Server(port) == 0)
    {
        return(0);
    }
    else
    {
        return(-1);
    }
}