#pragma once

constexpr int BUFFER_SIZE = 4096;

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

constexpr char SC_CHANGE_PLAYER_HP = 11;
constexpr char CS_CHANGE_PLAYER_HP = 12;

constexpr char SC_ADD_PORTAL = 13;
constexpr char SC_CHANGE_PORTAL_HP = 14;
constexpr char CS_CHANGE_PORTAL_HP = 15;

constexpr char SC_ADD_ITEM = 16;
constexpr char SC_REMOVE_ITEM = 17;
constexpr char CS_REMOVE_ITEM = 18;

constexpr char SC_RESPAWN = 19;

constexpr char SC_DESTROY_PORTAL = 20;

constexpr char SC_REMOVE_PLAYER = 21;

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
    int id;
    float x, y, z;
};

struct SC_ADD_PLAYER_PACKET {
    uint8 size;
    int8 type;
    int id;
    float x, y, z;
};

struct SC_ADD_PORTAL_PACKET {
    uint8 size;
    int8 type;
    int id;
    float hp;
    float x, y, z;
};

struct SC_REMOVE_PLAYER_PACKET {
    uint8 size;
    int8 type;
    int id;
};

struct SC_MOVE_PLAYER_PACKET {
    uint8 size;
    int8 type;
    int id;
    float x, y, z;
    float pitch, yaw, roll;
};

struct SC_ADD_BLOCK_PACKET {
    uint8 size;
    int8 type;
    int chunk_index;
    int ix, iy, iz;
    int8 blocktype;
};

struct CS_ADD_BLOCK_PACKET {
    uint8 size;
    int8 type;
    int chunk_index;
    int ix, iy, iz;
    int8 blocktype;
};

struct SC_REMOVE_BLOCK_PACKET {
    uint8 size;
    int8 type;
    int chunk_index;
    int ix, iy, iz;
    float wx, wy, wz; // 히트된 부분의 월드 좌표 벡터
    float nx, ny, nz; // 히트된 부분의 노멀 벡터
    int8 blocktype;
};

struct CS_REMOVE_BLOCK_PACKET {
    uint8 size;
    int8 type;
    int chunk_index;
    int ix, iy, iz;
    float wx, wy, wz; // 히트된 부분의 월드 좌표 벡터
    float nx, ny, nz; // 히트된 부분의 노멀 벡터
    int8 blocktype;
};

struct ANIM_PACKET {
    uint8 size;
    int8 type;
    int id;
    int8 anim_type;
};

struct CS_CHANGE_PLAYER_HP_PACKET {
    uint8 size;
    int8 type;
    //int8 id;
    int hit_id;
};

struct SC_CHANGE_PLAYER_HP_PACKET {
    uint8 size;
    int8 type;
    int id;
    float hp;
};

struct CS_CHANGE_PORTAL_HP_PACKET {
    uint8 size;
    int8 type;
    //int8 id;
    int hit_id;
};

struct SC_CHANGE_PORTAL_HP_PACKET {
    uint8 size;
    int8 type;
    int id;
    float hp;
};

struct CS_DISCONNECT_PACKET {
    uint8 size;
    int8 type;
    int id;
};

struct SC_ADD_ITEM_PACKET {
    uint8 size;
    int8 type;
    int id;
    float x, y, z;
    char item_type;
};

struct SC_REMOVE_ITEM_PACKET {
    uint8 size;
    int8 type;
    int id;
};

struct CS_REMOVE_ITEM_PACKET {
    uint8 size;
    int8 type;
    int id;
};

struct SC_RESPAWN_PACKET {
    uint8 size;
    int8 type;
    int id;
    float hp;
    float x, y, z;
};

struct SC_PORTAL_DESTROY_PACKET {
    uint8 size;
    int8 type;
    int id;
};

struct SC_REMOVE_PLAYER_PACKET {
    uint8 size;
    int8 type;
    int id;
};

#pragma pack (pop)