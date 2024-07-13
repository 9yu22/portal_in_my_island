#include "Session.h"

Session::Session()
{
	b_use = false;
	m_prev_remain = 0;
}

Session::~Session()
{
}

void Session::do_recv()
{
	DWORD recv_flag = 0;
	memset(&_recv_over._over, 0, sizeof(_recv_over._over));
	_recv_over._wsabuf.len = BUF_SIZE - m_prev_remain;
	_recv_over._wsabuf.buf = _recv_over._send_buf + m_prev_remain;
	WSARecv(m_socket, &_recv_over._wsabuf, 1, 0, &recv_flag,
		&_recv_over._over, 0);
}

void Session::do_send(void* packet)
{
	EX_OVERLAPPED* sdata = new EX_OVERLAPPED{ reinterpret_cast<unsigned char*>(packet) };
	WSASend(m_socket, &sdata->_wsabuf, 1, 0, 0, &sdata->_over, 0);
}

void Session::send_login_player_packet()
{
	SC_LOGIN_INFO_PACKET login_player;
	login_player.id = m_player.m_id;
	login_player.size = sizeof(SC_LOGIN_INFO_PACKET);
	login_player.type = SC_LOGIN_INFO;
	login_player.x = m_player.location.x;
	login_player.y = m_player.location.y;
	login_player.z = m_player.location.z;
	//std::cout << "id: " << p.id << " x:" << p.x << " y: " << p.y << " z: " << p.z << " �α��� ��Ŷ ����" << std::endl;
	do_send(&login_player);
}

void Session::send_move_player_packet(Player player)
{
	SC_MOVE_PLAYER_PACKET move_player;
	move_player.id = player.m_id;
	move_player.size = sizeof(SC_MOVE_PLAYER_PACKET);
	move_player.type = SC_MOVE_PLAYER;
	move_player.x = player.location.x;
	move_player.y = player.location.y;
	move_player.z = player.location.z;
	move_player.pitch = player.m_rotation.pitch;
	move_player.yaw = player.m_rotation.yaw;
	move_player.roll = player.m_rotation.roll;
	//std::cout << "Ŭ���̾�Ʈ " << c_id << "x:" << p.x << " y : " << p.y << " z : " << p.z << " ���� ��Ŷ ����" << std::endl << std::endl;
	do_send(&move_player);
}

void Session::send_add_player_packet(Player player)
{
	SC_ADD_PLAYER_PACKET add_player;
	add_player.id = player.m_id;
	//strcpy_s(add_packet.name, p->name);
	add_player.size = sizeof(add_player);
	add_player.type = SC_ADD_PLAYER;
	add_player.x = player.location.x;
	add_player.y = player.location.y;
	add_player.z = player.location.z;
	do_send(&add_player);
}

void Session::send_add_block_packet(char* packet)
{
	SC_ADD_BLOCK_PACKET* add_block = reinterpret_cast<SC_ADD_BLOCK_PACKET*>(packet);
	add_block->size = sizeof(SC_ADD_BLOCK_PACKET);
	add_block->type = SC_ADD_BLOCK;
	do_send(add_block);
}

void Session::send_remove_block_packet(char* packet)
{
	SC_REMOVE_BLOCK_PACKET* remove_block = reinterpret_cast<SC_REMOVE_BLOCK_PACKET*>(packet);
	remove_block->size = sizeof(SC_REMOVE_BLOCK_PACKET);
	remove_block->type = SC_REMOVE_BLOCK;
	do_send(remove_block);
}

void Session::send_hp_packet(Player player)
{
	SC_CHANGE_HP_PACKET change_hp;
	change_hp.size = sizeof(SC_CHANGE_HP_PACKET);
	change_hp.type = SC_CHANGE_HP;
	change_hp.hp = player.m_hp;
	change_hp.id = player.m_id;
	do_send(&change_hp);
}