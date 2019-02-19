	;; Code to unpack the next character line with
	;; background graphics

	;; Make symbols from other files visible.
	.autoimport +
	
	.export _playground_unpack_tile_line

	;; Unpack the next line of background graphics.
_playground_unpack_tile_line:	

	;; This is just a dummy for testing!
	ldx #40
	lda current_buffer
	bne write_buffer1 	; buffer 2 used at the moment?
				; no => write to buffer 2
	lda #2			; Screencode 'b'
continue_write2:	
	sta video_buffer2-1,x
	dex
	bne continue_write2
	;; Write a red background to the color ram.
	lda #2
	ldx #40
continue_write_colorram:	
	sta colorram_buffer-1,x
	dex
	bne continue_write_colorram
	rts
write_buffer1:
	lda #1
continue_write1:	
	sta video_buffer1-1,x
	dex
	bne continue_write1
	;; Write a green background to the color ram.
	lda #5
	ldx #40
continue_write_colorram2:	
	sta colorram_buffer-1,x
	dex
	bne continue_write_colorram2
	rts
	
