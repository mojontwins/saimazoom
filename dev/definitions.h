// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Globals & constants

#define KEY_LEFT 		0
#define KEY_RIGHT		1
#define KEY_UP  		2
#define KEY_DOWN 		3
#define KEY_BUTTON_A	4
#define KEY_BUTTON_B	5
#define KEY_ENTER		6
#define KEY_ESC			7
#define KEY_AUX1		8
#define KEY_AUX2		9
#define KEY_AUX3 		10
#define KEY_AUX4 		11

typedef struct sprite {
	unsigned int sp0;			// 0
	unsigned int sp1; 			// 2
	unsigned int coord0;
	signed char cox, coy;		// 6 7
	unsigned char cx, cy; 		// 8 9
	unsigned char ox, oy;		// 10 11
	void *invfunc;				// 12
	void *updfunc;				// 14
} SPR;

SPR sp_sw [SW_SPRITES_ALL] 					@ BASE_SPRITES;
unsigned char *spr_next [SW_SPRITES_ALL] 	@ BASE_SPRITES + (SW_SPRITES_ALL)*16;
unsigned char spr_on [SW_SPRITES_ALL]		@ BASE_SPRITES + (SW_SPRITES_ALL)*18;
unsigned char spr_x [SW_SPRITES_ALL]		@ BASE_SPRITES + (SW_SPRITES_ALL)*19;
unsigned char spr_y [SW_SPRITES_ALL]		@ BASE_SPRITES + (SW_SPRITES_ALL)*20;

unsigned char isr_player_on;
unsigned char pattern_line_ct;

// 

#define DIRECTION_UP       0
#define DIRECTION_RIGHT    1
#define DIRECTION_DOWN     2
#define DIRECTION_LEFT     3

#define STATE_IDLE         0
#define STATE_WALK         1
#define STATE_OFF          2

#define ITEM_EMPTY         0
#define ITEM_CAFE          1
#define ITEM_PISTOL        2
#define ITEM_KEY           3
#define ITEM_BOAT          4
#define ITEM_PICKAXE       5
#define ITEM_KNIFE         6
#define ITEM_WATER         7

signed char dx [] = { 0, 1, 0, -1 };
signed char dy [] = { -1, 0, 1, 0 };

// Player

unsigned char px, py;
unsigned char ptx, pty;
signed char pmx, pmy;
unsigned char pstate, pct;

unsigned char pdirection;

// Enem

unsigned char ex, ey;
unsigned char etx, ety;
unsigned char emx, emy;
unsigned char estate, ect;

unsigned char edirection;

unsigned char eattempts;

// general

unsigned char map_attr [56] @ BASE_ROOM_BUFFERS;

unsigned char n_pant;
unsigned char playing;
unsigned char _x, _y, _t, rda, rdb, rdc, gpit;

// 8 obstacle
// 1 river
// 2 plant
// 4 rock
// 16 key

unsigned char tile_behs [] = {
	10, 8, 8, 24, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 8, 8, 10, 8, 12, 8
};
