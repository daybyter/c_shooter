	;;  Move the player sprite

	.import _shot_fire_asm
	
	.export _player_move_asm

	;; Load the sprite as sprite 0
	sprite_index = 0
	
	joystick2 = $dc00
	joy_up 		= 1
	joy_down 	= 2
	joy_left 	= 4
	joy_right 	= 8
	joy_fire 	= 16

	player_xpos 	= $d000
	player_highbit	= $d010
	player_ypos	= $d001
	
_player_move_asm:
	
	lda joystick2
	tay			; save the joystick position
	and #joy_left
	bne not_left 		; no left signal
	lda #(1<<sprite_index)
	bit player_highbit	; Test, if we are right of pos 255
	bne rightof255
	lda player_xpos
	cmp #22			; if player left of 22 pixels
	beq not_right		; don't move further to the left
	dec player_xpos		; move left 1 pixel
	jmp not_right		; don't check for right, if left was pressed
rightof255:
	dec player_xpos 	; move left
	bpl not_right		; check for underflow
	lda player_highbit	; yes => delete high bit
	and #(255-(1<<sprite_index))
	sta player_highbit
	jmp not_right
not_left:
	tya			; restore joystick pos
	and #joy_right		; check for right switch
	bne not_right		; no right signal
	lda #(1<<sprite_index)
	bit player_highbit
	bne rightof255_2
	inc player_xpos
	bne not_right		; Check for overflow
	lda player_highbit	; Set high bit
	ora #(1<<sprite_index)
	sta player_highbit
	jmp not_right
rightof255_2:			; we are right of pixel 255
	lda player_xpos
	cmp #(324-256)
	beq not_right
	inc player_xpos
not_right:
	tya			; get joystick pos again
	and #joy_up
	bne not_up
	lda player_ypos
	cmp #100
	beq not_down		; Don't check for down, if
	dec player_ypos		; up was pressed.
	jmp not_down
not_up:
	tya
	and #joy_down
	bne not_down
	lda player_ypos
	cmp #220
	beq not_down
	inc player_ypos
not_down:

return:	
	rts			; end player movement
	
