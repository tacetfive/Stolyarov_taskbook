%include "stud_io.inc"
global	_start
; program reads input text and counts words in each line,
; then print stars. The word means number of space-and-CR separated symbols.
; Algorytm: if current symbol is letter and previous was Space or CR or 0 --
; then wordCounter increments.
; MOD: make this program except punctuation (",", ".", "/", "-", """, etc...)
section .text
_start:	
		xor	al, al		; al=0 char container
	;	xor	ah, ah		; ah for multiple spaces case
		xor	bl, bl		; bl=0 Word counter
		xor	cl, cl		; cl for multiple spaces case
read_char:	
		mov	cl, al		; move previous symbol to cl
      		GETCHAR
;		PUTCHAR al
;		PUTCHAR cl
		cmp	al, 11111111b	; is EOF?
		je	finish
		cmp	al, 10     	; is end of line?
		je	print_stars
		cmp	al, 32     	; is Space?
		jne	is_word_start   ; check if the're number of spaces
		jmp	read_char	; >Cycle of reading text<
is_word_start: ; runs only if in AL is not Space or CR:
		cmp	cl, 32		; is previous symbol Space?
		je	inc_word_count  ; word indicated!
		cmp 	cl, 00000000b	; is no previous symbols?
		je 	inc_word_count
		cmp	cl, 10		; is previous CR?
		je	inc_word_count
		jmp 	read_char
inc_word_count: 
		inc	bl
		jmp	read_char
print_stars:	
		cmp 	bl, 0
		je	read_another_line
		PRINT   "*"
		dec	bl
		jmp	print_stars
read_another_line:
		PUTCHAR	10
		jmp	read_char
finish:
		FINISH
