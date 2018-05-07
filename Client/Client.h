#pragma once

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>

struct UserData
{
	std::string username;
	std::string message;
};

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

	void onSetClientIDPacket(RakNet::Packet*);
	void sendClientData();
	void onReceivedClientDataPacket(RakNet::Packet*);

	//handle incoming packets
	void handleNetworkMessages();

protected:
	void enterMessage();

	RakNet::RakPeerInterface* m_pPeerInterface;
	int m_clientID;
	UserData m_user;

	bool typing;

	const char* IP = "127.0.0.1";
	const unsigned short PORT = 5456;
};