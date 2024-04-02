// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Objects.

// Objects will be placed at random (with restrictions) at the beginning of each game.
// Keys should NOT be placed inside of caves.
// One coffee sack should be placed inside of a cave.
// All objects substitute a non walkable tile and must be placed X = 1..6, Y = 1..5

unsigned char cave_room_numbers [] = { 38, 39, 48, 49 };

void find_random_no_cave_room (void) {
	// Never an infinite loop. A suitable value is guaranteed as the random lut contains all values
	while (1) {
		n_pant = rand ();
		if (
			n_pant < 100 && 
			n_pant != 38 && n_pant != 39 && n_pant != 48 && n_pant != 49
		) {
			return;
		}
	}
}

void find_random_cave_room (void) {
	n_pant = cave_room_numbers [rand () & 3];
}

void get_random_yx_in_rdb (void) {
	// Pracalculate a pointer to the current screen (in n_pant)
	#asm
			call _get_pointer_to_n_pant_in_hl
			ld  (_gp_gen), hl
	#endasm

	// Never an infinite loop. All rooms have non walkable tiles in the center.
	while (1) {
		rdb = rand () & 63; _x = rdb & 7;
		if (rdb > 8 && rdb < 47 && _x != 0 && _x != 7) {
			// Pick this yx if there's a non walkable tile
			if (*(gp_gen + rdb)) {
				// And make sure this place is not already taken!
				rda = 0;
				for (gpit = 0; gpit < oid; gpit ++) {
					if (object_n_pant [gpit] == n_pant && object_yx [gpit] == rdb) {
						rda = 1; break;
					}
				}
			}

			if (rda == 0) return;
		}
	}
}

void place_object_rdc () {
	object_n_pant [oid] = n_pant;
	object_yx [oid] = rdb;
	object_id [oid] = rdc;

	oid ++;
}

void place_n_objects (unsigned char n) {
	for (gpit = 0; gpit < n; gpit ++) {
		find_random_no_cave_room (); 		// writes to n_pant
		get_random_yx_in_rdb (); 			// finds YX inside roop to place object
		place_object_rdc (); 				// places object rdc in n_pant:rdb
	}
}

void init_objects (void) {
	oid = 0;

	// Two keys, outside of the caves (rooms # )
	rdc = OBJ_KEY;
	place_n_objects (MAX_KEYS);

	// Three sacks, outside of the caves.
	rdc = OBJ_SACK;
	place_n_objects (MAX_SACKS - 1);	

	// One sack, inside the caves.
	find_random_cave_room ();
	get_random_yx_in_rdb ();
	place_object_rdc ();

	// 16 waters
	rdc = OBJ_WATER;
	place_n_objects (MAX_WATERS);

	// 4 boats
	rdc = OBJ_BOAT;
	place_n_objects (MAX_BOATS);

	// 6 sabers
	rdc = OBJ_SABER;
	place_n_objects (MAX_SABERS);

	// 4 pickaxes
	rdc = OBJ_PICKAXE;
	place_n_objects (MAX_PICKAXES);

	// 6 pistols
	rdc = OBJ_PISTOL;
	place_n_objects (MAX_PISTOLS);

	// Total: 42 objects
}

// To help with processing time, objects will be marked in the hardness map upon
// entering room.

void init_room_objects (void) {
	for (gpit = 0; gpit < MAX_OBJECTS; gpit ++) {
		if (object_n_pant [gpit] == n_pant) {
			// Mark as object:
			map_attr [rda = object_yx [gpit]] = 40; 	// which is 8 (obstacle) | 32 (object)

			// Draw
			draw_tile (rda & 7, rda >> 3, object_id [gpit]);
		}
	}
}

unsigned char find_object_index (void) {
	// Finds which object is at _x, _y in current n_pant

	rdm = (_y << 3) | _x;

	for (gpit = 0; gpit < MAX_OBJECTS; gpit ++) {
		if (object_n_pant [gpit] == n_pant && object_yx [gpit] == rdm) return gpit;
	}

	return 0xff;
}
