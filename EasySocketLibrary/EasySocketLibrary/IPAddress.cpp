#include "pch.h"
#include "IPAddress.h"
#include <WS2tcpip.h>
#include <stdexcept>
#include <atlstr.h>

IPAddress::IPAddress(std::string ipAddress, int port)
{
    m_ipAddress = ipAddress;
    m_port = port;
    
}

std::string IPAddress::GetIp()
{
    return m_ipAddress;
}

std::string IPAddress::GetPort()
{
    return std::to_string(m_port);
}

IPAddress::IPAddress()
{
    m_ipAddress = "";
    m_port = 0;
}

IPAddress IPAddress::Parse(std::string ipAddress, int port)
{
    // Convert std string to windows unicode string
    std::wstring stemp = std::wstring(ipAddress.begin(), ipAddress.end());
    LPCWSTR unicodeIpString = stemp.c_str();

    const char errorChar = 0x18; // 

    char formattedIp[1024];
    formattedIp[0] = errorChar;

    InetPton(2, unicodeIpString, formattedIp);

    if (formattedIp[0] == errorChar) {
        throw std::invalid_argument("Invalid IP Address provided");
    }


    return IPAddress(ipAddress, port);
}
