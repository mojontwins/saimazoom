// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Numbers and stuff

unsigned char ni;
unsigned char nc;

// 4 digit numbers used in time, score and water are stored as 4 byte arrays
// These routines increment or decrement such numbers

void __fastcall__ number_reset (unsigned char *number) {
	#asm
			// Fastcall puts the 16 bit parameter into HL

			ld  e, (hl)
			inc hl 
			ld  d, (hl)

			xor a 
			ld  (de), a
			inc a
			ld  (de), a
			inc a
			ld  (de), a
			inc a
			ld  (de), a

	#endasm
}

void __fastcall_ number_increment (unsigned char *number) {
	#asm
			// Fastcall puts the 16 bit parameter into HL
			ld  e, (hl)
			inc hl 
			ld  d, (hl)
			
			// Number "1234" is stored backwards as "4321", so 			
			
			ld  a, (de)
			inc a 			// Increment current digit
						
			cp  10  		// Has this digit overflowed?
			jr  nz, ni_end 	// If no overflow, we're done

			xor a 
			ld  (de), a 	// Write a 0

			inc de 			// Next digit.

			ld  a, (de)
			inc a 			// Increment current digit
						
			cp  10  		// Has this digit overflowed?
			jr  nz, ni_end 	// If no overflow, we're done

			xor a 
			ld  (de), a 	// Write a 0

			inc de 			// Next digit.

			ld  a, (de)
			inc a 			// Increment current digit
						
			cp  10  		// Has this digit overflowed?
			jr  nz, ni_end 	// If no overflow, we're done

			xor a 
			ld  (de), a  	// Write a 0

			inc de 			// Next digit.

			ld  a, (de)
			inc a  			// Increment last digit. Don't care for overflows

		.ni_end
			ld  (de), a 

			// If the last digit overflows -> do nothing, this WON'T HAPPEN			
	#endasm
}

void __fastcall__ number_decrement (unsigned char *number) {
	#asm
			// Fastcall puts the 16 bit parameter into HL
			ld  e, (hl)
			inc hl 
			ld  d, (hl)

			ld  a, (de)
			dec a 
			cp  255 		// Has this digit overflowed?

			jr  nz, nd_end 	// No overflow, we're done 

			ld  a, 9 
			ld  (de), a  	// Write a 9

			inc de  		// Next digit

			ld  a, (de)
			dec a 
			cp  255 		// Has this digit overflowed?

			jr  nz, nd_end 	// No overflow, we're done 

			ld  a, 9 
			ld  (de), a  	// Write a 9

			inc de  		// Next digit

			ld  a, (de)
			dec a 
			cp  255 		// Has this digit overflowed?

			jr  nz, nd_end 	// No overflow, we're done 

			ld  a, 9 
			ld  (de), a  	// Write a 9

			inc de  		// Next digit

			ld  a, (de)
			dec a 

		.nd_end		
			ld  (de), a 

			// If the last digit overflows -> do nothing, this WON'T HAPPEN		
	#endasm
}

void number_add (unsigned char *number1, unsigned char *number2) {
	ni = 0; while (ni < 3) {
		number1 [ni] += number2 [ni];

		// Worst case is 9 + 9 + 1 = 19, carry still 1.

		if (number1 [ni] > 9) {
			number1 [ni] -= 10;
			number1 [ni ++] ++; 		// Carry
		} else break;
	}
}
