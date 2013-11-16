/***************************************************************************
 
    file                 : Client.h
    copyright            : (C) 2007 Daniele Loiacono
                           (C) 2013 Clara Caldeira
 
 ***************************************************************************/

/***************************************************************************

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.  

 ***************************************************************************/

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
#include <stdio.h>

using namespace std;



/** Holds the client code for UDP connections */
namespace client {

/*** defines for UDP *****/
#define UDP_MSGLEN 1000
#define UDP_CLIENT_TIMEUOT 1000000
/************************/



typedef int SOCKET;
typedef struct sockaddr_in tSockAddrIn;
#define CLOSE(x) close(x)
#define INVALID(x) x < 0

using namespace std;


/** Handles the connection */
void connect(unsigned int serverPort, string hostName, string id, WrapperBaseDriver *driver){

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

    // Create a socket (UDP on IPv4 protocol)
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (INVALID(socketDescriptor))
    {
        cout << "cannot create socket\n";
        exit(1);
    }

    // Set some fields in the serverAddress structure.
    serverAddress.sin_family = hostInfo->h_addrtype;
    memcpy((char *) &serverAddress.sin_addr.s_addr,
           hostInfo->h_addr_list[0], hostInfo->h_length);
    serverAddress.sin_port = htons(serverPort);

    bool shutdownClient=true;

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

            
            initString.insert(0,id);
           

            if (sendto(socketDescriptor, initString.c_str(), initString.length(), 0,
                       (struct sockaddr *) &serverAddress,
                       sizeof(serverAddress)) < 0)
            {
                cout << "cannot send data ";
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
                    cout << "didn't get response from server...";
                }
                else
                {
                    

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
                    cout << "didn't get response from server?";
                    CLOSE(socketDescriptor);
                    exit(1);
                }

                

                if (strcmp(buf,"***shutdown***")==0)
                {
                    driver->onShutdown();
                    shutdownClient = true;
                    break;
                }

                if (strcmp(buf,"***restart***")==0)
                {
                    shutdownClient=false;
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
                    cout << "cannot send data ";
                    CLOSE(socketDescriptor);
                    exit(1);
                }
                
            }
        }
    } while(shutdownClient==false);
    

    //if (shutdownClient==false)
    driver->onShutdown();
    CLOSE(socketDescriptor);

    

}

};

#endif /*CLIENT_H_*/