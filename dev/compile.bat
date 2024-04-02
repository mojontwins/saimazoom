@echo off

..\utils\mkts_om.exe platform=cpc cpcmode=0 greyordered mode=mapped       pal=..\gfx\paleta-game.png in=..\gfx\sprites.png  out=..\bin\ts.bin mappings=..\bin\tsmaps.bin offset=0,6 size=10,3 max=28 metasize=3,3 tmapoffs=10 silent >nul
..\utils\mkts_om.exe platform=cpc cpcmode=0             mode=sprites      pal=..\gfx\paleta-game.png in=..\gfx\sprites.png  out=..\bin\ss.bin mappings=assets\ssmaps.h   offset=0,0 size=10,2 max=20 metasize=3,3             silent >nul

..\utils\mkts_om.exe platform=cpc cpcmode=0             mode=superbuffer  pal=..\gfx\paleta-game.png in=..\gfx\marcador.png out=..\bin\marco.bin silent > nul
..\utils\mkts_om.exe platform=cpc cpcmode=0             mode=superbuffer  pal=..\gfx\paleta-game.png in=..\gfx\menu.png     out=..\bin\title.bin silent > nul

..\utils\mkts_om.exe platform=cpc cpcmode=0 greyordered mode=chars        pal=..\gfx\paleta-game.png in=..\gfx\sprites.png  out=..\bin\numbers.bin offset=0,15 max=10 silent > nul

..\utils\mkts_om.exe platform=cpc mode=pals in=..\gfx\paleta-game.png prefix=my_inks out=assets\pal.h silent > nul

..\utils\mapcnv.exe ..\map\mapa.map assets\mapa.h 10 10 8 7 99 > nul

del ..\bin\*c.bin >nul
..\utils\zx0.exe ..\bin\marco.bin ..\bin\marcoc.bin > nul
..\utils\zx0.exe ..\bin\title.bin ..\bin\titlec.bin > nul

zcc +cpc -m -vn -unsigned -zorg=1024 -lcpcrslib -o saimazoom.bin system\tilemap_conf.asm saimazoom.c > nul

..\utils\printsize.exe saimazoom.bin

del saimazoom.sna > nul
..\utils\cpctbin2sna.exe saimazoom.bin 0x400 -pc 0x400 -o saimazoom.sna
