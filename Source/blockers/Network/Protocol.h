#pragma once

#define BUFSIZE = 256;

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

constexpr char CS_DISCONNECT = 50;

enum class Anim : int8 { IDLE, MOVE, JUMP };

#pragma pack (push, 1)
struct CS_LOGIN_PACKET {
    uint8 size;
    int8 type;
    //char name[NAME_SIZE];
};

struct CS_MOVE_PACKET {
    uint8 size;
    int8 type;
    float x, y, z;
    float pitch, yaw, roll;
};

struct SC_LOGIN_INFO_PACKET {
    uint8 size;
    int8 type;
    int8 id;
    float x, y, z;
};

struct SC_ADD_PLAYER_PACKET {
    uint8 size;
    int8 type;
    int8 id;
    float x, y, z;
    //char	name[NAME_SIZE];
};

struct SC_REMOVE_PLAYER_PACKET {
    uint8 size;
    int8 type;
    int8 id;
};

struct SC_MOVE_PLAYER_PACKET {
    uint8 size;
    int8 type;
    int8 id;
    float x, y, z;
    float pitch, yaw, roll;
};

struct SC_ADD_BLOCK_PACKET {
    uint8 size;
    int8 type;
    int8 chunk_index;
    int32 ix, iy, iz;
    int8 blocktype;
};

struct CS_ADD_BLOCK_PACKET {
    uint8 size;
    int8 type;
    int8 chunk_index;
    int32 ix, iy, iz;
    int8 blocktype;
};

struct SC_REMOVE_BLOCK_PACKET {
    uint8 size;
    int8 type;
    int8 chunk_index;
    int32 ix, iy, iz;
    float wx, wy, wz; // 히트된 부분의 월드 좌표 벡터
    float nx, ny, nz; // 히트된 부분의 노멀 벡터
    int8 blocktype;
};

struct CS_REMOVE_BLOCK_PACKET {
    uint8 size;
    int8 type;
    int8 chunk_index;
    int32 ix, iy, iz;
    float wx, wy, wz; // 히트된 부분의 월드 좌표 벡터
    float nx, ny, nz; // 히트된 부분의 노멀 벡터
    int8 blocktype;
};

struct ANIM_PACKET {
    uint8 size;
    int8 type;
    int8 id;
    int8 anim_type;
};

struct CS_CHANGE_HP_PACKET {
    uint8 size;
    int8 type;
    //int8 id;
    int8 hit_id;
};

struct SC_CHANGE_HP_PACKET {
    uint8 size;
    int8 type;
    int8 id;
    int hp;
};

struct CS_DISCONNECT_PACKET {
    uint8 size;
    char type;
    char id;
};


#pragma pack (pop)