#pragma once
#include "Definitions.h"
#include <Winsock2.h>
#include <string>
#include "IPAddress.h"
#include "Client_sm.h"
#include <WS2tcpip.h>
#include <mutex>
#include <thread>

extern "C" class Client
{
public:
	EASYSOCKET_API Client(IPAddress ipAddress);
	EASYSOCKET_API bool Connect();
	EASYSOCKET_API bool Disconnect();


private:
	std::mutex* m_lock;
	ADDRINFO* m_hints;
	SOCKET* m_socket;
	IPAddress m_ipAddress;

	std::thread* m_thread;

	#pragma region State machine actions
	void NotifyFailureToConnect();
	void DisconnectFromServerAsync();
	void NotifyDisconnected();
	void NotifyFailureToDisconnect();
	void NotifyConnected();
	void CleanupSocket();
	void ConnectToServerAsync();
	#pragma endregion

	void OnConnectionSuccessful();
	void OnConnectionUnSuccessful();

	ClientContext* m_smContext;
	friend class ClientState;
	friend class ClientMap;
	friend class ClientMap_Disconnected;
	friend class ClientMap_Connecting;
	friend class ClientMap_Connected;
	friend class ClientMap_Disconnecting;
	friend class ClientMap_Disposed;
	friend class ClientContext;
};

