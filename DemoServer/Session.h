#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "Player.h"
#include "protocol.h"

class EX_OVERLAPPED {
public:
	WSAOVERLAPPED _over;
	WSABUF _wsabuf;
	char _send_buf[BUF_SIZE];
	OP_TYPE _op_type;
	char _packet_type;
	EX_OVERLAPPED()
	{
		_wsabuf.len = BUF_SIZE;
		_wsabuf.buf = _send_buf;
		_op_type = OP_RECV;
		ZeroMemory(&_over, sizeof(_over));
	}
	EX_OVERLAPPED(unsigned char* packet)
	{
		_wsabuf.len = packet[0];
		_wsabuf.buf = _send_buf;
		ZeroMemory(&_over, sizeof(_over));
		_op_type = OP_SEND;
		_packet_type = packet[1];
		memcpy(_send_buf, packet, packet[0]);
	}
};

class Session
{
	EX_OVERLAPPED _recv_over;

public:
	SOCKET m_socket;
	Player m_player;

	int m_prev_remain;
	bool b_use;

public:
	Session();
	~Session();

	void do_recv();
	void do_send(void* packet);

	void send_login_player_packet();
	void send_move_player_packet(Player player);
	void send_add_player_packet(Player player);
	void send_add_block_packet(char* packet);
	void send_remove_block_packet(char* packet);
	void send_hp_packet(Player player);
};
