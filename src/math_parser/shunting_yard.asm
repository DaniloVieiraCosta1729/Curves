section .note.GNU-stack noalloc noexec nowrite

;*************************************************************
; Shunting yard algorithm
;
; This program is just an exercise. I want to implement this algorithm one time before I make the parser that generates an AST.
;*************************************************************

section .data
    MAX_OUTPUT_SIZE equ 256

    TOKENS db "1234567890x. +-*/()"

section .bss
    output resb MAX_OUTPUT_SIZE
    