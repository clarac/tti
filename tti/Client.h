#ifndef CLIENT_H_
#define CLIENT_H_

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "WrapperBaseDriver.h"
#include "SimpleParser.h"

using namespace std;

class SimpleDriver;

namespace client {

	/*** defines for UDP *****/
#define UDP_MSGLEN 1000
#define UDP_CLIENT_TIMEUOT 1000000
//#define __UDP_CLIENT_VERBOSE__
/************************/

#ifdef __UDP_CLIENT_VERBOSE__
#define _print(a) cout << a << endl;
#define _print2(a,b) cout << a << b << endl;
#else
#define _print(a) 
#define _print2(a,b) 
#endif

typedef int SOCKET;
typedef struct sockaddr_in tSockAddrIn;
#define CLOSE(x) close(x)
#define INVALID(x) x < 0

using namespace std;



void connect(unsigned int serverPort, string hostName, string id, WrapperBaseDriver *driver)
{
    SOCKET socketDescriptor;
    int numRead;

    tSockAddrIn serverAddress;
    struct hostent *hostInfo;
    struct timeval timeVal;
    fd_set readSet;
    char buf[UDP_MSGLEN];

    hostInfo = gethostbyname(hostName.c_str());

    if (hostInfo == NULL)
    {
        cout << "Error: problem interpreting host: " << hostName << "\n";
        exit(1);
    }

    // Print command line option used
    //cout << "***********************************" << endl;
    //cout << "HOST: "   << hostName    << endl;
    //cout << "PORT: " << serverPort  << endl;
    //cout << "ID: "   << id     << endl;
    //cout << "***********************************" << endl;

    // Create a socket (UDP on IPv4 protocol)
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (INVALID(socketDescriptor))
    {
        cerr << "cannot create socket\n";
        exit(1);
    }

    // Set some fields in the serverAddress structure.
    serverAddress.sin_family = hostInfo->h_addrtype;
    memcpy((char *) &serverAddress.sin_addr.s_addr,
           hostInfo->h_addr_list[0], hostInfo->h_length);
    serverAddress.sin_port = htons(serverPort);

    bool shutdownClient=false;

    do
    {
        /***********************************************************************************
        ************************* UDP client identification ********************************
        ***********************************************************************************/
        do
        {
        	// Initialize the angles of rangefinders
        	float angles[19];
        	driver->init(angles);

        	string initString = SimpleParser::stringify(string("init"),angles,19);

            _print2("Sending id to server: ", id);
            initString.insert(0,id);
            _print2("Sending init string to the server: ",initString);

            if (sendto(socketDescriptor, initString.c_str(), initString.length(), 0,
                       (struct sockaddr *) &serverAddress,
                       sizeof(serverAddress)) < 0)
            {
                cerr << "cannot send data ";
                CLOSE(socketDescriptor);
                exit(1);
            }

            // wait until answer comes back, for up to UDP_CLIENT_TIMEUOT micro sec
            FD_ZERO(&readSet);
            FD_SET(socketDescriptor, &readSet);
            timeVal.tv_sec = 0;
            timeVal.tv_usec = UDP_CLIENT_TIMEUOT;

            if (select(socketDescriptor+1, &readSet, NULL, NULL, &timeVal))
            {
                // Read data sent by the solorace server
                memset(buf, 0x0, UDP_MSGLEN);  // Zero out the buffer.
                numRead = recv(socketDescriptor, buf, UDP_MSGLEN, 0);
                if (numRead < 0)
                {
                    cerr << "didn't get response from server...";
                }
				else
				{
                	_print2("Received: ",buf);

                	if (strcmp(buf,"***identified***")==0)
                    		break;
            	}
	      	}

        }  while(1);

        while(1)
        {
            // wait until answer comes back, for up to UDP_CLIENT_TIMEUOT micro sec
            FD_ZERO(&readSet);
            FD_SET(socketDescriptor, &readSet);
            timeVal.tv_sec = 0;
            timeVal.tv_usec = UDP_CLIENT_TIMEUOT;

            if (select(socketDescriptor+1, &readSet, NULL, NULL, &timeVal))
            {
                // Read data sent by the solorace server
                memset(buf, 0x0, UDP_MSGLEN);  // Zero out the buffer.
                numRead = recv(socketDescriptor, buf, UDP_MSGLEN, 0);
                if (numRead < 0)
                {
                    cerr << "didn't get response from server?";
                    CLOSE(socketDescriptor);
                    exit(1);
                }

                _print2("Received: ",buf);

                if (strcmp(buf,"***shutdown***")==0)
                {
                    driver->onShutdown();
                    shutdownClient = true;
                    //cout << "Client Shutdown" << endl;
                    break;
                }

                if (strcmp(buf,"***restart***")==0)
                {
                    driver->onRestart();
                    cout << "Client Restart" << endl;
                    break;
                }
                /**************************************************
                 * Compute The Action to send to the solorace sever
                 **************************************************/

            	string action = driver->drive(string(buf));
            	memset(buf, 0x0, UDP_MSGLEN);
		        sprintf(buf,"%s",action.c_str());

                if (sendto(socketDescriptor, buf, strlen(buf)+1, 0,
                           (struct sockaddr *) &serverAddress,
                           sizeof(serverAddress)) < 0)
                {
                    cerr << "cannot send data ";
                    CLOSE(socketDescriptor);
                    exit(1);
                }
                _print2("Sending ",buf);
            }
            else
            {
                //cout << "** Server did not respond in 1 second.\n";
            }
        }
    } while(shutdownClient==false);

    if (shutdownClient==false)
	driver->onShutdown();
    CLOSE(socketDescriptor);

    

}

};

#endif /*CLIENT_H_*/