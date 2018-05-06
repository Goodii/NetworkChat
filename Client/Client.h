#pragma once

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>

class Client {
public:

	Client();
	virtual ~Client();

	virtual bool startup();
	virtual void shutdown();

	virtual void update();

	//initialise connection
	void handleNetworkConnection();
	void initialiseClientConnection();

	//handle incoming packets
	void handleNetworkMessages();

protected:

	RakNet::RakPeerInterface* m_pPeerInterface;

	const char* IP = "127.0.0.1";
	const unsigned short PORT = 5456;
};