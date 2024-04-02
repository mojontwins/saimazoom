// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Tileset and tilemaps

extern unsigned char tileset [0];
extern unsigned char tsmaps [0];

#asm
		XDEF _ts
		XDEF tiles
	._tileset
	.tiles
		BINARY "../bin/numbers.bin" 	// 10 patterns
		BINARY "../bin/ts.bin" 

	._tmaps
		BINARY "../bin/tsmaps.bin" 		// tsmaps start @ pattern 10, of course.
#endasm


