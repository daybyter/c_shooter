	;; 
	;; A small util file to do some math ops faster with the 6502
	;;

	.export _multiply_by5_table

	;; 
	;; A table with results of a multiplication by 5
	;; So it holds constants 0,5,10,15,...
	;; 
_multiply_by5_table:
	.repeat 51,multiplier
	.byte multiplier*5
	.endrepeat
