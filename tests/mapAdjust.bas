Print "if map_orig = 18 then map = 0 else map = map_orig + 1"

Dim As Integer fIn, fOut
Dim As uByte b

fIn = FreeFile
Open "mapa_orig.map" For Binary As #fIn

fOut = FreeFile
Open "mapa.map" For Binary As #fOut

While Not Eof (fIn)
	Get #fIn, , b
	If b = 18 Then 
		b = 0
	Else
		b = b + 1
	End If 
	Put #fOut, , b
Wend
Print "DONE"
