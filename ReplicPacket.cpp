#include "StdAfx.h"
#include ".\replicpacket.h"

void printPacket(char* Packet, int Len)
{
    printf("packet: ");
    for(int i=0; i<Len; i++)
         printf(" %d",Packet[i]);
    printf("\n");
} 

CReplicPacket::CReplicPacket(_buffer buffer): CClient(CReplicPacket::m_socket)
{ 
	m_packet=BufferToReplicPacket(buffer);
}
CReplicPacket::~CReplicPacket(void)
{
/*	
	switch(m_TypePacket)
		 {
				case 11://diagnostic packet
				      DeleteViewListReplic(ViewListPacket);
					break;
				case 23: //Replication packet
					{
				      delete [ ] m_packet->DB_name;
   	                  delete [ ] m_packet->Replic_name;
   	                  delete [ ] m_packet->SQLQuery;
	                  delete m_packet;
					}
				    break;
				default:
					the_log.LogT(2,"Type not found!!!"); 
				break;
		 }
*/
 }
_buffer CReplicPacket::ReplicPacketToBuffer (_ReplicPacket packet)
{
	_buffer res;
	INT SizePacket= packet.ReplicName.GetLength()+1+
                    packet.Server_IP.GetLength()+1+
					packet.BD_name.GetLength()+1+
					packet.DBSource.GetLength()+1+
					packet.User.GetLength()+1+
					packet.Password.GetLength()+1+
					packet.Path.GetLength()+1+
					sizeof(INT);//call_id size
	  INT call_size = (size_t)packet.call_id.size();
	  SizePacket=SizePacket+(call_size*sizeof(INT));

	res.buffer = new char[SizePacket];
	res.SizeBuffer=SizePacket;
    
  char*	temp=res.buffer;

  strcpy(temp,packet.ReplicName.GetBuffer());temp+=packet.ReplicName.GetLength()+1;
  strcpy(temp,packet.Server_IP.GetBuffer());temp+=packet.Server_IP.GetLength()+1;
  strcpy(temp,packet.BD_name.GetBuffer());temp+=packet.BD_name.GetLength()+1;
  strcpy(temp,packet.DBSource.GetBuffer());temp+=packet.DBSource.GetLength()+1;
  strcpy(temp,packet.User.GetBuffer());temp+=packet.User.GetLength()+1;
  strcpy(temp,packet.Password.GetBuffer());temp+=packet.Password.GetLength()+1;
  strcpy(temp,packet.Path.GetBuffer());temp+=packet.Path.GetLength()+1;
 
  memcpy(temp,(char*)&call_size,sizeof(INT));temp+=sizeof(INT);
 	for (int i = 0; i< packet.call_id.size();i++ )
    {
		INT id=packet.call_id[i];
        memcpy(temp,(char*)&id,sizeof(INT));temp+=sizeof(INT);
		printf("id= %ld \n",id);
    }
	return res;
}
_ReplicPacket CReplicPacket::BufferToReplicPacket(_buffer buffer)
{
	_ReplicPacket packet;
	
	char* temp=buffer.buffer; 
    
	int i=0;
   	
	while ((temp[i]!=0)&&(i<buffer.SizeBuffer))	{ packet.ReplicName+=temp[i]; i++; }
	printf("field = %s \n",packet.ReplicName.GetBuffer());
	i++;	
	while ((temp[i]!=0)&&(i<buffer.SizeBuffer)) { packet.Server_IP+=temp[i]; i++; }
	printf("field = %s \n",packet.Server_IP.GetBuffer());
	i++;
	while ((temp[i]!=0)&&(i<buffer.SizeBuffer))	{ packet.BD_name+=temp[i]; i++; }
	printf("field = %s \n",packet.BD_name.GetBuffer());
	i++;
	while ((temp[i]!=0)&&(i<buffer.SizeBuffer))	{ packet.DBSource+=temp[i]; i++; }
	printf("field = %s \n",packet.DBSource.GetBuffer());
	i++;
	while ((temp[i]!=0)&&(i<buffer.SizeBuffer))	{ packet.User+=temp[i]; i++; }
	printf("field = %s \n",packet.User.GetBuffer());
	i++;
	while ((temp[i]!=0)&&(i<buffer.SizeBuffer))	{ packet.Password+=temp[i]; i++; }
	printf("field = %s \n",packet.Password.GetBuffer());
	i++;
	while ((temp[i]!=0)&&(i<buffer.SizeBuffer))	{ packet.Path+=temp[i]; i++; }
	printf("field = %s \n",packet.Path.GetBuffer());
	i++;
    
	if (i<buffer.SizeBuffer)
	{
	temp+=i;
    INT call_size=0;
	memcpy(&call_size,temp,sizeof(INT));temp+=sizeof(INT);
	printf("call_size = %ld\n",call_size);

	for (int i = 0; i< call_size;i++ )
    {
		INT id = 0;
		memcpy((char*)&id,temp,sizeof(INT));temp+=sizeof(INT);
		packet.call_id.push_back(id);
    }
	
	for (int i = 0; i< packet.call_id.size();i++ )
    {
		INT id=packet.call_id[i];
        printf("id= %ld \n",id);
    }
	}//if (i<buffer.SizeBuffer)
	
	return packet;
}
int CReplicPacket::SendErrorPacket(SOCKET socket,_HeadPacket inHead,int error)
{
	_HeadPacket head;
     memcpy(&head.prefix,"MDIS",4);
	 head.PacketType=0xf100;
	 head.CodCommand=error;
	 head.Request_ID=inHead.Request_ID;
     head.PacketSize=0;

	int BytesSend = send(socket,(char*)&head,sizeof(_HeadPacket),0); 
	return BytesSend;
}


