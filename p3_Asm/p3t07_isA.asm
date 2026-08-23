%include "stud_io.inc"
global	_start
; the GETCHAR macro is bufferized input. Make this program non-bufferized input.
section .text
_start:	
		mov	ebx,	41h
      		GETCHAR
		cmp	eax, ebx
		je	pr_yes		;jump if equal
		PRINT	"NO"
		jmp	finish
pr_yes:  	
		PRINT	"YES"
finish:
		PUTCHAR 10
		FINISH
