#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "SimpleDriver.h"

class Client {
public:
	static int connect(unsigned int serverPort, string hostName, string id, SimpleDriver driver);
};