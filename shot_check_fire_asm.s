
	;; Assembler method to create a new shot
	
	.export _shot_check_fire_asm

	.import _flyingShots 	; Get the number of already fired shots.
	.import _shot_search_free_slot ; Method to search for a free shot slot
	
	max_shots = 6
	joystick2 = $dc00
	joy_fire  = 16
	
_shot_check_fire_asm:
	
	;; Check for fire
	lda joystick2
	and #joy_fire
	bne return
	
	lda _flyingShots	; Check, if we reached the max number of shots
	cmp max_shots		; yes => 
	beq return		; do nothing

	;;  Search for a free shot slot
	jsr _shot_search_free_slot
	cmp #$ff		; should never happen, if _flyingShots is
	beq return		; correctly updated all the time.

	;; Actually create the new shot.
	;; Set it's coordinates to the coordinates of the player sprite.
	inc $d020		; just a dummy

	inc _flyingShots	; Count this shot.

return:	
	rts
