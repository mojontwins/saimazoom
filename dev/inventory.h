// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Cyclic inventory routines

unsigned char inventory[4];

unsigned char init_inventory (void) {
	#asm
			ld  a, OBJ_EMPTY
			ld  (_inventory), a
			ld  (_inventory + 1), a
			ld  (_inventory + 2), a
			ld  (_inventory + 3), a
	#endasm
}

unsigned char inventory_shift (unsigned char entering_object) {
	// entering_object will go to inventory [0], inventory [0] to [3] will be shifted forwards,
	// inventory [3] will be returned.

	rda = inventory [3];
	for (gpit = 3; gpit > 0; gpit --) inventory [gpit] = inventory [gpit - 1];
	inventory [0] = entering_object;

	return rda;
}

unsigned char has_item (unsigned char item) {
	return inventory [0] == item || inventory [1] == item || inventory [2] == item || inventory [3] == item;
}

unsigned char draw_inventory () {
	#asm
			ld  a, (_inventory)
			ld  d, a
			ld  bc, 0x0E19  // B = 15, C = 25
			call draw_tile_direct2

			ld  a, (_inventory + 1)
			ld  d, a
			ld  bc, 0x0E1C  // B = 15, C = 28
			call draw_tile_direct2			

			ld  a, (_inventory + 2)
			ld  d, a
			ld  bc, 0x1119  // B = 18, C = 25
			call draw_tile_direct2			

			ld  a, (_inventory + 3)
			ld  d, a
			ld  bc, 0x111C  // B = 18, C = 28
			call draw_tile_direct2

			ld  bc, 0x0E19  // B = 15, C = 25
			ld  de, 0x131E 	// B = 20, C = 30
			call cpc_InvalidateRect
	#endasm
}

unsigned char has_object (unsigned char o) {
	for (gpit = 0; gpit < 4; gpit ++) if (inventory [gpit] == o) return gpit;
	return 0xff;
}
