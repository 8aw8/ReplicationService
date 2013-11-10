#pragma once
#include "StdAfx.h"
#include "packetparser.h"


////////////////////private////////////////////////////////////////////////////////
int CPacketParser::changeBufferSize(size_t new_size)
{
	char *temp = new char[new_size];
	memset(temp,0,new_size);
	char *old_buffer = this->buffer;
	int size_to_copy =this->current_buf_size;
	if(new_size<size_to_copy) size_to_copy = new_size;
	memcpy(temp,old_buffer,size_to_copy);
	delete [] old_buffer;
	this->buffer = temp;
	this->current_buf_size = new_size;
	return 0;
}
int CPacketParser::splitBuffer(char *buffer, int len)
{
	if((this->current_buf_data_end+len)>this->current_buf_size) 
		if(this->changeBufferSize(this->current_buf_data_end+len)!=0) return -1; 
	memcpy(this->buffer+this->current_buf_data_end, buffer, len);
	this->current_buf_data_end+=len;
	return 0;
}
void CPacketParser::removeGarbageFromBeginning(int len)
{
	memcpy(this->buffer, this->buffer + len, this->current_buf_data_end-len);
	this->current_buf_data_end -= len;
}
void CPacketParser::addPacket(MDIS_HeadPacket * head, char * buffer)
{
	MDIS_packet * packet = new MDIS_packet;
	packet->head = head;
	packet->buffer = buffer;
	this->packet_list.push_back(packet);
}

int CPacketParser::analizeBuffer()
{
	int prefix_size = 4;
	char prefix[4+1] = {'M','D','I','S','\0'};
	char temp[4+1] =  {'\0','\0','\0','\0','\0'};	
	int i=0;
	while(i<this->current_buf_data_end-prefix_size) 
	{
		memcpy((char *)&temp,this->buffer+i,prefix_size);
		 //ищем prefix
		if(strcmp(temp,prefix)==0)//ура префикс найден
		{
			if((i+sizeof(MDIS_HeadPacket))>this->current_buf_data_end)//заголовок пакета в буфер не влез
			{//удаляем мусор от начала
				removeGarbageFromBeginning(i);
				return 0;
			} else {//по крайней мере имеем заголовок
				MDIS_HeadPacket * head = new MDIS_HeadPacket;
				memcpy(head,this->buffer+i,sizeof(MDIS_HeadPacket));
				i+=sizeof(MDIS_HeadPacket); // проскочили на размер заголовка;				
				if(head->PacketSize==0) //после заголовка ничего нет
				{
					addPacket(head, NULL);
					continue;
				} else { //есть данные проверяем дальше
					if((head->PacketSize+i)>this->current_buf_data_end){//данные не влезли, ожидаем
						i-=sizeof(MDIS_HeadPacket); 
						removeGarbageFromBeginning(i);
						return 0;
					} else {
						char * temp_buff =  new char[head->PacketSize];
						memcpy(temp_buff,this->buffer+i,head->PacketSize);
						i+=head->PacketSize;//проскочили на размер данных
						addPacket(head, temp_buff);
						continue;
					}
				}
			}
		}
		i++;
	}
	removeGarbageFromBeginning(i);
	return 0;
}
////////////////////public////////////////////////////////////////////////////////
MDIS_packet * CPacketParser::getPacket()
{
	if(this->packet_list.size()>0)
	{
		MDIS_packet * res = NULL;
		res = packet_list.at(0);
		packet_list.erase(packet_list.begin());
		return res;
	} else return NULL;
}
int CPacketParser::addBuffer(char* buffer, int len)
{
	if(splitBuffer(buffer, len)!=0) return -1;
	analizeBuffer();
	return 0;
}

CPacketParser::CPacketParser(void)
{	
	this->buffer = new char[BUFFER_SIZE];
	memset(this->buffer,0,BUFFER_SIZE);
	this->current_buf_size = BUFFER_SIZE;
	this->current_buf_data_end = 0;
}
CPacketParser::~CPacketParser(void)
{
	delete [] this->buffer;
}

