%include "stud_io.inc"
global	_start
; program counts symbols in input text 
; then print quantity of symbols in decimal format.
section .text
_start:
		cld			; DF=0, edi increases
		mov	edi, number	; edi == Destination Index
		mov	esi, edi	; save start address
		mov	ecx, 10		; 4-byte divider.
		xor	eax, eax	; char container
		xor	edx, edx	; clear dividend
		xor	ebx, ebx	; number container
read_char:
      		GETCHAR
		cmp	al, 11111111b 	; if EOF, end of reading.
		je	bin_to_dec	;
		inc	ebx		; chars counter
		jmp	read_char
bin_to_dec:
		mov 	eax, ebx	; move number of chars to eax.
		div	ecx		; eax will contain quotient;
					; edx will contain remainder.
		cmp 	eax, 0		; Is quotient zero?
		je	put_last_remainder
		mov 	ebx, eax	; Save quotient
		mov	eax, edx	; Move remainder to EAX and
		stosb			; write it to [edi]
		xor 	edx, edx	; clear edx.
		xor 	eax, eax	; clear eax.
		jmp	bin_to_dec
put_last_remainder:
		mov 	[edi], edx
print_number_start:
		mov	ecx, esi	; esi contains start address
		mov	esi, edi	; move end address to esi
		std			; DF=1
print_number:
		lodsb			; read digit to AL
		add 	eax, "0"
		PUTCHAR	al 
		cmp 	esi, ecx	; you cold also use rep prefix
		je	print_last_digit
		jmp	print_number
print_last_digit:			; what's better? using special codeblocks for last
		mov	eax, [esi]		; symbols or just dec edi/esi registers after
		add	eax, "0"		; putting/reading number in/out RAM?
		PUTCHAR al
finish:
		PUTCHAR 10
		FINISH

section .bss
number		resb	10 		; reserve 10 bytes for 32-bit integer
