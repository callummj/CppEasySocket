#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "Client.h"
#include <string>
#include <IPAddress.h>
#include <WS2tcpip.h>

EASYSOCKET_API Client::Client(IPAddress ip)
{
	m_ipAddress = ip;
	m_lock = new std::mutex();
	m_smContext = new ClientContext(*this);

	auto defaultSocket = INVALID_SOCKET;
	m_socket = &defaultSocket;

	auto hints = ADDRINFO();
	m_hints = &hints;
	

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}

EASYSOCKET_API bool Client::Connect()
{
	m_lock->lock();
	m_smContext->Connect();
	m_lock->unlock();
	
	return true;
}

EASYSOCKET_API bool Client::Disconnect()
{
	return false;
}


void Client::NotifyFailureToConnect()
{
}

void Client::DisconnectFromServerAsync()
{
}

void Client::NotifyDisconnected()
{
}

void Client::NotifyFailureToDisconnect()
{
}

void Client::NotifyConnected()
{
}

void Client::CleanupSocket()
{
}

void Client::ConnectToServerAsync()
{
	m_thread = new std::thread([this](){
		PADDRINFOA* result = new PADDRINFOA();
		auto iResult = getaddrinfo(m_ipAddress.GetIp(), m_ipAddress.GetPort(), m_hints, result);
		delete result;

		if (iResult != 0)
		{
			OnConnectionUnSuccessful();
		}
		else {
			OnConnectionSuccessful();
		}
	});
	
	m_thread->detach();
}

void Client::OnConnectionUnSuccessful()
{
	m_lock->lock();
	m_smContext->ConnectionUnsuccessful();
	m_lock->unlock();
}

void Client::OnConnectionSuccessful()
{
	m_lock->lock();
	m_smContext->ConnectionSuccessful();
	m_lock->unlock();
}