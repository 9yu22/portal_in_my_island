// Fill out your copyright notice in the Description page of Project Settings.


#include "S1.h"
//#include "Modules/ModuleManager.h"
//IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultModuleImpl, S1, "S1");
// -->blockers에 정의되어 있으므로 여기서 정의하면 안됨.

//--------SendBuffer--------

SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.SetNum(bufferSize);
}

SendBuffer::~SendBuffer()
{

}

BYTE* SendBuffer::Buffer()
{
	return _buffer.GetData();
}

int32 SendBuffer::WriteSize()
{
	return _writeSize;
}

int32 SendBuffer::Capacity()
{
	return static_cast<int32>(_buffer.Num());
}

void SendBuffer::CopyData(void* data, int32 len)
{
	::memcpy(_buffer.GetData(), data, len);
	_writeSize = len;
}

void SendBuffer::Close(uint32 writeSize)
{
	_writeSize = _writeSize;
}
