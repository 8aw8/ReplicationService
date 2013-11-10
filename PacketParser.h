#pragma once
#define BUFFER_SIZE 0x0FFF
#include <vector>
using namespace std;

struct MDIS_HeadPacket
{
  char prefix[4];
  int PacketType;
  int CodCommand;
  int Request_ID;
  int PacketSize;
  void set(char _prefix[4],  int _PacketType, int _CodCommand, int _Request_ID, int _PacketSize)
  {
	  prefix[0] = _prefix[0];
	  prefix[1] = _prefix[1];
	  prefix[2] = _prefix[2];
	  prefix[3] = _prefix[3];
	  PacketType = _PacketType;
  	  CodCommand = _CodCommand;
	  Request_ID = _Request_ID;
	  PacketSize = _PacketSize;
  }
  void create()
  {
	  prefix[0] = 'M';
	  prefix[1] = 'D';
	  prefix[2] = 'I';
	  prefix[3] = 'S';
	  PacketType = 0;
  	  CodCommand = 0;
	  Request_ID = 0;
	  PacketSize = 0;
  }
 void print()
	{
		printf("//======= Send/Reseive ====\n");
		printf("  Prefix: %.4s \n",prefix);
		printf("  Type: 0x%x \n",PacketType);
		printf("  Command: 0x%x \n",CodCommand);
		printf("  Request ID: 0x%x \n",Request_ID);
		printf("  Data Size: %ld \n",PacketSize);
		printf("//========================\n");
	}
};
struct MDIS_packet
{
	MDIS_HeadPacket * head;
	char * buffer;
};

class CPacketParser
{
private: 
 char * buffer;
 vector<MDIS_packet * > packet_list;
 int current_buf_size;
 int current_buf_data_end;
 char prefix [4];
private:
	int changeBufferSize(size_t new_size);
	int splitBuffer(char *buffer, int len);
	int analizeBuffer();
	void removeGarbageFromBeginning(int len);
	void addPacket(MDIS_HeadPacket * head, char * buffer);
public:
	CPacketParser(void);
	~CPacketParser(void);
public:
	int addBuffer(char* buffer, int len);
	MDIS_packet *getPacket();
};
