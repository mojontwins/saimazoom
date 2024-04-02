' Does the opposite to mapcnv.

Sub usage
	Print "Usage:"
	Print "$ mapDeConvert.exe mapaScr.map mapaFull.map w h scr_w scr_h"
	Print
End Sub

Dim As Integer mapW, mapH, scrW, scrH
Dim As Integer fIn, fOut 
Dim As Integer i, j, x, y

Redim As uByte map (0, 0)

If Command (6) = "" Then usage: End

mapW = Val (Command (3))
mapH = Val (Command (4))
scrW = Val (Command (5))
scrH = Val (Command (6))

Redim As uByte map (mapH * scrH, mapW * scrW)

' Read & reorder

fIn = FreeFile
Open Command (1) For Binary As #fIn

For i = 0 To mapH - 1
	For j = 0 To mapW - 1
		For y = 0 To scrH - 1
			For x = 0 To scrW - 1
				Get #fIn, , map (i * scrH + y, j * scrW + x)
			Next x
		Next y
	Next j
Next i

Close #fIn

' Write

fOut = FreeFile
Open Command (2) For Binary As #fOut

For y = 0 To mapH * scrH - 1
	For x = 0 To mapW * scrW - 1
		Put #fOut, , map (y, x)
	Next x
Next y

Close #fOut
Print "DONE"
