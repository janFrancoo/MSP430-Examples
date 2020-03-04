;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
			.data
NUMBERS		.word	5, 8, -2, 4, -1
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
			mov.w	#NUMBERS, R4

			mov.w	#5, R5				; N
			mov.w	#0, R6				; i
			mov.w	#0, R7				; sum

			mov.w	#0x41, &P1DIR

FOR
			cmp.w	R6, R5
			jz		EXIT

			mov.w	@R4+, R8
			bit.w	#0x01, R8
			jz		IF

ELSE
			mov.w	#0x40, &P1OUT
			jmp		IFEXIT

IF
			mov.w	#0x01, &P1OUT

IFEXIT
			add.w	#1, R6
			jmp 	FOR

EXIT


ILOOP
			jmp		ILOOP


;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
