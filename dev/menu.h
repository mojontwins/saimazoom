// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Shows a title screen and selects keyboard / joystick

void menu (void) {
	
	// Decompress frame
	#asm
			ld  hl, s_title
			ld  de, BASE_SUPERBUFF
			call dzx0_standard
	#endasm
	cpc_ShowTileMap (1);

	while (1) {
		// The best randomizer! (and the cheapest)
		rand_lut_index ++;

		if (cpc_TestKey (KEY_AUX3)) { gp_gen = def_keys_kbd; break; }
		if (cpc_TestKey (KEY_AUX4)) { gp_gen = def_keys_joy; break; }
	}

	

	// Copy keys to extern 
	#asm
		._copy_keys_to_extern
			ld  hl, (_gp_gen)
			ld  de, cpc_KeysData + 12
			ld  bc, 24
			ldir
	#endasm
}
