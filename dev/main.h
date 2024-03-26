// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Main file

void main (void) {
	init ();

	for(rda = 0; rda < 28; rda ++) {
		draw_tile (rda & 7, rda >> 3, rda);
	}

	invalidate_viewport ();

	#asm
		ld  ix, BASE_SPRITES
		ld  a, 5
		ld  (ix + 8), a
		ld  (ix + 9), a

		ld  hl, (_sm_sprptr)
		ld  (ix + 0), l
		ld  (ix + 1), h

		ld  ix, #(BASE_SPRITES + 16)
		ld  a, 15
		ld  (ix + 8), a
		ld  (ix + 9), a

		ld  hl, (_sm_sprptr+20)
		ld  (ix + 0), l
		ld  (ix + 1), h
	#endasm

	while (1) {
		cpc_UpdateNow (1);
	}
}
