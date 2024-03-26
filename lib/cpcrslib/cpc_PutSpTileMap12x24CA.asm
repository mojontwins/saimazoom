; ******************************************************
; **       Librer�a de rutinas para Amstrad CPC       **
; **       Ra�l Simarro,      Artaburu 2007           **
; ******************************************************

; [na_th_an] Nueva versi�n reescrita por Mojon Twins 
; - eliminamos la "cabecera" de los sprites.
; - la funci�n marca por separado los tiles "anteriores" (ox,oy)
;   y los tiles "actuales" (cx, cy).
; - Intenta "podar" los casos en los que los rect�ngulos 
;   sean iguales.
; - Intenta "afinar" y marcar solo 2/3 de long. en vez de 3/4
;   cuando sea posible.
; - Prescinde de bucles y otros c�lculos al ser un caso muy concreto

; Esta versi�n de la rutina asume coordenadas "de celda"
; x = 0-31; y = 0-23.

; El sprite es de 3x3 celdas, o 12x24 ladrillos.

XLIB cpc_PutSpTileMap12x24CA

XREF tiles_tocados
XREF pantalla_juego
XREF posiciones_super_buffer
XREF tiles

LIB cpc_UpdTileTableSaimazoom

.cpc_PutSpTileMap12x24CA
    ex  de, hl
    ld  ixh, d
    ld  ixl, e

    ; El sprite siempre est� alineado, as� que se marcar� 9 celdas

    ld e, (ix + 10) ; ox
    ld d, (ix + 11) ; oy

    ; Marcar OX, OY

    call do_update

    ld e, (ix + 8) ; cx
    ld d, (ix + 9) ; cy

    ; Copiamos (cx,cy) -> (ox,oy)

    ld (ix + 10), e
    ld (ix + 11), d

    ; Marcar CX, CY

do_update:
    ; X e Y est� ya en coordeadas de "rejilla

    push de                    ; Guardamos coordenadas

    ; Marco el tile origen
    call cpc_UpdTileTableSaimazoom   ; Marca el tile en DE

    ; Y el de su derecha
    inc e
    call cpc_UpdTileTableSaimazoom   ; Marca el tile en DE

    ; Y el siguiente
    inc e
    call cpc_UpdTileTableSaimazoom   ; Marca el tile en DE

origin_next_row:
    pop de                     ; Recuperamos   
    inc d                      ; Y = Y + 1
    push de                    ; Guardamos

    ; Marco el primer tile
    call cpc_UpdTileTableSaimazoom   ; Marca el tile en DE

    ; Y el de su derecha
    inc e
    call cpc_UpdTileTableSaimazoom   ; Marca el tile en DE
    ; Y el siguiente

    inc e
    call cpc_UpdTileTableSaimazoom   ; Marca el tile en DE

origin_next_row2:
    pop de                     ; Recuperamos   
    inc d                      ; Y = Y + 1

    ; Marco el primer tile
    call cpc_UpdTileTableSaimazoom   ; Marca el tile en DE

    ; Y el de su derecha
    inc e
    call cpc_UpdTileTableSaimazoom   ; Marca el tile en DE

    ; Y el siguiente
    inc e
    call cpc_UpdTileTableSaimazoom   ; Marca el tile en DE    

fin:
    ret
