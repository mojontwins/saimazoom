; ******************************************************
; **       Librer�a de rutinas para Amstrad CPC       **
; **	   Ra�l Simarro, 	  Artaburu 2007           **
; ******************************************************

; [na_th_an] Modificada por Mojon Twins - eliminamos la "cabecera" de los sprites.
; [na_th_an] Modificada por Mojon Twins - eliminamos la "cabecera" de los sprites.
; [na_th_an] Esta versi�n utiliza OR para mezclar con lo que ya haya. No necesita LUT.

XLIB cpc_PutTrSp8x16TileMap2bG

XREF tiles_tocados
XREF pantalla_juego		
XREF posiciones_super_buffer
XREF tiles
XREF ancho_pantalla_bytes 
XREF posicion_inicial_superbuffer

.cpc_PutTrSp8x16TileMap2bG

	;seg�n las coordenadas x,y que tenga el sprite, se dibuja en el buffer 
    ex de,hl	;4
    LD IXH,d	;9
    LD IXL,e	;9 

	;lo cambio para la rutina de multiplicar 
    ld e,(ix+8)		;x
    ld h,(ix+9)		;y

.pasa_bloque_a_sbuffer

.transferir_sprite
	ld d,0

	; Multiplico HL = H * 64, offset "Y"
	ld  l, 0
	srl h
	rr  l
	srl h
	rr  l
	
	add hl,de 		; sumo offset "X"

	ld de,posicion_inicial_superbuffer
	add hl,de

	;hl apunta a la posici�n en buffer (destino)
			
	ld e,(ix+0)
    ld d,(ix+1)	;HL apunta al sprite
	
.sp_buffer_mask
	ld ixh,16

.loop_alto_map_sbuffer

	; El ancho est� desenrollado: Hay que procesar y copiar 4 bytes.

	ld a, (de) 		; Get sprite
	or (hl) 		; Get bg + draw pixels
	ld (hl), a 		; save BG+sprite
	inc de
	inc hl

	ld a, (de) 		; Get sprite
	or (hl) 		; Get bg + draw pixels
	ld (hl), a 		; save BG+sprite
	inc de
	inc hl

	ld a, (de) 		; Get sprite
	or (hl) 		; Get bg + draw pixels
	ld (hl), a 		; save BG+sprite
	inc de
	inc hl

	ld a, (de) 		; Get sprite
	or (hl) 		; Get bg + draw pixels
	ld (hl), a 		; save BG+sprite
	inc de
	
	;*************************************************		
		
	dec ixh
	ret z

	; de += 61 (next line in bg)
	ld bc, 61
	add hl, bc

	jp loop_alto_map_sbuffer
