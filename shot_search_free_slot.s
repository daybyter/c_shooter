	;; This method searches for a free slot
	;; for a new shot.
	;;
	;; It searches the shot array top -> down!
	;; 
	;; @return a shot index in the accumulator or $ff, if
	;;         no free shot is available.

	.export _shot_search_free_slot

	.import _multiply_by5_table
	.import _shots		; The array of shot Y coordinates
	
	MAX_SHOTS = 6

_shot_search_free_slot:
	ldx #0
cont_search:
	ldy _multiply_by5_table,x ; y = 5*x now. 5 = sizeof(VirtualSprite).
	lda _shots+3,y		; load shots[y].y
	beq shot_found		; Check, if the y coordinate is 0
	inx
	cpy #MAX_SHOTS
	bne cont_search		; Check next slot.
	lda #$ff		; No free slot found
	rts			; => return $ff
shot_found:
	txa			; move the result to the accumulator
	rts			; and return it.
