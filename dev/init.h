// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Hardware and data inits, ISR and all that boring shit!

void init (void) {
	// CPC initialization
	
	wyz_init ();

	#asm
		di

		ld  hl, 0xC000
		xor a
		ld  (hl), a
		ld  de, 0xC001
		ld  bc, 0x3DFF
		ldir

		ld  a, 195
		ld  (0x38), a
		ld  hl, _isr
		ld  (0x39), hl
		jp  isr_done

	._isr
		push af 
		
		ld  a, (isr_c1)
		inc a
		cp  6
		jr  c, _skip_ay_player

		ld  a, (isr_c2)
		inc a
		ld  (isr_c2), a

		ld  a, (_isr_player_on)
		or  a
		jr  z, _skip_ay_player

		push hl
		push de
		push bc
		push ix
		push iy

		call WYZ_PLAYER_ISR

		pop iy
		pop ix
		pop bc
		pop de 
		pop hl

		xor a

	._skip_ay_player 
		ld  (isr_c1), a	
		
		pop af
		ei
		ret

	.isr_c1 
		defb 0
	.isr_c2
		defb 0

	.isr_done
	#endasm
	
	// Border 0

	cpc_Border (0x58);
	
	blackout ();
	pal_set (my_inks);
	
	// Set mode

	#ifdef MODE_1
		cpc_SetMode (1);
	#else
		cpc_SetMode (0);
	#endif

	// Set tweaked mode 
	// (thanks Augusto Ruiz for the code & explanations!)
	
	#asm
		; Horizontal chars (32), CRTC REG #1
		ld    b, 0xbc
		ld    c, 1			; REG = 1
		out   (c), c
		inc   b
		ld    c, 32			; VALUE = 32
		out   (c), c

		; Horizontal pos (42), CRTC REG #2
		ld    b, 0xbc
		ld    c, 2			; REG = 2
		out   (c), c
		inc   b
		ld    c, 42			; VALUE = 42
		out   (c), c

		; Vertical chars (24), CRTC REG #6
		ld    b, 0xbc
		ld    c, 6			; REG = 6
		out   (c), c
		inc   b
		ld    c, 24			; VALUE = 24
		out   (c), c
	#endasm

	// Sprite creation

	// Player 

	// sp_sw struct is 16 bytes wide. This is easy
	// 0   2   4      6   7   8  9  10 11 12      14
	// sp0 sp1 coord0 cox coy cx cy ox oy invfunc updfunc

	/*
	sp_sw [SP_PLAYER].cox = sm_cox [0];
	sp_sw [SP_PLAYER].coy = sm_coy [0];
	sp_sw [SP_PLAYER].invfunc = sm_invfunc [0];
	sp_sw [SP_PLAYER].updfunc = sm_updfunc [0];
	sp_sw [SP_PLAYER].sp0 = sp_sw [SP_PLAYER].sp1 = (unsigned int) (sm_sprptr [0]);
	*/
	#asm
		.sprite_creation

			ld  ix, BASE_SPRITES
			
			ld  a, (_sm_cox) 			// sm_cox [0]
			ld  (ix + 6), a

			ld  a, (_sm_coy) 			// sm_coy [0]
			ld  (ix + 7), a

			ld  hl, (_sm_invfunc)		// sm_invfunc [0]
			ld  (ix + 13), h
			ld  (ix + 12), l

			ld  hl, (_sm_updfunc)		// sm_updfunc [0]
			ld  (ix + 15), h
			ld  (ix + 14), l

			ld  hl, (_sm_sprptr) 		// sm_sprptr [0]
			ld  (ix + 1), h
			ld  (ix + 0), l

			ld  (ix + 3), h
			ld  (ix + 2), l			
	#endasm

	// Enemy
	#asm
			ld  ix, #(BASE_SPRITES + 16)

			ld  a, (_sm_cox) 			// sm_cox [0]
			ld  (ix + 6), a

			ld  a, (_sm_coy) 			// sm_coy [0]
			ld  (ix + 7), a

			ld  hl, (_sm_invfunc)		// sm_invfunc [0]
			ld  (ix + 13), h
			ld  (ix + 12), l

			ld  hl, (_sm_updfunc)		// sm_updfunc [0]
			ld  (ix + 15), h
			ld  (ix + 14), l

			ld  hl, (_sm_sprptr) 		// sm_sprptr [0]
			ld  (ix + 1), h
			ld  (ix + 0), l

			ld  (ix + 3), h
			ld  (ix + 2), l		
	#endasm

	// Turn off all sprites

	#asm
			ld  ix, BASE_SPRITES
			ld  de, 16
			ld  b, SW_SPRITES_ALL

		.sp_sw_init_turnoff_loop			
			ld  a, 24
			ld  (ix + 8), a
			ld  (ix + 10), a

			xor a
			ld  (ix + 9), a 
			ld  (ix + 11), a 

			add ix, de
			djnz sp_sw_init_turnoff_loop
	#endasm	
	
	// Turn on ISR

	#asm
		ei
	#endasm

}
