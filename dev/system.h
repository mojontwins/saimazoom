// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// System & backend routines

unsigned char gpit;

// Memory map

#define BASE_TILEMAP 		0x0100
#define WYZ_SONG_BUFFER 	0x8800
#define BASE_ROOM_BUFFERS	0xC000 + 0x600
#define BASE_DIRTY_CELLS 	0xC800 + 0x600
#define BASE_ARRAYS 		0xD000 + 0x600
#define BASE_WYZ 			0xDF80
#define BASE_SPRITES 		0xE000 + 0x600
#define BASE_LUT			0xF800 + 0x600

#define BASE_SUPERBUFF  	0x9000

// CPCRSLIB stuff

#define VIEWPORT_X      0
#define VIEWPORT_Y      0

#asm
	// Imports

		XREF _nametable
		XREF tabla_teclas
		LIB cpc_KeysData
		LIB cpc_UpdTileTable
		LIB cpc_InvalidateRect
		LIB cpc_TestKeyboard

	// Exports

		XDEF viewport_x
		XDEF viewport_y

		defc viewport_x = VIEWPORT_X
		defc viewport_y = VIEWPORT_Y
#endasm

// PIXEL 0  PIXEL 1
// 3 2 1 0  3 2 1 0
// 1 5 3 7  0 4 2 6
#define BLACK_COLOUR_BYTE ((BLACK_PEN>>3)&1)|(((BLACK_PEN>>3)&1)<<1)\
							|(((BLACK_PEN>>1)&1)<<2)|(((BLACK_PEN>>1)&1)<<3)\
							|(((BLACK_PEN>>2)&1)<<4)|(((BLACK_PEN>>2)&1)<<5)\
							|((BLACK_PEN&1)<<6)|((BLACK_PEN&1)<<7)


// Sprite engine

#define SW_SPRITES_ALL 		2 				// 1 sprite for the player, 1 sprite for the enemy
#define SP_PLAYER 			0
#define SP_ENEM 			1

#define BLACK_PEN			0

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

// Sound engine

#define WYZ_FX_CHANNEL      1

unsigned char isr_player_on;
unsigned char pattern_line_ct;

// Keyboard

/*
=========================================================================================================
|     |                                       column                                                    |
|     |-------------------------------------------------------------------------------------------------|
| row |     40      |     41     |  42   | 43  | 44  | 45   |     46       | 47  |   48     |    49     |
|=====|=============|============|=======|=====|=====|======|==============|=====|==========|===========|
| 80  | f.          | f0         | Ctrl  | > , | < . | Space| V            | X   | Z        | Del       |
| 40  | Enter       | f2         | ` \   | ? / | M   | N    | B            | C   | Caps Lock| Unused    |
| 20  | f3          | f1         | Shift | * : | K   | J    | F Joy1_Fire1 | D   | A        | Joy0_Fire1|
| 10  | f6          | f5         | f4    | + ; | L   | H    | G Joy1_Fire2 | S   | Tab      | Joy0_Fire2|
| 08  | f9          | f8         | } ]   | P   | I   | Y    | T Joy1_Right | W   | Q        | Joy0_Right|
| 04  | Cursor Down | f7         | Return| | @ | O   | U    | R Joy1_Left  | E   | Esc      | Joy0_Left |
| 02  | Cursor Right| Copy       | { [   | = - | ) 9 | ' 7  | % 5 Joy1_Down| # 3 | " 2      | Joy0_Down |
| 01  | Cursor Up   | Cursor Left| Clr   | £ ^ | _ 0 | ( 8  | & 6 Joy1_Up  | $ 4 | ! 1      | Joy0_Up   |
=========================================================================================================
*/

extern unsigned char def_keys_kbd [0];
#asm
	._def_keys_kbd
		defw $4404 		; LEFT     O
		defw $4308 		; RIGHT    P
		defw $4808 		; UP       Q
		defw $4820 		; DOWN     A

		defw $4580 		; BUTTON_A SPACE
		defw $4808 		; BUTTON_B Q

		defw $4204		; KEY_ENTER
		defw $4804		; KEY_ESC	

		defw $4880		; KEY_AUX1 Z
		defw $4780 		; KEY_AUX2 X
		defw $4801 		; KEY_AUX3 1
		defw $4802 		; KEY_AUX4 2
#endasm

extern unsigned char def_keys_joy [0];
#asm
	._def_keys_joy
		defw 0x4904, 0x4908, 0x4901, 0x4902, 0x4910, 0x4920
		defw 0x4004, 0x4804, 0x4880, 0x4780, 0x4801, 0x4802
#endasm

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

// Random LUT

unsigned char rand_lut [] = { 
	55, 123, 218, 132, 130, 19, 73, 70, 88, 91, 3, 51, 181, 215, 66, 222,
	165, 110, 191, 135, 148, 173, 131, 159, 34, 203, 126, 95, 122, 111, 27, 255, 
	241, 10, 13, 220, 32, 236, 230, 0, 28, 24, 81, 12, 41, 76, 228, 46, 
	133, 176, 163, 185, 57, 83, 154, 30, 42, 221, 144, 197, 40, 199, 237, 153, 
	97, 245, 171, 100, 198, 187, 89, 156, 15, 17, 94, 56, 184, 48, 248, 253, 
	31, 64, 93, 106, 239, 18, 49, 189, 117, 63, 233, 249, 22, 149, 232, 143, 
	59, 113, 26, 193, 90, 7, 139, 164, 214, 67, 9, 235, 243, 1, 35, 247, 
	82, 157, 251, 208, 8, 47, 60, 80, 61, 244, 227, 223, 98, 39, 5, 99, 178, 
	192, 25, 252, 65, 175, 37, 78, 74, 186, 179, 114, 87, 119, 124, 14, 242, 
	172, 84, 142, 77, 180, 158, 182, 177, 75, 254, 58, 53, 238, 209, 168, 125, 
	204, 234, 141, 127, 206, 118, 169, 23, 188, 226, 201, 128, 79, 229, 137, 101, 
	134, 140, 103, 54, 225, 68, 162, 170, 194, 96, 190, 20, 86, 69, 29, 202, 
	216, 161, 212, 150, 211, 52, 115, 116, 44, 102, 250, 62, 71, 240, 92, 183, 
	219, 152, 151, 195, 145, 85, 166, 72, 2, 129, 108, 6, 200, 224, 16, 174,
	196, 246, 147, 36, 167, 33, 231, 11, 21, 105, 138, 155, 136, 4, 213, 45, 
	43, 210, 217, 109, 121, 160, 120, 107, 38, 104, 50, 205, 207, 112, 146 
};

unsigned char rand_lut_index; 

// System related funtions

void blackout (void) {
	gpit = BLACK_COLOUR_BYTE;
	#asm
			ld  a, 0xc0
		.bo_l1
			ld  h, a
			ld  l, 0
			ld  b, a
			ld  a, (_gpit)
			ld  (hl), a
			ld  a, b
			ld  d, a
			ld  e, 1
			ld  bc, 0x5ff
			ldir

			add 8
			jr  nz, bo_l1
	#endasm
}

void __FASTCALL__ cpc_Border (unsigned char b) {
	#asm
			ld 	a, l
			ld  bc, 0x7F11
			out (c), c
			out (c), a
	#endasm
}

void pal_set (unsigned char *pal) {
	#ifdef MODE_1
		gpit = 4;
	#else
		gpit = 16;
	#endif
	while (gpit --) cpc_SetColour (gpit, pal[gpit]);
}

void invalidate_viewport (void) {
	#asm
			ld  B, VIEWPORT_Y
			ld  C, VIEWPORT_X
			ld  D, VIEWPORT_Y+20
			ld  E, VIEWPORT_X+23
			call cpc_InvalidateRect
	#endasm
}

unsigned char rand (void) {
	return rand_lut [rand_lut_index ++];
}
