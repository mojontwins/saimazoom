// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Objects.

unsigned char find_object_id (unsigned char x, unsigned char y) {
	// Finds which object is at x, y in current n_pant

	rda = (y << 3) | x;

	for (gpit = 0; gpit < MAX_OBJECTS; gpit ++) {
		if (object_n_pant [gpit] == n_pant && object_yx [gpit] == rda) return gpit;
	}

	return 0xff;
}
