#pragma once
#include "Definitions.h"
#include <string>


extern "C" class IPAddress
{
private:
	std::string m_ipAddress;
	int m_port;


public:
	EASYSOCKET_API IPAddress();
	EASYSOCKET_API PCSTR GetIp();
	EASYSOCKET_API PCSTR GetPort();

	EASYSOCKET_API static IPAddress Parse(std::string ipAddress, int port);
};

