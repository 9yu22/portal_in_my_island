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

void Session::send_login_info_packet()
{
	SC_LOGIN_INFO_PACKET p;
	p.id = m_player.m_id;
	p.size = sizeof(SC_LOGIN_INFO_PACKET);
	p.type = SC_LOGIN_INFO;
	p.x = m_player.location.x;
	p.y = m_player.location.y;
	p.z = m_player.location.z;
	//std::cout << "id: " << p.id << " x:" << p.x << " y: " << p.y << " z: " << p.z << " 로그인 패킷 전송" << std::endl;
	do_send(&p);
}

void Session::send_move_packet(Player player)
{
	SC_MOVE_PLAYER_PACKET p;
	p.id = player.m_id;
	p.size = sizeof(SC_MOVE_PLAYER_PACKET);
	p.type = SC_MOVE_PLAYER;
	p.x = player.location.x;
	p.y = player.location.y;
	p.z = player.location.z;
	p.pitch = player.m_rotation.pitch;
	p.yaw = player.m_rotation.yaw;
	p.roll = player.m_rotation.roll;
	//std::cout << "클라이언트 " << c_id << "x:" << p.x << " y : " << p.y << " z : " << p.z << " 무브 패킷 전송" << std::endl << std::endl;
	do_send(&p);
}

void Session::send_add_block_packet(char* packet)
{
	SC_ADD_BLOCK_PACKET* p = reinterpret_cast<SC_ADD_BLOCK_PACKET*>(packet);
	p->size = sizeof(SC_ADD_BLOCK_PACKET);
	p->type = SC_ADD_BLOCK;
	do_send(p);
}

void Session::send_remove_block_packet(char* packet)
{
	SC_REMOVE_BLOCK_PACKET* p = reinterpret_cast<SC_REMOVE_BLOCK_PACKET*>(packet);
	p->size = sizeof(SC_REMOVE_BLOCK_PACKET);
	p->type = SC_REMOVE_BLOCK;
	do_send(p);
}

void Session::send_hp_packet(Player player)
{
	SC_CHANGE_HP_PACKET p;
	p.size = sizeof(SC_CHANGE_HP_PACKET);
	p.type = SC_CHANGE_HP;
	p.hp = player.m_hp;
	p.id = player.m_id;
	do_send(&p);
}
