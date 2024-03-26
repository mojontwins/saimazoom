// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Cyclic inventory routines

unsigned char inventory[4];

unsigned char inventory_shift (unsigned char entering_object) {
	// entering_object will go to inventory [0], inventory [0] to [3] will be shifted forwards,
	// inventory [3] will be returned.

	rda = inventory [3];
	for (i = 0; i < 3; i ++) inventory [i + 1] = inventory [i];
	inventory [0] = entering_object;

	return rda;
}

unsigned char has_item (unsigned char item) {
	return inventory [0] == item || inventory [1] == item || inventory [2] == item || inventory [3] == item;
}
