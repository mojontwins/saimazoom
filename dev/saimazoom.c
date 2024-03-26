// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Program skeleton. 

// CPC Memory map
// 0100 tilemap
// 0400 ejecutable
// 8800 descomprimir canción
// 9000 buffer
// C000 pantalla (a trozos)
// C600 room buffers
// CE00 dirty cells (tiles_tocados)
// D600 arrays
// DF80 buffers WYZ
// E600 sprite structures
// FE00 LUT

#include <cpcrslib.h>

#define VIEWPORT_X      0
#define VIEWPORT_Y      0

// We are using some stuff from CPCRSLIB directly
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

#define BASE_TILEMAP 		0x0100
#define WYZ_SONG_BUFFER 	0x8800
#define BASE_ROOM_BUFFERS	0xC000 + 0x600
#define BASE_DIRTY_CELLS 	0xC800 + 0x600
#define BASE_ARRAYS 		0xD000 + 0x600
#define BASE_WYZ 			0xDF80
#define BASE_SPRITES 		0xE000 + 0x600
#define BASE_LUT			0xF800 + 0x600

#define BASE_SUPERBUFF  	0x9000

#define SW_SPRITES_ALL 		2 				// 1 sprite for the player, 1 sprite for the enemy
#define SP_PLAYER 			0
#define SP_ENEM 			1

#define BLACK_PEN			0

#define WYZ_FX_CHANNEL      1

// PIXEL 0  PIXEL 1
// 3 2 1 0  3 2 1 0
// 1 5 3 7  0 4 2 6
#define BLACK_COLOUR_BYTE ((BLACK_PEN>>3)&1)|(((BLACK_PEN>>3)&1)<<1)\
							|(((BLACK_PEN>>1)&1)<<2)|(((BLACK_PEN>>1)&1)<<3)\
							|(((BLACK_PEN>>2)&1)<<4)|(((BLACK_PEN>>2)&1)<<5)\
							|((BLACK_PEN&1)<<6)|((BLACK_PEN&1)<<7)

#include "definitions.h"
#include "assets/pal.h"
#include "assets/ss.h"
#include "assets/ssmaps.h"
#include "assets/ts.h"
#include "assets/map.h"

#include "zx0.h"

#include "system.h"
#include "printer.h"
#include "update.h"

#include "wyz/efectos.h"
#include "wyz/instrumentos.h"
#include "wyz/songs.h"
#include "wyz/wyz_player.h"

#include "init.h"

#include "main.h"
