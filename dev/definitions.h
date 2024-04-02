// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Globals & constants


#define DIRECTION_UP       0
#define DIRECTION_RIGHT    1
#define DIRECTION_DOWN     2
#define DIRECTION_LEFT     3

#define STATE_OFF          0
#define STATE_APPEARING    1
#define STATE_IDLE         2
#define STATE_WALK         3
#define STATE_KILLED       4

#define ITEM_EMPTY         0
#define ITEM_CAFE          1
#define ITEM_PISTOL        2
#define ITEM_KEY           3
#define ITEM_BOAT          4
#define ITEM_PICKAXE       5
#define ITEM_KNIFE         6
#define ITEM_WATER         7

#define OBJ_SACK           20
#define OBJ_PISTOL         21
#define OBJ_KEY            22
#define OBJ_BOAT           23
#define OBJ_PICKAXE        24
#define OBJ_SABER          25
#define OBJ_WATER          26
#define OBJ_EMPTY          27

#define MAX_SACKS 			4
#define MAX_KEYS            2
#define MAX_WATERS         16
#define MAX_BOATS           4
#define MAX_SABERS          6
#define MAX_PICKAXES        4
#define MAX_PISTOLS         6

#define MAX_OBJECTS        MAX_SACKS + MAX_KEYS + MAX_WATERS + MAX_BOATS + MAX_SABERS + MAX_PICKAXES + MAX_PISTOLS
#define MAX_PERSISTENT     MAX_SABERS + MAX_PICKAXES

signed char dx [] = { 0, 1, 0, -1 };
signed char dy [] = { -1, 0, 1, 0 };

// Player

unsigned char px, py;
unsigned char ptx, pty;
signed char pmx, pmy;
unsigned char pstate, pct;
unsigned char pdirection;
unsigned char pstep;
unsigned char pfacing;
unsigned char pcooldown;

// Enem

unsigned char ex, ey;
unsigned char etx, ety;
signed char emx, emy;
unsigned char estate, ect;
unsigned char edirection;
unsigned char eattempts;
unsigned char estep;
unsigned char etype;

// Objects

unsigned char object_n_pant [MAX_OBJECTS];
unsigned char object_yx [MAX_OBJECTS];
unsigned char object_id [MAX_OBJECTS];
unsigned char oid;

// Tile persistence (tiles cleared must stay cleared)

unsigned char persistence_n_pant [MAX_PERSISTENT];
unsigned char persistence_yx [MAX_PERSISTENT];
unsigned char pid;

// general

unsigned char map_attr [56] @ BASE_ROOM_BUFFERS;

unsigned char n_pant, o_pant;
unsigned char playing;
unsigned char _x, _y, _t, rda, rdb, rdc, rdm;
unsigned char *gp_gen;
unsigned char flip_flop;

unsigned char n_time  [] = { 0, 0, 0, 0 };
unsigned char n_water [] = { 0, 0, 0, 0 };

// 8 obstacle
// 1 river
// 2 plant
// 4 rock
// 16 lock

unsigned char tile_behs [] = {
	 0, 10,  8,  8, 24,  8,  9,  9,  9,  9,
	 9,  9, 10, 10, 10, 10,  8, 12,  8, 10
};
