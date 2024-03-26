// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Printer routines

// Tile address calculator.
void nametable_address(void) {
	#asm
			// BC = YX; returns DE = _nametable + Y * 32 + X

			ld  a, b 
			add a, a 		// x2
			add a, a 		// x4
			add a, a  		// x8
			ld  h, 0 
			ld  l, a 
			add hl, hl  	// x16
			add hl, hl 		// x32

			// HL = 32 * Y

			ld  d, 0 
			ld  e, c 
			add hl, de 

			// HL = 32 * Y + X

			ld  de, _nametable
			add hl, de 

			ex de, hl 

			// DE = _nametable + Y * 32 + X
	#endasm
}

// Simple 3x3 tile printer. x, y are tile coordinates.
void draw_tile(unsigned char x, unsigned char y, unsigned char t) {
	#asm
			ld  hl, 6
			add hl, sp 			// Three parameters

			ld  c, (hl) 		// x
			ld  a, c 
			add a, a 
			add a, c 
			ld  c, a 			// x3


			dec hl
			dec hl
			ld  b, (hl) 		// y
			ld  a, b 
			add a, a
			add a, b 
			ld  b, a 			// x3

			dec hl
			dec hl
			ld  a, (hl) 		// t

		.draw_tile_direct
			// Assumes BC = YX, A = T

			// We have 27 tiles, 27 * 9 = 243, so we do 8bit math
			// Calculate metatile address; t * 9 = (t * 8) + t
			ld  d, a 			// Save for later
			add a, a 
			add a, a
			add a, a 			// A = T * 8
			add a, d 			// A = T * 9

			ld  h, 0
			ld  l, a
			ld  de, _tmaps
			add hl, de 			// HL = metatile address

			push hl 			// Save
			call _nametable_address 	// this destroys HL!
			pop hl 				// Get it back

			// Copy three patterns
			ldi 
			ldi 
			ldi 				

			// Next row
			ex  de, hl 
			ld  bc, 29
			add hl, bc 
			ex  de, hl 

			// Copy three patterns
			ldi 
			ldi 
			ldi 				

			// Next row
			ex  de, hl 
			ld  bc, 29
			add hl, bc 
			ex  de, hl 

			ldi 
			ldi 
			ldi 				
			
	#endasm
}

void draw_map (void) {
	// Draws room "n_pant". Screens are 8x7 or 56 metatiles big
	// 100 * 56 = 5600 so 16 bits math is needed to make the calculations.
	// N * 56 = N * 32 + N * 16 + N * 8

	#asm
			ld  hl, (_n_pant)
			ld  h, 0

			add hl, hl 
			add hl, hl 
			add hl, hl 			// N * 8

			ld  d, h 
			ld  e, l 			// DE = N * 8 

			add hl, hl 			// N * 16

			ld  b, h 
			ld  c, l 			// BC = N * 16

			add hl, hl 			// N * 32
			add hl, bc 			// N * 32 + N * 16
			add hl, de 			// N * 32 + N * 16 + N * 8

			ld  de, _map 
			add hl, de 			// HL points to room n_pant

			ld  de, _map_attr

			ld b, 56 			// Iterate 56 times

		.draw_map_loop
			push bc 

			ld  a, (hl) 		// Get current tile 
			ld  (__t), a 
			push hl

			// Calculate attr of tile			
			ld  l, a 
			ld  h, 0 
			ld  bc, _tile_behs
			add hl, bc 

			ld  a, (hl) 
			ld  (de), a
			push de 

			ld  a, (__x)
			ld  c, a
			ld  a, (__y)
			ld  b, a 			// BC = YX
			ld  a, (__t)

			call draw_tile_direct

			ld  a, (__x)
			inc a 
			cp  8 
			jr  nz, dml_noincy

			ld  hl, __y 
			inc (hl)

			xor a 
		.dml_noincy
			ld  (__x), a

			pop de
			inc de 				// Next in attribute buffer

			pop hl 
			inc hl 				// Next tile

			pop bc 
			djnz draw_map_loop

	#endasm
}
