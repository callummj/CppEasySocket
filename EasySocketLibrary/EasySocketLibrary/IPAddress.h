#pragma once
#include "Definitions.h"
#include <string>


extern "C" class IPAddress
{
private:
	std::string m_ipAddress;
	int m_port;
	IPAddress(std::string ipAddress, int port);

public:
	EASYSOCKET_API std::string GetIp();
	EASYSOCKET_API std::string GetPort();
	EASYSOCKET_API IPAddress();

	EASYSOCKET_API static IPAddress Parse(std::string ipAddress, int port);
};

