#pragma once
#include "client.h"
#include <vector>

using namespace std;
struct _ReplicPacket
{
	CString ReplicName;
	CString Server_IP;
	CString BD_name;
	CString DBSource;
	CString User;
	CString Password;
    CString Path;
	vector<int> call_id;
};

class CReplicPacket :public CClient
{
private:

public:
	//constructors
	CReplicPacket(_buffer buffer);
	virtual ~CReplicPacket(void);

  _ReplicPacket BufferToReplicPacket(_buffer buffer);
        _buffer ReplicPacketToBuffer(_ReplicPacket packet);
            int SendErrorPacket(SOCKET socket,_HeadPacket inHead,int error);
  _ReplicPacket m_packet;
};
