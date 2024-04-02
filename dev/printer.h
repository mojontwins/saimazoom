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

			dec hl
			dec hl
			ld  b, (hl) 		// y

			dec hl
			dec hl
			ld  a, (hl) 		// t

		.draw_tile_direct
			// Assumes BC = YX, A = T, tile coodinates
			ld  d, a 			// Save T

			// Tile to cell coordinates:
			ld  a, c 
			add a, a 
			add a, c 
			ld  c, a 			// x3

			ld  a, b 
			add a, a
			add a, b 
			ld  b, a 			// x3

			ld  a, d 			// Get T back

		.draw_tile_direct2
			// Assumes BC = YX, A = D = T, tile coodinates

			// We have 27 tiles, 27 * 9 = 243, so we do 8bit math
			// Calculate metatile address; t * 9 = (t * 8) + t
			add a, a 
			add a, a
			add a, a 			// A = T * 8
			add a, d 			// A = T * 9

			ld  h, 0
			ld  l, a
			ld  de, _tmaps
			add hl, de 			// HL = metatile address

			push hl 			// Save
			call _nametable_address 	// this destroys HL, returns address in DE
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
			
			ret
	#endasm
}

void invalidate_tile (unsigned char x, unsigned char y) {
	#asm
			ld  hl, 4
			add hl, sp 			// Two parameters

			ld  c, (hl) 		// x

			dec hl
			dec hl
			ld  b, (hl) 		// y

		.invalidate_tile_direct
			// Assumes BC = YX, tile coodinates

			// Tile to cell coordinates:
			ld  a, c 
			add a, a 
			add a, c 
			ld  c, a 			// x3

			ld  a, b 
			add a, a
			add a, b 
			ld  b, a 			// x3

		.invalidate_tile_direct2
			// Assumes BC = YX, cell coordinates

			// BC = YX; calculates D = B + 2, E = C + 2

			ld  a, 2
			add b 
			ld  d, a 

			ld  a, 2
			add c 
			ld  e, a 

			call cpc_InvalidateRect
	#endasm
}

// Called ingame. Removes tile _x, _y from screen and buffer
void clear_tile(void) {
	#asm 
			ld  a, (__y)
			sla a
			sla a
			sla a
			ld  c, a
			ld  a, (__x)
			or  c 

			ld  h, 0 
			ld  l, a 
			ld  de, _map_attr 
			add hl, de 

			xor a 
			ld  (hl), 0

			ld  a, (__x)
			ld  c, a 
			ld  a, (__y)
			ld  b, a 
			push bc
			xor a 
			call draw_tile_direct
			pop bc 
			call invalidate_tile_direct
	#endasm
}

void get_pointer_to_n_pant_in_hl (void) {
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

			ld  de, _mapa
			add hl, de 			// HL points to room n_pant
	#endasm
}

void draw_map (void) {
	// Draws room "n_pant". Screens are 8x7 or 56 metatiles big
	// 100 * 56 = 5600 so 16 bits math is needed to make the calculations.
	// N * 56 = N * 32 + N * 16 + N * 8

	#asm
			xor a
			ld  (__x), a
			ld  (__y), a

			call _get_pointer_to_n_pant_in_hl

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
		.draw_map_done
	#endasm
}

void print_number(void) {
	// Prints number pointed by _gp_gen at (_x, _y).
	// Remember that numbers are stored backwards!
	#asm
		// Calculate initial nametable address.
		ld  a, (__x)
		add 3 					// Remember, numbers are stored backwards.
		ld  c, a
		ld  a, (__y)
		ld  b, a 

		push bc 				// Save for later (last position to invalidate)

		call _nametable_address
		ld  hl, (_gp_gen) 
		
		ld  a, (hl)
		inc hl 
		ld  (de), a 
		dec de

		ld  a, (hl)
		inc hl 
		ld  (de), a 
		dec de

		ld  a, (hl)
		inc hl 
		ld  (de), a 
		dec de

		ld  a, (hl)
		inc hl 
		ld  (de), a 		

		// Invalidate
	.print_number_inv
		pop de 
		ld  b, d
		ld  c, e 
		dec c
		dec c
		dec c
		
		call cpc_InvalidateRect

	#endasm
}
