// MTE MK1 (la Churrera) v5.10
// Copyleft 2010-2014, 2020-2022 by the Mojon Twins

// Spriteset mappings. 
// One entry per sprite face in the spriteset!

#define SWsprites_ALL  16

unsigned char sm_cox [] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
};

unsigned char sm_coy [] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
};

void *sm_invfunc [] = {
	cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, 
	cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, 
	cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, 
	cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, 
	cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA, cpc_PutSpTileMap12x24CA
};

void *sm_updfunc [] = {
	cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, 
	cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, 
	cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, 
	cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, 
	cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA, cpc_PutTrSp12x24TileMap2bGCA
};

extern void *sm_sprptr [0];
#asm
	._sm_sprptr
		defw _sprites + 0x0000, _sprites + 0x0090, _sprites + 0x0120, _sprites + 0x01B0
		defw _sprites + 0x0240, _sprites + 0x02D0, _sprites + 0x0360, _sprites + 0x03F0
		defw _sprites + 0x0480, _sprites + 0x0510, _sprites + 0x05A0, _sprites + 0x0630
		defw _sprites + 0x06C0, _sprites + 0x0750, _sprites + 0x07E0, _sprites + 0x0870
		defw _sprites + 0x0900, _sprites + 0x0990, _sprites + 0x0A20, _sprites + 0x0AB0
#endasm

// A list of MK1v4-friendly macros
#define SPRITE_00 (_sprites + 0x0000)
#define SPRITE_01 (_sprites + 0x0090)
#define SPRITE_02 (_sprites + 0x0120)
#define SPRITE_03 (_sprites + 0x01B0)
#define SPRITE_04 (_sprites + 0x0240)
#define SPRITE_05 (_sprites + 0x02D0)
#define SPRITE_06 (_sprites + 0x0360)
#define SPRITE_07 (_sprites + 0x03F0)
#define SPRITE_08 (_sprites + 0x0480)
#define SPRITE_09 (_sprites + 0x0510)
#define SPRITE_0A (_sprites + 0x05A0)
#define SPRITE_0B (_sprites + 0x0630)
#define SPRITE_0C (_sprites + 0x06C0)
#define SPRITE_0D (_sprites + 0x0750)
#define SPRITE_0E (_sprites + 0x07E0)
#define SPRITE_0F (_sprites + 0x0870)
#define SPRITE_10 (_sprites + 0x0900)
#define SPRITE_11 (_sprites + 0x0990)
#define SPRITE_12 (_sprites + 0x0A20)
#define SPRITE_13 (_sprites + 0x0AB0)

