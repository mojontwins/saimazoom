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

void sync_coodinates (void) {
	#asm
			ld  a, (_ptx)
			ld  b, a 
			sla a 
			add b 
			ld  (_px), a 

			ld  a, (_pty)
			ld  b, a 
			sla a 
			add b 
			ld  (_py), a 
	#endasm
}

void move_jones (void) {

	// Update player
	if (pstate == STATE_WALK) {
		px += pmx; py += pmy;

		pct ++; if (pct == 3) {			
			pstate = STATE_IDLE;
		}

		pstep ^= 1;
	} else if (pdirection != 0xff) {
		pmx = dx [pdirection]; pmy = dy [pdirection];
		pfacing = pdirection << 1;

		_x = ptx + pmx; _y = pty + pmy;
		rda = attr (_x, _y);

		// Can move?
		if ((rda & 8) == 0) { 
			ptx += pmx; pty += pmy;
			pstate = STATE_WALK;
			pct = 0;
		} else {
			// Use object?

			if ((rda & 1) && (rdb = has_object (OBJ_BOAT) != 0xff)) {
			
				// Advance player twice in the current direction
				_x += pmx; _y += pmy;
				if (attr (_x, _y) == 0) {
					inventory [rdb] = OBJ_EMPTY;
					ptx = _x; pty = _y;
					sync_coodinates ();
					draw_inventory ();
				}
			} else if ((rda & 2) && (rdb = has_object (OBJ_SABER) != 0xff)) {
				inventory [rdb] = OBJ_EMPTY;
				draw_inventory ();
				clear_tile ();
				persist ();
			} else if ((rda & 4) && (rdb = has_object (OBJ_PICKAXE) != 0xff)) {
				inventory [rdb] = OBJ_EMPTY;
				draw_inventory ();
				clear_tile ();
				persist ();
			} else if ((rda & 16) && (rdb = has_object (OBJ_KEY) != 0xff)) {
				// Key is not consumed?
				clear_tile ();
			} else if (rda & 32) {
				// Object?
				if (pcooldown == 0) {
					// Find which object we are colliding to 
					// Function reads _x _y
					oid = find_object_index ();
					rda = object_id [oid];

					// Special case: water
					if (rda == OBJ_WATER) {
						object_id [oid] = OBJ_EMPTY;
						number_set (n_water);
					} else {
						// Inventory shift
						object_id [oid] = inventory_shift (rda);
						draw_inventory ();
					}

					// Update on screen
					draw_tile (_x, _y, object_id [oid]);
					invalidate_tile (_x, _y);
					
					// Cooldown 10 frames
					pcooldown = 10;
				} else {
					pcooldown --;
				}
			} else cpc_Border (0x58);
		}

		pdirection = 0xff;
	}

	rda = 0xff;

	// Read keyboard
	if (cpc_TestKey (KEY_UP)) {
		rda = DIRECTION_UP;
	} else if (cpc_TestKey (KEY_DOWN)) {
		rda = DIRECTION_DOWN;
	}

	if (cpc_TestKey (KEY_LEFT)) {
		rda = DIRECTION_LEFT;
	} else if (cpc_TestKey (KEY_RIGHT)) {
		rda = DIRECTION_RIGHT;
	}

	if (rda != 0xff) pdirection = rda; else {
		pdirection = 0xff;
		pcooldown = 0;			// Reset object interaction cooldown
	}
}

void move_malote (void) {

	switch (estate) {
		case STATE_OFF:
			// Make appear? 
			if ((rand () & 15) == 0) {
				eattempts = 3;

				// Select somewhere
				while (eattempts --) {
					etx = rand () & 7;
					ety = rand () & 7;

					if (eattr (etx, ety) == 0) {
						estate = STATE_APPEARING;
						break;
					}
				}

				if (estate == STATE_APPEARING) {
					ex = (etx << 1) + etx;
					ey = (ety << 1) + ety;
					ect = 10; estep = 0;
					etype = 8; 	// Sprite ID of "puff"
				}
			}
			break;

		case STATE_APPEARING:
		case STATE_KILLED:
			if (ect --) {
				estep ^= 1;
			} else if (estate == STATE_APPEARING) {
				etype = rand () & 7; if (etype > 4) etype -= 4;
				etype = 10 + (etype << 1);
				estate = STATE_IDLE;
			}
			break;

		case STATE_WALK:
			// Walk
			ex += emx; ey += emy;

			ect ++; if (ect == 3) {
				estate = STATE_IDLE;
			}

			estep ^= 1;
			break;

		default:
			// Move at random, if possible
			rda = rand() & 7; if (rda < 4) {
				emx = dx [rda]; emy = dy [rda];

				// Can move?
				if ((eattr (etx + emx, ety + emy) & 8) == 0) {
					etx += emx; ety += emy;
					estate = STATE_WALK;
					ect = 0;
				}
			}

			estep = rand () & 1;
	}
}

void update_sprites (void) {
	// Johnny
	#asm		
			ld  ix, #(BASE_SPRITES + (SP_PLAYER*16))
			ld  a, (_px)
			ld  (ix + 8), a 
			ld  a, (_py)
			ld  (ix + 9), a
			
			// _pframe = sm_sprptr [pfacing | pstep];
			ld  a, (_pfacing);
			ld  c, a 
			ld  a, (_pstep);
			or  c 
			sla a 
			ld  h, 0 
			ld  l, a 
			ld  de, _sm_sprptr 
			add hl, de 
			ld  a, (hl)
			inc hl 
			ld  h, (hl)
			
			ld  (ix + 0), a
			ld  (ix + 1), h
	#endasm

	// Enemy
	if (estate == STATE_OFF) {
		#asm
				// Move outside of screen
				ld  ix, #(BASE_SPRITES + (SP_ENEM*16))
				ld  (ix + 8), 24
				ld  a, (_py)
				ld  (ix + 9), 0
		#endasm
	} else {
		#asm
				ld  ix, #(BASE_SPRITES + (SP_ENEM*16))
				ld  a, (_ex)
				ld  (ix + 8), a 
				ld  a, (_ey)
				ld  (ix + 9), a
					
				// _eframe = sm_sprptr [etype | estep];
				ld  a, (_etype);
				ld  c, a 
				ld  a, (_estep);
				or  c 
				sla a 
				ld  h, 0 
				ld  l, a 
				ld  de, _sm_sprptr 
				add hl, de 
				ld  a, (hl)
				inc hl 
				ld  h, (hl)

				ld  (ix + 0), a
				ld  (ix + 1), h
		#endasm
	}
}

void main_loop (void) {
	// Decompress frame
	#asm
			ld  hl, s_marco
			ld  de, BASE_SUPERBUFF
			call dzx0_standard
	#endasm
	cpc_ShowTileMap (1);


	// Place objects at random
	init_objects ();
	init_inventory ();
	draw_inventory ();

	// Init tile persistence
	pid = 0;

	// Init player
	n_pant = 90; o_pant = 0xff;
	ptx = 2; px = 3*2;
	pty = 4; py = 3*4;
	pstep = 0;
	pfacing = 2;

	playing = 1;
	flip_flop = 0;

	// Init numbers
	number_set (n_water);
	number_reset (n_time);

	while (playing) {
		flip_flop ^= 1;

		if (flip_flop) number_increment (n_time);
		number_decrement (n_water);
		number_decrement (n_water);

		if (n_pant != o_pant) { 
			draw_map ();
			init_room_objects ();
			clear_tiles_on_enter ();
			invalidate_viewport ();
			o_pant = n_pant;

			pct = 0; pstate = STATE_IDLE;
			ect = 0; estate = STATE_OFF;

			pdirection = 0xff;
			sync_coodinates (); 
		}
	
		// Update sprites
		update_sprites ();

		// Sync
		#asm
			.ml_min_faps_loop
				ld  a, (isr_c2)
				cp  2
				jr  c, ml_min_faps_loop

			.ml_min_faps_loop_end
				xor a
				ld  (isr_c2), a
		#endasm

		// Draw numbers in hud
		_x = 26;
		_y = 9; gp_gen = n_time; print_number ();
		_y = 12; gp_gen = n_water; print_number ();

		// Update
		cpc_UpdateNow (1);
	
		// Collision?
		if (estate > STATE_APPEARING) {
			if (px >= ex - 2 && px <= ex + 2 && py >= ey - 2 && py <= ey + 2) {
				if ((rdb = has_object (OBJ_PISTOL)) != 0xff) {
					inventory [rdb] = OBJ_EMPTY;
					estate = STATE_KILLED;
					ect = 10; etype = 8;
				} else {
					o_pant = 0xff; 		// Force re-enter screen
					continue;
				}
			}
		}

		move_jones ();
		move_malote ();

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
