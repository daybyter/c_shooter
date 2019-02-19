	;; 
	;; Map a virtual sprite to an actual hardware sprite.
	;;

	;; 
	;; @param virtualSprite The virtual sprite to map
	;; @param sprite_index The index of the hardware sprite to map to (0-7)
	;; 
	.macro sprite_map_virtual_2_hardware virtualSprite,sprite_index
	... Store the sprite data in the vic registers
	lda virtualSprite+offsetof(VirtualSprite,y)
	sta $d000+1+(2*sprite_index)
	
	.endmacro
