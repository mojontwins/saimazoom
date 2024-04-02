// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Program skeleton. 

// CPC Memory map
// 0100 tilemap
// 0400 ejecutable
// 8800 descomprimir canción
// 9000 buffer
// C000 pantalla (a trozos)
// C600 room buffers
// CE00 dirty cells (tiles_tocados)
// D600 arrays
// DF80 buffers WYZ
// E600 sprite structures
// FE00 LUT

#include <cpcrslib.h>

#include "system.h"

#include "definitions.h"
#include "assets/pal.h"
#include "assets/ss.h"
#include "assets/ssmaps.h"
#include "assets/ts.h"
#include "assets/mapa.h"

#include "zx0.h"

#include "bitmaps.h"
#include "printer.h"
#include "numbers.h"
#include "update.h"
#include "menu.h"
#include "objects.h"
#include "inventory.h"
#include "persistence.h"
#include "engine.h"

#include "wyz/efectos.h"
#include "wyz/instrumentos.h"
#include "wyz/songs.h"
#include "wyz/wyz_player.h"

#include "init.h"

#include "main.h"
