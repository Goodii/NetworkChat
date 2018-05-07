#include "Client.h"
#include <iostream>
#include "GameMessages.h"

using std::cout;

Client::Client() {

}

Client::~Client() {
}

bool Client::startup() {

	std::cout << "Enter a username: "; std::cin >> m_user.username;

	handleNetworkConnection();

	update();

	return true;
}

void Client::shutdown() {

	
}

void Client::update() {

	while (true)
	{		
		//multithread network updates and input
		handleNetworkMessages();
		
		//if (typing)
		//{
		//	typing = false;
		//	std::cin >> m_user.message;
		//	sendClientData();
		//}

	}
}

void Client::handleNetworkConnection()
{
	//initialise the raknet peer interface first
	m_pPeerInterface = RakNet::RakPeerInterface::GetInstance();
	initialiseClientConnection();
}

void Client::initialiseClientConnection()
{
	//Create a socket descriptor to describe this connection
	//no data needed, as we will be connecting to a server
	RakNet::SocketDescriptor sd;

	//Now call startup - max of 1 connection (to the server)
	m_pPeerInterface->Startup(1, &sd, 1);

	cout << "Connecting to server at: " << IP << std::endl;

	//call connect to attempt to connect to the given server
	RakNet::ConnectionAttemptResult res = m_pPeerInterface->Connect(IP, PORT, nullptr, 0);

	//finaly, check to see if we connected, and if not, throw an error
	if (res != RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		cout << "Unable to start connection, Error number: " << res << std::endl;
	}
}

void Client::handleNetworkMessages()
{
	RakNet::Packet* packet;

	for (packet = m_pPeerInterface->Receive(); packet;
		m_pPeerInterface->DeallocatePacket(packet),
		packet = m_pPeerInterface->Receive())
	{
		switch (packet->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			cout << "Another client has disconnected.\n";
			break;
		case ID_REMOTE_CONNECTION_LOST:
			cout << "Another client has lost connection.\n";
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			cout << "Another client has connected.\n";
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			cout << "Our connection request has been accepted.\n";
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			cout << "The server is full.\n";
			break;
		case ID_CONNECTION_LOST:
			cout << "Connection lost.\n";
			break;
		case ID_SERVER_SET_CLIENT_ID:
			onSetClientIDPacket(packet);
			break;
		default:
			cout << "Received a text message with an unkown id: " << packet->data[0] << std::endl;
			break;
		}
	}
}

void Client::onSetClientIDPacket(RakNet::Packet* packet)
{
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(m_clientID);

	cout << "Set my client ID to: " << m_clientID << std::endl;
}

void Client::sendClientData()
{
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)GameMessages::ID_CLIENT_CLIENT_DATA);
	bs.Write(m_clientID);
	bs.Write((char*)&m_user, sizeof(UserData));

	m_pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
		RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::onReceivedClientDataPacket(RakNet::Packet* packet)
{
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	int clientID;
	bsIn.Read(clientID);

	//if clientID does not match our ID, update our client GameObject information
	if (clientID != m_clientID)
	{
		UserData clientData;
		bsIn.Read((char*)&clientData, sizeof(UserData));
		
		//output GameObject information to console
		cout << clientData.username;// << " : " << clientData.message << std::endl;
	}
}
