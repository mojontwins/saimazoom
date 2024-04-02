// Saimazoom CPC
// Copyleft 2023 by The Mojon Twins

// Numbers and stuff

unsigned char ni;
unsigned char nc;

// 4 digit numbers used in time, score and water are stored as 4 byte arrays
// These routines increment or decrement such numbers

// Make number = 0000
void __FASTCALL__ number_reset (unsigned char *number) {
	#asm
			// Fastcall puts the 16 bit parameter into HL

			xor a 

		.number_fill
			ld  (hl), a
			inc hl
			ld  (hl), a
			inc hl
			ld  (hl), a
			inc hl
			ld  (hl), a

	#endasm
}

// Make number = 9999
void __FASTCALL__ number_set (unsigned char *number) {
	#asm
			// Fastcall puts the 16 bit parameter into HL

			ld  a, 9
			jp  number_fill			

	#endasm
}


void __FASTCALL__ number_increment (unsigned char *number) {
	#asm
			// Number "1234" is stored backwards as "4321", so 			
			
			ld  a, (hl)
			inc a 			// Increment current digit
						
			cp  10  		// Has this digit overflowed?
			jr  nz, ni_end 	// If no overflow, we're done

			xor a 
			ld  (hl), a 	// Write a 0

			inc hl 			// Next digit.

			ld  a, (hl)
			inc a 			// Increment current digit
						
			cp  10  		// Has this digit overflowed?
			jr  nz, ni_end 	// If no overflow, we're done

			xor a 
			ld  (hl), a 	// Write a 0

			inc hl 			// Next digit.

			ld  a, (hl)
			inc a 			// Increment current digit
						
			cp  10  		// Has this digit overflowed?
			jr  nz, ni_end 	// If no overflow, we're done

			xor a 
			ld  (hl), a  	// Write a 0

			inc hl 			// Next digit.

			ld  a, (hl)
			inc a  			// Increment last digit. Don't care for overflows

		.ni_end
			ld  (hl), a 

			// If the last digit overflows -> do nothing, this WON'T HAPPEN			
	#endasm
}

void __FASTCALL__ number_decrement (unsigned char *number) {
	#asm
			// Fastcall puts the 16 bit parameter into HL
	
			ld  a, (hl)
			dec a 
			cp  255 		// Has this digit overflowed?

			jr  nz, nd_end 	// No overflow, we're done 

			ld  a, 9 
			ld  (hl), a  	// Write a 9

			inc hl  		// Next digit

			ld  a, (hl)
			dec a 
			cp  255 		// Has this digit overflowed?

			jr  nz, nd_end 	// No overflow, we're done 

			ld  a, 9 
			ld  (hl), a  	// Write a 9

			inc hl  		// Next digit

			ld  a, (hl)
			dec a 
			cp  255 		// Has this digit overflowed?

			jr  nz, nd_end 	// No overflow, we're done 

			ld  a, 9 
			ld  (hl), a  	// Write a 9

			inc hl  		// Next digit

			ld  a, (hl)
			dec a 

		.nd_end		
			ld  (hl), a 

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
