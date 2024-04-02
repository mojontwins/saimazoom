// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Persistence - so broken tiles "persist"
// There are 6 sabers and 4 pickaxes, so we'll have to store 
// 10 tiles at most (MAX_PERSISTENT)

void persist (void) {
	persistence_n_pant [pid] = n_pant;
	persistence_yx [pid] = (_y << 3) | _x;
	pid ++;
}

void clear_tiles_on_enter (void) {
	for (gpit = 0; gpit < MAX_PERSISTENT; gpit ++ ) {
		if (persistence_n_pant [gpit] == n_pant) {
			map_attr [rda = persistence_yx [gpit]] = 0;
			draw_tile (rda & 7, rda >> 3, 0);
		}
	}
}
