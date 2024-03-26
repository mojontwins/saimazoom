// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Engine

unsigned char attr(unsigned char x, unsigned char y) {
	// This is for the player. Will return "clear" if outside of the screen.
	#asm
			ld  hl, 4
			add hl, sp
			ld  c, (hl) 	// x

			dec hl
			dec hl
			ld  a, (hl) 	// y

		.attr_direct
			// A = y, C = x
			cp  7
			jr  c, _attr_1
			ld  hl, 0
			ret

		._attr_1
			ld  b, a 		// save y
			ld  a, c 		// x
			cp  8
			jr  c, _attr_1b
			ld  hl, 0
			ret

		._attr_1b
			ld  a, b 		// restore y

			sla a
			sla a
			sla a			// y * 8

			add c 			// y * 8 + x

			ld  d, 0
			ld  e, a
			ld  hl, _map_attr
			add hl, de
			ld  l, (hl)
			ld  h, 0

	#endasm
}

unsigned char eattr(unsigned char x, unsigned char y) {
	// This is for the enemies. Will return "blocked" if outside of the screen.
	// This is for the player. Will return "clear" if outside of the screen.
	#asm
			ld  hl, 4
			add hl, sp
			ld  c, (hl) 	// x

			dec hl
			dec hl
			ld  a, (hl) 	// y

		.eattr_direct
			// A = y, C = x
			cp  7
			jr  c, _eattr_1
			ld  hl, 8
			ret

		._eattr_1
			ld  b, a 		// save y
			ld  a, c 		// x
			cp  8
			jr  c, _attr_1b
			ld  hl, 8
			ret

	#endasm
}

void move_jones (void) {

	// Update player
	if (pstate == STATE_WALK) {
		px += pmx; py += pmy;

		pct ++; if (pct == 3) {			
			pstate = STATE_IDLE;
		}
	} else if (pdirection != 0xff) {
		pmx = dx [pdirection]; pmy = dy [pdirection];

		// Can move?
		if (attr (ptx + pmx, pty + pmy) & 8 == 0) {
			ptx += pmx; pty += pmy;
			pstate = STATE_WALK;
			pct = 0;
		} else {
			// Use object?

			// Object?

		}

		pdirection = 0xff;
	}

	// Read keyboard
	if (cpc_TestKey (KEY_UP)) {
		pdirection = pdirection_UP;
	} else if (cpc_TestKey (KEY_DOWN)) {
		pdirection = pdirection_DOWN
	}

	if (cpc_TestKey (KEY_LEFT)) {
		pdirection = pdirection_LEFT
	} else if (cpc_TestKey (KEY_RIGHT)) {
		pdirection = pdirection_RIGHT
	}
}

void move_malote (void) {

	if (estate == STATE_OFF) {
		// Make appear? 
		if ((rand () & 7) == 0) {
			ex = 0xff; eattempts = 3;

			// Select somewhere
			while (eattempts --) {
				ex = rand () & 7;
				ey = rand () & 7;

				if (attr (ex, ey) == 0) {
					estate = STATE_IDLE;
					break;
				}
			}

			if (estate == STATE_IDLE) {
				ex = (etx << 1) + etx;
				ey = (ety << 1) + ety;
				ect = 0;
			}
		}
	} else if (estate == STATE_WALK) {
		// Walk

		ex += emx; ey += emy;

		ect ++; if (ect == 3) {
			estate = STATE_IDLE;
		}
	} else {
		// Move at random, if possible
		gpt = rand() & 7;

		if (gpt < 4) {
			emx = dx [gpt]; emy = dy [gpt];

			// Can move?
			if (eattr (etx + emx, ety + emy) & 8 == 0) {
				etx += emx; ety += emy;
				estate = STATE_WALK;
				ect = 0;
			}
		}
	}
}

void main_loop (void) {
	n_pant = 90; o_pant = 0xff;
	ptx = 3; px = 3*3;
	pty = 4; py = 3*4;

	while (playing) {
		if (n_pant != o_pant) {
			draw_map ();
			o_pant = n_pant ();

			pct = 0; pstate = STATE_IDLE;
			ect = 0; estate = STATE_OFF;
		}

		move_jones ();
		move_malote ();

		// Update sprites
		cpc_UpdateNow (1);

		// Collision?
		if (px >= ex - 2 && px <= ex + 2 && py >= ey + 2 && py <= ey + 2) {
			o_pant = 0xff; 		// Force re-enter screen
			continue;
		}

		// Flick screen ?
		// Note how player ptx, pty may be outside of the screen
		if (ptx == 0xff) {
			n_pant --;
			ptx = 7; px = 7*3;
		} else if (ptx == 8) {
			n_pant ++; 
			ptx = px = 0;
		}

		if (pty == 0xff) {
			n_pant -= 10;
			pty = 6; py = 6*3;
		} else if (pty == 7) {
			n_pant += 10;
			pty = py = 0;
		}
	}
}
