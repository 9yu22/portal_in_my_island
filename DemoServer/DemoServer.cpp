// DemoServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#include "Common.h"
#include "DemoServer.h"

#include <iostream>
#include <array>
#include <vector>
#include <algorithm>

std::array<blockInfo, 3> blockList;
std::vector<blockInfo> gameBlocks{ 10 };

void InitBlockList()
{
    blockList[0].blockType = WOOD;
    blockList[0].blockType = NONE;
    blockList[0].x = 0;
    blockList[0].y = 0;
    blockList[0].z = 0;

    blockList[1].blockType = ROCK;
    blockList[1].blockType = NONE;
    blockList[1].x = 0;
    blockList[1].y = 0;
    blockList[1].z = 0;

    blockList[2].blockType = IRON;
    blockList[2].blockType = NONE;
    blockList[2].x = 0;
    blockList[2].y = 0;
    blockList[2].z = 0;
}

void SendBlockList(SOCKET client_sock)
{
    /*for (int i = 0; i < blockList.size(); i++)
    {
        int retval = send(client_sock, (char*)&blockList[i], sizeof(blockList[0]), 0);
        if (retval == SOCKET_ERROR) err_display("recv()");
    }*/

    int retval = send(client_sock, (char*)&blockList, sizeof(blockList), 0);
    if (retval == SOCKET_ERROR) err_display("recv()");
}

void RecvPacket(SOCKET client_sock) {

    packetHeader header;

    int retval = recv(client_sock, (char*)&header, sizeof(header), 0);
    if (retval == SOCKET_ERROR) {
        err_display("recv()");
        return;
    }

    blockInfo block;

    switch (header.packetType) {
    case ADD_BLOCK:
        retval = recv(client_sock, ((char*)&block) + sizeof(header), sizeof(block) - sizeof(header), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        AddGameBlock(block);
        SendGameBlocks(client_sock);
        break;

    case REMOVE_BLOCK:
        retval = recv(client_sock, ((char*)&block) + sizeof(header), sizeof(block) - sizeof(header), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        RemoveGameBlock(block);
        SendGameBlocks(client_sock);
        break;

    default:
        break;
    }
}

// 일단 블록 위치는 자율이 아닌 일정 간격으로 정해져 있으므로 좌표가 같으면 충돌로 판정
void AddGameBlock(const blockInfo block) 
{
    for (int i = 0; i < gameBlocks.size(); ++i){
        if (block.x == gameBlocks[i].x && block.y == gameBlocks[i].y && block.z == gameBlocks[i].z)
            return;
    }
    gameBlocks.emplace_back(block);
}

void RemoveGameBlock(const blockInfo block) 
{
    auto newEnd = std::remove_if(gameBlocks.begin(), gameBlocks.end(),
        [&block](const blockInfo& blocks) {// 현재 반복자에 대한 요소
            return block.x == blocks.x && block.y == blocks.y && block.z == blocks.z;
        }
    );

    gameBlocks.erase(newEnd, gameBlocks.end());
}

void SendGameBlocks(SOCKET client_sock)
{
    // 전체 블록 수 전송
    int blocksNum = gameBlocks.size();
    int retval = send(client_sock, (char*)&blocksNum, sizeof(blocksNum), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return;
    }

    // 각 블록 정보 전송
    for (int i = 0; i < gameBlocks.size(); ++i)
    {
        gameBlocks[i].packetType = ADD_BLOCK; // 이건 클라이언트쪽 코드 작성하면 맞게 바꿔줘야 함
        gameBlocks[i].packetSize = sizeof(blockInfo);

        retval = send(client_sock, (char*)&gameBlocks[i], sizeof(blockInfo), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
    }
}

DWORD WINAPI GameThread(LPVOID arg)
{
    char addr[INET_ADDRSTRLEN];
    int addrlen;

    SOCKET client_sock = (SOCKET)arg;
    struct sockaddr_in clientaddr;

    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
    inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

    InitBlockList();
    SendBlockList(client_sock);
    while (true) {       
        RecvPacket(client_sock);
    }

    closesocket(client_sock);
}

int main(int argc, char* argv[])
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 소켓 생성
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    struct sockaddr_in clientaddr;
    int addrlen = sizeof(clientaddr);
    HANDLE hThread;

    while (1) {
        // accept()
        SOCKET client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            return 1;
        }

        hThread = CreateThread(NULL, 0, GameThread, (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }

    closesocket(listen_sock);
    WSACleanup();

    return 0;
}

