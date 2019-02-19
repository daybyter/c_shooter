
	;; Make symbols from other files visible.
	.autoimport +
	
	;; Function to play level 1 of the game.
	.export _play_level0
	.export _irq_scheduler
	
	;; Include macros to set raster IRQs.
	.include "irqutil.s"
	
		;;  Activate ram at $e400
	.macro disable_kernal
	lda $0001
	and #$fd
	sta $0001	
	.endmacro

	;;  Activate rom at $e000
	.macro enable_kernal
	lda $0001
	ora #$02
	sta $0001
	.endmacro
	
	;; Play level 1 of the game
_play_level0:
	sei
	disable_kernal

	;; Set 24 row mode
	lda $d011
	and #247
	sta $d011

	;;
	;; Init scheduler
	;;
	jsr _scheduler_init
	
	;;
	;; Install raster irq
	;;
	set_irq_routine 10,_irq_scheduler
	cli 			; enable raster irq
	
continue_play:	
	
	jmp continue_play 	; Continue the level
	
	;; Set 25 row mode
	lda $d011
	ora #8
	sta $d011
	
	enable_kernal
	cli
	rts
	
	;;
	;; Rasterline routine for task scheduling
	;; 
_irq_scheduler:	
	start_irq_routine
	jsr _scheduler_task
	end_irq_routine

