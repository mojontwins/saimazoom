// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// System & backend routines

void blackout (void) {
	rda = BLACK_COLOUR_BYTE;
	#asm
			ld  a, 0xc0
		.bo_l1
			ld  h, a
			ld  l, 0
			ld  b, a
			ld  a, (_rda)
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
