#pragma once

constexpr int PORT_NUM = 4040;
constexpr int BUF_SIZE = 256; 
constexpr int NAME_SIZE = 20;

constexpr char TEST = 100;
constexpr char LOCATION = 90;

constexpr char CS_LOGIN = 0;
constexpr char CS_MOVE = 1;

constexpr char SC_LOGIN_INFO = 2;
constexpr char SC_ADD_PLAYER = 3;
constexpr char SC_REMOVE_PLAYER = 4;
constexpr char SC_MOVE_PLAYER = 5;
constexpr char SC_ADD_BLOCK = 6;
constexpr char CS_ADD_BLOCK = 7;
constexpr char ANIM = 8;
constexpr char SC_REMOVE_BLOCK = 9;
constexpr char CS_REMOVE_BLOCK = 10;

constexpr char SC_CHANGE_HP = 11;
constexpr char CS_CHANGE_HP = 12;

enum OP_TYPE { OP_ACCEPT, OP_RECV, OP_SEND };

enum class Anim : char { IDLE, MOVE, JUMP };

enum class BKEBlock : char
{
    Null, Air, Stone, Dirt, Grass, Amethyst, Stair
};

typedef unsigned char u_char;

#pragma pack (push, 1)
struct CS_LOGIN_PACKET {
    u_char size;
    char type;
    //char name[NAME_SIZE];
};

struct CS_MOVE_PACKET {
    u_char size;
    char type;
    float x, y, z;
    float pitch, yaw, roll;
};

struct SC_LOGIN_INFO_PACKET {
    u_char size;
    char type;
    char id;
    float x, y, z;
};

struct SC_ADD_PLAYER_PACKET {
    u_char size;
    char type;
    char id;
    float x, y, z;
    //char	name[NAME_SIZE];
};

struct SC_REMOVE_PLAYER_PACKET {
    u_char size;
    char type;
    char id;
};

struct SC_MOVE_PLAYER_PACKET {
    u_char size;
    char type;
    char id;
    float x, y, z;
    float pitch, yaw, roll;
};

struct SC_ADD_BLOCK_PACKET {
    u_char size;
    char type;
    char chunk_index;
    int ix, iy, iz;
    char blocktype;
};

struct CS_ADD_BLOCK_PACKET {
    u_char size;
    char type;
    char chunk_index;
    int ix, iy, iz;
    char blocktype;
};

struct SC_REMOVE_BLOCK_PACKET {
    u_char size;
    char type;
    char chunk_index;
    int ix, iy, iz;
    float wx, wy, wz; 
    float nx, ny, nz; 
    char blocktype;
};

struct CS_REMOVE_BLOCK_PACKET {
    u_char size;
    char type;
    char chunk_index;
    int ix, iy, iz;
    float wx, wy, wz;
    float nx, ny, nz; 
    char blocktype;
};

struct ANIM_PACKET {
    u_char size;
    char type;
    char id;
    char anim_type;
};

struct CS_CHANGE_HP_PACKET {
    u_char size;
    char type;
    //int8 id;
    char hit_player_id;
};

struct SC_CHANGE_HP_PACKET {
    u_char size;
    char type;
    char id;
    int hp;
};

#pragma pack (pop)