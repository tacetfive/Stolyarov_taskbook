%include "stud_io.inc"
global	_start
; program reads input text consisting of positive 32-bit integer number,
; then print corresponding quantity of stars.
; check: $ echo 100 | ./p3_18... | wc -c
; the wc output will be greater by one because of printing CR in the end
; of this program.
section .text
_start:
		xor	eax, eax	; char container
		xor	ebx, ebx	; number container
		xor	ecx, ecx	; number copy
		mov	esi, 10		; to multiply. I don't use EDX 
read_char:
      		GETCHAR
is_digit_check:
		cmp	al, "0"	 	; if not digit, end of reading.
		jl	print_stars	; jump if less
		cmp	al, "9"		; 
		jg	print_stars   	; jump if greater 
		sub 	al, "0"		; translate char to digit
		mov	cl, al		; save currently readed digit in cl
		mov 	eax, ebx	; ebx is current number
		mul	esi		; multiply current number by 10
		add	eax, ecx	; add last readed digit  
		mov	ebx, eax	; update current number
		jmp	read_char
print_stars:	
		cmp 	ebx, 0
		je	finish
		PRINT   "*"
		dec	ebx
		jmp	print_stars
finish:
		PUTCHAR 10
		FINISH
