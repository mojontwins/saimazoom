// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Reglas del original (Spectrum, Dinamic 1984) que faltaban en el port:
// puntos, vidas, fin de partida, victoria y puertas de las cuevas.
// Referencias "BASIC nnnn" = lineas del listado decompilado del original.

#define RESULT_ABORT  0
#define RESULT_DEAD   1
#define RESULT_TIME   2
#define RESULT_WIN    3

#define LIVES_X       29 			// Mismo sitio que en el Spectrum (AT 2,29)
#define LIVES_Y       2
#define SCORE_Y       6 			// Fila de los digitos bajo "PUNTOS"

unsigned char lives;
unsigned int score;
unsigned int gpint;
unsigned char n_score [] = { 0, 0, 0, 0 };
unsigned char game_result;
unsigned char door_visited;


// Espera n frames de 50 Hz (usa el contador de la ISR)
void __FASTCALL__ wait_frames (unsigned char n) {
	#asm
			ld  b, l
		.wf_loop
			xor a
			ld  (isr_c2), a
		.wf_wait
			ld  a, (isr_c2)
			or  a
			jr  z, wf_wait
			djnz wf_loop
	#endasm
}

// Pinta el digito _t en (_x, _y), en celdas
void print_digit (void) {
	#asm
			ld  a, (__x)
			ld  c, a
			ld  a, (__y)
			ld  b, a
			push bc
			call _nametable_address
			ld  a, (__t)
			ld  (de), a
			pop bc
			ld  d, b
			ld  e, c
			call cpc_InvalidateRect
	#endasm
}

void draw_lives (void) {
	_x = LIVES_X; _y = LIVES_Y; _t = lives; print_digit ();
}

// Valor binario de un numero BCD de 4 digitos (guardado al reves)
unsigned int number_value (unsigned char *n) {
	return n [0] + 10 * n [1] + 100 * n [2] + 1000 * n [3];
}

// BASIC: puntos es un entero; el marcador tiene 4 cifras, asi que saturamos.
void score_add (unsigned int v) {
	score += v; if (score > 9999) score = 9999;
	gpint = score;
	for (gpit = 0; gpit < 4; gpit ++) {
		n_score [gpit] = gpint % 10; gpint = gpint / 10;
	}
}

// BASIC 270: PAUSE 100 : vidas = vidas - 1 : IF vidas < 0 THEN fin
// Con 3 vidas se puede morir 4 veces (3, 2, 1, 0 y fuera).
void lose_life (void) {
	wait_frames (100);
	if (lives == 0) {
		game_result = RESULT_DEAD; playing = 0;
	} else {
		lives --;
		draw_lives ();
		o_pant = 0xff; 				// Redibuja la sala y resetea el enemigo
	}
}

// BASIC 120: gana si lleva 4 sacos de cafe (los 4 huecos del inventario)
// y esta de vuelta en la casilla de salida.
void check_win (void) {
	if (n_pant == 90 && ptx == 2 && pty == 4 && pstate == STATE_IDLE &&
		inventory [0] == OBJ_SACK && inventory [1] == OBJ_SACK &&
		inventory [2] == OBJ_SACK && inventory [3] == OBJ_SACK) {
		game_result = RESULT_WIN; playing = 0;
	}
}

// BASIC 1130-1220: las puertas de las cuevas no se abren, te teletransportan.
// Hace falta la llave (no se gasta). Numeracion de salas del port.
// sala origen, sala destino, x, y de llegada (en tiles). Las entradas van por
// parejas (i, i^1) para saber cual es la puerta de vuelta.
unsigned char doors [] = {
	15, 39, 5, 3, 		39, 15, 4, 5,
	26, 49, 1, 3, 		49, 26, 4, 5,
	98, 38, 4, 1, 		38, 98, 2, 0,
	99, 48, 4, 5, 		48, 99, 5, 5
};

void cave_door (void) {
	for (gpit = 0; gpit < 8; gpit ++) {
		gp_gen = doors + (gpit << 2);
		if (gp_gen [0] == n_pant) {
			// +500 si la sala de la que sales no ha sido marcada; se marca
			// la de llegada (BASIC 1210-1220). score_add usa gpit: guardamos antes.
			rdc = 1 << gpit;
			rdm = 1 << (gpit ^ 1);
			if ((door_visited & rdc) == 0) score_add (500);
			door_visited |= rdm;

			n_pant = gp_gen [1];
			ptx = gp_gen [2]; pty = gp_gen [3];
			px = (ptx << 1) + ptx; py = (pty << 1) + pty;
			pstate = STATE_IDLE;
			return;
		}
	}
}

// TODO: pantallas propias. El original muestra "ESTAS MUERTO",
// "SIN TIEMPO" o "LO HAS CONSEGUIDO / TIEMPO EMPLEADO nnnn" (BASIC 290-340).
void game_end (void) {
	if (game_result == RESULT_ABORT) return;
	cpc_Border (game_result == RESULT_WIN ? 0x52 : 0x4C);
	wait_frames (150);
	while (cpc_TestKey (KEY_BUTTON_A) == 0);
	cpc_Border (0x58);
}
