	;;
	;; Scheduler for main tasks in the game.
	;; It is called via rasterline IRQ
	;; 50 times per second (PAL).
	;;

	.export _scheduler_init	; init the scheduler.
	.export _scheduler_task  ; call the next tasks.

	.import _hardware_scroll_task1
	.import _hardware_scroll_task2
	.import _hardware_scroll_task3
	.import _hardware_scroll_task4
	.import _hardware_scroll_task5
	.import _hardware_scroll_task6
	.import _hardware_scroll_task7
	.import	_player_move_asm
	.import _playground_unpack_tile_line
	.import _shot_check_fire_asm
	.import _shot_move_task
	.import _video_backup_colorram_task
	.import _video_copy_buf1_buf2_task
	.import _video_copy_buf2_buf1_task
	.import _video_install_colorram_irq_task
	.import _video_switch_buffer1_to_2_task
	.import _video_switch_buffer2_to_1_task

	.linecont +
	
	;; Address of the counter for the current frame.
	;; I use a zeropage address here to speed up the code.
	;; It counts 2 cycles = 64 frames. 1 cycle with each buffer.
framecounter = $0002

_scheduler_init:
	lda #0
	sta framecounter
	rts
	
_scheduler_task:
	ldy framecounter 	; Inc the 
	iny			; framecounter
	tya			
	and #63			;
	sta framecounter	; update the framecounter
	tay
	lda tasklist_low,y	; Create jump to task function
	sta taskjump+1
	lda tasklist_high,y
	sta taskjump+2
taskjump:
	jmp $ffff		; Overwritten by actual task address.
				; Jump to the actual task and then
				; return to main program from there.


	;;
	;;  This is just a dummy task for empty slots.
	;;
dummy_task:
	rts			; Just do nothing and return.

	;; A macro to create a sequence of tasks, that are called
	;; one after the other. Max task count is 6 here
	.macro taskSequence task1,task2,task3,task4,task5,task6
	.ifblank task1 		; if there are no more tasks
	.exitmacro		; stop here.
	.else			; more tasks to add
	.ifnblank task2 	; if this is not the last task
	jsr task1		; call it and return from it.
	.else			; this is the last task,
	jmp task1		; so return to the main program from it.
	.endif
	taskSequence task2,task3,task4,task5,task6 ; process the other tasks.
	.endif
	.endmacro

	;; Create task sequences for the taskList
task00:	taskSequence _video_switch_buffer2_to_1_task,_player_move_asm
task01:	taskSequence _player_move_asm,_shot_check_fire_asm
task02:	taskSequence _player_move_asm,_shot_move_task
task03:	taskSequence _player_move_asm
task04:	taskSequence _hardware_scroll_task1,_player_move_asm
task05:	taskSequence _player_move_asm
task06:	taskSequence _player_move_asm,_shot_move_task
task07:	taskSequence _player_move_asm
task08:	taskSequence _hardware_scroll_task2,_player_move_asm
task09:	taskSequence _player_move_asm,_shot_check_fire_asm
task10:	taskSequence _player_move_asm,_shot_move_task
task11:	taskSequence _player_move_asm
task12:	taskSequence _hardware_scroll_task3,_player_move_asm
task13:	taskSequence _player_move_asm
task14:	taskSequence _player_move_asm,_shot_move_task
task15:	taskSequence _player_move_asm
task16:	taskSequence _hardware_scroll_task4,_player_move_asm
task17:	taskSequence _player_move_asm,_shot_check_fire_asm
task18:	taskSequence _player_move_asm,_shot_move_task
task19:	taskSequence _player_move_asm
task20:	taskSequence _hardware_scroll_task5,_player_move_asm
task21:	taskSequence _player_move_asm
task22: taskSequence _player_move_asm,_shot_move_task
task23:	taskSequence _player_move_asm
task24:	taskSequence _hardware_scroll_task6,_player_move_asm
task25:	taskSequence _player_move_asm,_shot_check_fire_asm
task26:	taskSequence _video_copy_buf1_buf2_task,_player_move_asm,_shot_move_task
task27:	taskSequence _player_move_asm,_playground_unpack_tile_line
task28:	taskSequence _hardware_scroll_task7,_player_move_asm
task29:	taskSequence _player_move_asm
task30:	taskSequence _video_backup_colorram_task,_player_move_asm,_shot_move_task
task31:	taskSequence _player_move_asm,_video_install_colorram_irq_task
task32:	taskSequence _video_switch_buffer1_to_2_task,_player_move_asm
task33:	taskSequence _player_move_asm,_shot_check_fire_asm
task34:	taskSequence _player_move_asm,_shot_move_task
task35: taskSequence _player_move_asm
task36:	taskSequence _hardware_scroll_task1,_player_move_asm
task37:	taskSequence _player_move_asm
task38:	taskSequence _player_move_asm,_shot_move_task
task39:	taskSequence _player_move_asm
task40:	taskSequence _hardware_scroll_task2,_player_move_asm
task41:	taskSequence _player_move_asm,_shot_check_fire_asm
task42:	taskSequence _player_move_asm,_shot_move_task
task43:	taskSequence _player_move_asm
task44:	taskSequence _hardware_scroll_task3,_player_move_asm
task45:	taskSequence _player_move_asm
task46:	taskSequence _player_move_asm,_shot_move_task
task47:	taskSequence _player_move_asm
task48:	taskSequence _hardware_scroll_task4,_player_move_asm
task49:	taskSequence _player_move_asm,_shot_check_fire_asm
task50:	taskSequence _player_move_asm,_shot_move_task
task51:	taskSequence _player_move_asm
task52:	taskSequence _hardware_scroll_task5,_player_move_asm
task53:	taskSequence _player_move_asm
task54:	taskSequence _player_move_asm,_shot_move_task
task55:	taskSequence _player_move_asm
task56:	taskSequence _hardware_scroll_task6,_player_move_asm
task57:	taskSequence _player_move_asm,_shot_check_fire_asm
task58:	taskSequence _video_copy_buf2_buf1_task,_player_move_asm,_shot_move_task
task59:	taskSequence _player_move_asm,_playground_unpack_tile_line
task60:	taskSequence _hardware_scroll_task7,_player_move_asm
task61:	taskSequence _player_move_asm
task62:	taskSequence _video_backup_colorram_task,_player_move_asm,_shot_move_task
task63:	taskSequence _player_move_asm,_video_install_colorram_irq_task
	
	;; The list of tasks to be executed.
	;; 64 methods to be executed.
	.define taskList \
	task00,task01,task02,task03,task04,task05,task06,task07,\
	task08,task09,task10,task11,task12,task13,task14,task15,\
	task16,task17,task18,task19,task20,task21,task22,task23,\
	task24,task25,task26,task27,task28,task29,task30,task31,\ 
	task32,task33,task34,task35,task36,task37,task38,task39,\
	task40,task41,task42,task43,task44,task45,task46,task47,\
	task48,task49,task50,task51,task52,task53,task54,task55,\
	task56,task57,task58,task59,task60,task61,task62,task63
	
	;;  Lowbytes of addresses
tasklist_low:
	.lobytes taskList
	
tasklist_high:
	.hibytes taskList
	

	
