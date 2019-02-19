
	;; 
	;; Set a interrupt routine
	;; 
	.macro set_irq_routine rasterline,address

	;; Tell vic2 to generate raster IRQ
	lda #$01
	sta $d01a
	
	;; Just use the first 255 rasterlines.
	lda #rasterline
	sta $d012
	lda $d011		; so just delete the
	and #$7f               	; 8th bit of $d011
	sta $d011
	
	;;  Set the address of the irq routine
	lda #<address
	sta $fffe
	lda #>address
	sta $ffff
	.endmacro

	;;
	;; Start a IRQ routine
	;;
	.macro start_irq_routine
	;; Save the registers, so we can use them
	;; in the IRQ routine.
	pha
	tya
	pha
	txa
	pha

	;; Clear IRQ condition
	lda #$ff
	sta $d019
	.endmacro

	;;
	;; End an IRQ routine
	;;
	.macro end_irq_routine

	;; Restore registers from stack
	pla
	tax
	pla
	tay
	pla

	;;  Return from interrupt to the main
	;;  program.
	rti
	.endmacro

	
