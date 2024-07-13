#include <iostream>
#include <array>
//#include <WS2tcpip.h>
//#include <MSWSock.h>
#include "Session.h"
#include "protocol.h"
#include "Map.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")
using namespace std;
constexpr int MAX_USER = 4;

array<Session, MAX_USER> clients;
Map map;
//

int get_new_client_id()
{
	for (int i = 0; i < MAX_USER; ++i)
		if (clients[i].b_use == false)
			return i;
	return -1;
}

void disconnect(int c_id)
{
	std::cout << "클라이언트 " << c_id << " 연결 끊김" << std::endl;
	closesocket(clients[c_id].m_socket);
	clients[c_id].b_use = false;
}

void process_packet(int c_id, char* packet)
{
	switch (packet[1]) {
	case CS_LOGIN: {
		std::cout << "클라이언트 " << c_id << " 로그인 패킷 도착" << std::endl;
		//CS_LOGIN_PACKET* p = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
		//strcpy_s(clients[c_id]._name, p->name);
		clients[c_id].send_login_player_packet();

		std::cout << std::endl;
		for (auto& pl : clients) { // 새로 추가된 클라를 모두에게 전송
			if (false == pl.b_use) continue;
			if (pl.m_player.m_id == c_id) continue;
			pl.send_add_player_packet(clients[c_id].m_player);

			//pl.do_send(&add_packet);
			std::cout << "클라이언트 " << pl.m_player.m_id << "에게 " << c_id << "의 ADD 패킷을 전송" << std::endl;
		}
		for (auto& pl : clients) { // 새로 추가된 클라에게 모두 전송
			if (false == pl.b_use) continue;
			if (pl.m_player.m_id == c_id) continue;
			clients[c_id].send_add_player_packet(pl.m_player);

			std::cout << "클라이언트 " << c_id <<"에게 " << pl.m_player.m_id << "의 ADD 패킷을 전송" << std::endl;
		}
		break;
	}
	case CS_MOVE: {
		
		CS_MOVE_PACKET* p = reinterpret_cast<CS_MOVE_PACKET*>(packet);
		clients[c_id].m_player.SetWorldLocation(p->x, p->y, p->z);
		clients[c_id].m_player.SetWorldRotation(p->pitch, p->yaw, p->roll);
		//std::cout << "클라이언트 " <<c_id << "x:" << p->x << " y : " << p->y << " z : " << p->z << " 무브 패킷 도착" << std::endl;
		for (auto& pl : clients)
			if (true == pl.b_use && pl.m_player.m_id != c_id)
				pl.send_move_player_packet(clients[c_id].m_player);
		break;
	}
	case CS_ADD_BLOCK: {
		CS_ADD_BLOCK_PACKET* p = reinterpret_cast<CS_ADD_BLOCK_PACKET*>(packet);
		if (map.AddBlockToMap(p)) {
			for (auto& pl : clients)
				if (true == pl.b_use)
					pl.send_add_block_packet(packet);
		}
			
		break;
	}
	case CS_REMOVE_BLOCK: {
		CS_REMOVE_BLOCK_PACKET* p = reinterpret_cast<CS_REMOVE_BLOCK_PACKET*>(packet);
		if (map.RemoveBlockToMap(p)) {
			for (auto& pl : clients)
				if (true == pl.b_use)
					pl.send_remove_block_packet(packet);
		}		
				
		break;
	}

	case CS_CHANGE_HP: {
		CS_CHANGE_HP_PACKET* p = reinterpret_cast<CS_CHANGE_HP_PACKET*>(packet);
		clients[p->hit_player_id].m_player.m_hp -= 20;
		std::cout << "클라이언트 " << p->hit_player_id << " 공격당함, 남은 hp: " << clients[p->hit_player_id].m_player.m_hp << std::endl;
		clients[p->hit_player_id].send_hp_packet(clients[p->hit_player_id].m_player);
		break;
	}
	case CS_DISCONNECT:
		CS_DISCONNECT_PACKET* p = reinterpret_cast<CS_DISCONNECT_PACKET*>(packet);
		disconnect(p->id);
	}

	
}

int main()
{
	HANDLE h_iocp;

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET server = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(server, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	listen(server, SOMAXCONN);
	SOCKADDR_IN cl_addr;
	int addr_size = sizeof(cl_addr);
	int client_id = 0;

	h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(server), h_iocp, 9999, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	EX_OVERLAPPED a_over;
	a_over._op_type = OP_ACCEPT;
	AcceptEx(server, c_socket, a_over._send_buf, 0, addr_size + 16, addr_size + 16, 0, &a_over._over);

	while (true) {
		DWORD num_bytes;
		ULONG_PTR key;
		WSAOVERLAPPED* over = nullptr;
		BOOL ret = GetQueuedCompletionStatus(h_iocp, &num_bytes, &key, &over, INFINITE);
		EX_OVERLAPPED* ex_over = reinterpret_cast<EX_OVERLAPPED*>(over);
		if (FALSE == ret) {
			if (ex_over->_op_type == OP_ACCEPT) {
				cout << "Accept Error";
				exit(-1);
			}
			else {
				cout << "GQCS Error on client[" << key << "]\n";
				disconnect(static_cast<int>(key));
				if (ex_over->_op_type == OP_SEND) delete ex_over;
				continue;
			}
		}

		switch (ex_over->_op_type) {
		case OP_ACCEPT: {
			int client_id = get_new_client_id();
			if (client_id != -1) {
				clients[client_id].b_use = true;
				switch (client_id % 4) {
				case 0:
					clients[client_id].m_player.SetWorldLocation(8000.f, 8000.f, 1000.f);
					break;
				case 1:
					clients[client_id].m_player.SetWorldLocation(-8000.f, -8000.f, 1000.f);
					break;
				case 2:
					clients[client_id].m_player.SetWorldLocation(8000.f, -8000.f, 1000.f);
					break;
				case 3:
					clients[client_id].m_player.SetWorldLocation(-8000.f, 8000.f, 1000.f);
					break;
				}
				
				clients[client_id].m_player.m_id = client_id;
				clients[client_id].m_prev_remain = 0;
				clients[client_id].m_socket = c_socket;
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket),
					h_iocp, client_id, 0);
				clients[client_id].do_recv();
			}
			else {
				cout << "Max user exceeded.\n";
				closesocket(c_socket);
			}
			c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			ZeroMemory(&a_over._over, sizeof(a_over._over));
			AcceptEx(server, c_socket, a_over._send_buf, 0, addr_size + 16, addr_size + 16, 0, &a_over._over);
			break;
		}
		case OP_RECV: {
			int remain_data = num_bytes + clients[key].m_prev_remain;
			char* p = ex_over->_send_buf;
			while (remain_data > 0) {
				int packet_size = p[0];
				if (packet_size <= remain_data) {
					process_packet(static_cast<int>(key), p);
					p = p + packet_size;
					remain_data = remain_data - packet_size;
				}
				else break;
			}
			clients[key].m_prev_remain = remain_data;
			if (remain_data > 0)
				memcpy(ex_over->_send_buf, p, remain_data);
			clients[key].do_recv();
			break;
		}
		case OP_SEND:
			if (ex_over->_packet_type == SC_LOGIN_INFO) {
				std::cout << "GQCS Login Send" << std::endl;
			}
				
			else if (ex_over->_packet_type == SC_ADD_PLAYER) {
				std::cout << "GQCS Add Send" << std::endl;
			}
				
			/*else if (ex_over->_packet_type == SC_MOVE_PLAYER)
				std::cout << "GQCS Move Send" << std::endl;*/
			
			delete ex_over;
			break;
		}
	}
	closesocket(server);
	WSACleanup();
}
