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

            .global _main		    		; define entry point

_main       mov.w   #0280h, SP              ; initialize stack pointer
            mov.w   #WDTPW+WDTHOLD, &WDTCTL ; stop watchdog timer

            bis.b   #01000001b, &P1DIR      ; make P1.0 and P1.6 output
                                            ; all others are inputs by default

Mainloop    bit.b   #00001000b, &P1IN       ; read switch at P1.3
            jc      Off                     ; if P1.3 open branch to Off label

On          bic.b   #00000001b, &P1OUT      ; clear P1.0 (red off)
            bis.b   #01000000b, &P1OUT      ; set P1.6 (green on)
            jmp     Wait                    ; branch to a delay routine

Off         bis.b   #00000001b, &P1OUT		; set P1.0 (red on)
            bic.b   #01000000b, &P1OUT      ; clear P1.6 (green off)

Wait        mov.w   #1834, R15              ; load R15 with value for delay
L1          dec.w   R15                     ; decrement R15
            jnz     L1                      ; if R15 is not zero jump to L1
            jmp     Mainloop                ; jump to the Mainloop label
                                            

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
            
