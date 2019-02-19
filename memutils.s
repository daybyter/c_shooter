
	;; Macro to copy a part of the memory to another
	;; location
	.macro copy_memory_block src_adr,target_adr,nbytes
	.local copy_loop
	
	ldx #nbytes

	;; The loop copies from end to start.
	;; Important for overlapping memory areas! 
copy_loop:
	lda src_adr-1,x	
	sta target_adr-1,x
	dex
	bne copy_loop
	
	.endmacro


	;;
	;; Macro to copy a memory block to another location.
	;; This version copies forward, which is important
	;; for some methods (copy screen while racing the beam).
	;;
	.macro copy_memory_block_forward src_adr,target_adr,nbytes
	.local copy_loop

	ldx #0

	;; The loop copies from end to start.
	;; Important for overlapping memory areas! 
copy_loop:
	lda src_adr,x	
	sta target_adr,x
	inx
	cpx #nbytes
	bne copy_loop
	
	.endmacro
