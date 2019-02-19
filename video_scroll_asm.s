
	;; Include macros to set raster IRQs.
	.include "irqutil.s"

	;; Include macros to copy memory segments etc.
	.include "memutils.s"
	
	;; Make symbols from other files visible.
	.autoimport +
	
	;; Functions to scroll the text screen by 1 pixel.
	.export _video_backup_colorram_task
	.export _video_copy_buf1_buf2_task
	.export _video_copy_buf2_buf1_task
	.export _video_install_colorram_irq_task
	.export _video_switch_buffer1_to_2_task
	.export _video_switch_buffer2_to_1_task
	.export _hardware_scroll_task1
	.export _hardware_scroll_task2
	.export _hardware_scroll_task3
	.export _hardware_scroll_task4
	.export _hardware_scroll_task5
	.export _hardware_scroll_task6
	.export _hardware_scroll_task7
	.export video_buffer1
	.export video_buffer2
	.export current_buffer
	.export colorram_buffer
	
	.debuginfo on
	
videoram_asm = $e400 		; RAM address of the screen
colorram_asm = $d800 		; RAM address of the color ram.

	video_buffer1 = $e400
	video_buffer2 = $e800
	
scroll_colorram = 1		; Scroll also the color ram.

current_buffer:			; Flag to indicate the active buffer
	.byte 1			; 0 = buffer 1, !0 = buffer 2

current_frame:			; Counter for the frames, so we can scroll
	.byte 0			; every x frames
	
	;; Function to save the colorram to the buffer with an
	;; offset of 40 characters
	;;
colorram_buffer:
	.res 1000
	
_video_backup_colorram_task:
	ldx #240
cont_backup_colorram:	
	lda colorram_asm-1,x
	sta colorram_buffer-1+40,x
	lda colorram_asm-1+240,x
	sta colorram_buffer-1+240+40,x
	lda colorram_asm-1+480,x
	sta colorram_buffer-1+480+40,x
	lda colorram_asm-1+720,x
	sta colorram_buffer-1+720+40,x
	dex
	bne cont_backup_colorram
	rts
	
	;;
	;; 3rd version of the colorram scroller IRQ
	;;
scroll_color_ram3:
	
	start_irq_routine

	;; Just copy the colorram buffer back to the colorram
	;; I copy line by line here, so I can copy from the higher
	;; address to the lower and avoid the additional cp* upper_limit
	;; operation.
	.repeat 25,current_line
	copy_memory_block colorram_buffer+current_line*40,colorram_asm+current_line*40,40
	.endrepeat

	;;
	;; Install raster irq again
	;;
	set_irq_routine 10,_irq_scheduler

	end_irq_routine
	
	;; Macro to set the video ram offset
	.macro set_video_address addy
	lda $d018
	and #$0f
	ora #(addy*16)
	sta $d018
	.endmacro
	
	;;
	;; Tasks to do hardware scrolling (up to 7 pixels).
	;; I use 1 routine per scroll pos. More mem, but faster.
	;;

	.macro set_hardware_scrollpos_task scrollpos
	lda $d011
	and #$f8
	ora #scrollpos
	sta $d011
	.endmacro
	
_hardware_scroll_task1:
	set_hardware_scrollpos_task 1
	rts
	
_hardware_scroll_task2:
	set_hardware_scrollpos_task 2
	rts
	
_hardware_scroll_task3:
	set_hardware_scrollpos_task 3
	rts
	
_hardware_scroll_task4:
	set_hardware_scrollpos_task 4
	rts
	
_hardware_scroll_task5:
	set_hardware_scrollpos_task 5
	rts
	
_hardware_scroll_task6:
	set_hardware_scrollpos_task 6
	rts
	
_hardware_scroll_task7:
	set_hardware_scrollpos_task 7
	rts
	
	;;
	;; Tasks to switch buffers
	;;
_video_switch_buffer1_to_2_task:
	set_hardware_scrollpos_task 0 ; Reset scrollpost.
	;; Switch from buffer 1 to 2
	;; set_video_adr 10*1K
	set_video_address 10
	lda #1
	sta current_buffer
	rts

_video_switch_buffer2_to_1_task:
	set_hardware_scrollpos_task 0 ; Reset scrollpost.
	;; Switch from buffer 2 to 1
	;; set_video_adr 9*1K
	set_video_address 9
	lda #0
	sta current_buffer
	rts

	;;
	;; Tasks to copy the buffers to each other
	;; These are just optimized for speed. Not meant
	;; to be splitted in several tasks.
	;;
	.macro copy_video_buf buf1,buf2
	.local cont_copy
	ldx #240
cont_copy:	
	lda buf1-1,x
	sta buf2-1+40,x
	lda buf1-1+240,x
	sta buf2-1+40+240,x
	lda buf1-1+480,x
	sta buf2-1+40+480,x
	lda buf1-1+720,x
	sta buf2-1+40+720,x
	dex
	bne cont_copy
	.endmacro
	
_video_copy_buf1_buf2_task:
	copy_video_buf video_buffer1,video_buffer2
	rts

_video_copy_buf2_buf1_task:
	copy_video_buf video_buffer2,video_buffer1
	rts

	;;
	;; Task to install coloram scroll routine
	;; as raster IRQ routine
_video_install_colorram_irq_task:	
	set_irq_routine 56,scroll_color_ram3
	rts
