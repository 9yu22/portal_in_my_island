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

#pragma pack (push, 1)

//struct UPacketHeader {
//    int8 size;
//    int8 type;
//};

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

//struct LocationPacket {
//    uint8 size;
//    int8 type;
//    float x, y, z;
//};

struct TestPacket {
    u_char size;
    char type;
    short number;
};
#pragma pack (pop)