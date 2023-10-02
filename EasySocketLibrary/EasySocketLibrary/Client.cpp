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
	if (m_thread != nullptr) {
		return;
	}

	m_thread = new std::thread([this](){

		WSADATA wsaData;
		SOCKET connectSocket = INVALID_SOCKET;
		struct addrinfo* result = NULL, * ptr = NULL, hints;
		int iResult;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			m_socket = &connectSocket;
			OnThreadComplete();
			OnConnectionUnSuccessful();
			return;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		//iResult = getaddrinfo(m_ipAddress.GetIp(), m_ipAddress.GetPort(), &hints, &result);
		iResult = getaddrinfo(m_ipAddress.GetIp().c_str(), m_ipAddress.GetPort().c_str(), &hints, &result);
		if (iResult != 0) {
			m_socket = &connectSocket;
			OnThreadComplete();
			OnConnectionUnSuccessful();
			return;
		}

		// Create a socket object
		connectSocket = socket(result->ai_family, result->ai_socktype,
			result->ai_protocol);


		// Failed to create a socket
		if (connectSocket == INVALID_SOCKET) {
			m_socket = &connectSocket;
			OnThreadComplete();
			OnConnectionUnSuccessful();
			return;
		}

		// Connect to the socket
		iResult = connect(connectSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			m_socket = &connectSocket;
			auto errorCode = WSAGetLastError();
			closesocket(connectSocket);
			OnConnectionUnSuccessful();
			OnThreadComplete();
			return;
		}
		
		// Success
		m_socket = &connectSocket;
		OnThreadComplete();
		OnConnectionSuccessful();
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

void Client::OnThreadComplete() {
	delete(m_thread);
	m_thread = nullptr;
}