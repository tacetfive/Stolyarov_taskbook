%include "stud_io.inc"
global	_start
; mod: program could calculate sum of digits in each line
; (sum = 0 for empty line)
; digits have ascii code 48--57.
section .text
_start:	
		xor	bl, bl		; there will be sum
read_char:	
      		GETCHAR
		cmp	al, 10
		je	print_stars
		cmp	al, 48		; 48 is ascii 0
		jge	is_digit_high	; if greater or equal, check high edge
		jmp 	read_char
is_digit_high:  
		cmp	al, 57		; 57 is ascii 9
		jle	sum_line_digits ; if less or equal
		jmp	read_char
sum_line_digits:
		sub	al, 48
		add	bl, al
		jmp	read_char
print_stars:	
		cmp 	bl, 0
		je 	finish
		PRINT "*"
		dec	bl
		jmp	print_stars
finish:
		PUTCHAR 10
		FINISH


