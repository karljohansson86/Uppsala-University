##############################################################################
#
#  KURS: 1DT093 våren 2015
#
#   VAD: Inlämningsuppift: Arrayer, Strängar, Loopar och Subrutiner.
#
# DATUM:
#
#  NAMN: Karl_Johansson
#
#  NAMN:
#
##############################################################################

	.data

ARRAY_SIZE:
	.word	10	# Change here to try other values (less than 10)
FIBONACCI_ARRAY:
	.word	1, 1, 2, 3, 5, 8, 13, 21, 34, 55
STR_str:
	.asciiz "Hunden, Katten, Glassen"

	.globl DBG
	.text

##############################################################################
#
# DESCRIPTION:  For an array of integers, returns the total sum of all
#		elements in the array.
#
# INPUT:        $a0 - address to first integer in array.
#		$a1 - size of array, i.e., numbers of integers in the array.
#
# OUTPUT:       $v0 - the total sum of all integers in the array.
#
##############################################################################
integer_array_sum:

DBG:	##### DEBUGG BREAKPOINT ######

  addi    $v0, $zero, 0     # Initialize Sum to zero.
	add	$t0, $zero, $zero			# Initialize array index i to zero.

for_all_in_array:
	#### Append a MIPS-instruktion before each of these comments

	beq $t0,$a1, end_for_all 		# Done if i == N

	sll $t1,$t0,2					# 4*i
	add $t1,$a0,$t1				# address = ARRAY + 4*i
	lw $t1,0($t1)					# n = A[i]
	add $v0,$v0,$t1     	# Sum = Sum + n
	addi $t0,$t0,1      	# i++

  j for_all_in_array		# next element

end_for_all:
 jr	$ra									# Return to caller.

##############################################################################
#
# DESCRIPTION: Gives the length of a string.
#
#       INPUT: $a0 - address to a NUL terminated string.
#
#      OUTPUT: $v0 - length of the string (NUL excluded).
#
#    EXAMPLE:  string_length("abcdef") == 6.
#
##############################################################################
string_length:
	add $v0, $zero,0 				# set sum and index to zero
sl_loop:
	#### Write your solution here ####
	add $t0, $a0, $v0				#copy starting/next adress to $t0
	lb $t0,0($t0)						#load first/current byte
	beq $t0,$zero, sl_exit	#check if value in $t0 is zero meaning end of string,
													# if so jump to sl_exit
	addi $v0,$v0,1					#increase value of sum and index
	j sl_loop								#loop
sl_exit: jr	$ra

##############################################################################
#
#  DESCRIPTION: For each of the characters in a string (from left to right),
#		call a callback subroutine.
#
#		The callback suboutine will be called with the address of
#	        the character as the input parameter ($a0).
#
#        INPUT: $a0 - address to a NUL terminated string.
#
#		$a1 - address to a callback subroutine.
#
##############################################################################
string_for_each:
	add $t1,$zero,0					# set index to zero

	addi	$sp, $sp, -4			# PUSH return address to caller
	sw	$ra, 0($sp)

	#### Write your solution here ####
	sfe_loop:
	add $t0, $a0, $t1				#copy starting/next adress to $t0
	lb $t2,0($t0)						#load first byte
	beq $t2,$zero, sfe_exit	#check if value in $t0 is zero meaning end of string
													# if so jump to sfe_exit

	addi	$sp, $sp, -8			# PUSH
	sw	$a0, 0($sp)					# store starting point/adress of array/string
	sw $t1, 4($sp)					# store counter/index

	la $a0, 0($t0)					#load the current adress to $a0(first argument for next function)
	jal $a1

	lw	$a0, 0($sp)					# load starting adress
	lw $t1, 4($sp)					# load counter

	addi	$sp, $sp, 8
	addi $t1,$t1,1					#increase counter

	j sfe_loop

	sfe_exit:
	lw	$ra, 0($sp)					# Pop return address to caller
	addi	$sp, $sp, 4

	jr	$ra

##############################################################################
#
#  DESCRIPTION: Transforms a lower case character [a-z] to upper case [A-Z].
#
#        INPUT: $a0 - address of a character
#
##############################################################################
to_upper:

	#### Write your solution here ####
	lb $t0, 0($a0)				#load byte at adress $a0 into reg. $t0

	#check if the character is between smaller characters a-z, if not
	#jump to end of function
	blt $t0,'a',tu_exit		#check if less than a, meaning already uppercase
	bgt $t0,'z',tu_exit		#check if greater than z, meaning

	addi $t0,$t0,-32			#subtract -32 to get to the uppercase character
	sb $t0, 0($a0)				#store updated bit at adress $a0

	tu_exit:
	jr	$ra




##############################################################################
#
# Description: Reverse a string
#
# Input: $a0 - address to a NUL terminated string.
#
##############################################################################
reverse_string:

addi	$sp, $sp, -4			# PUSH return address to caller
sw	$ra, 0($sp)

jal string_length
addi $t0, $zero,0				# set index to zero
addi $v0,$v0,-1					# set index to lenght of string -1 due to
												# starting value of front index is zero

rs_loop:

blt $v0,$t0, rs_exit		# check if back index is less than front index
												# if true, end loop

add $t1, $a0, $t0 #adress of current front char
add $t3,$a0,$v0		#adress of current back char

lb $t2, 0($t1)		#load first char
lb $t4, 0($t3)		#load last char

sb $t2, 0($t3)		#store first char in back char adress
sb $t4, 0($t1)		#store last char in front char adress

addi $t0,$t0,1		# incement front index
addi $v0,$v0,-1		# decrement back index

j rs_loop

rs_exit:

lw	$ra, 0($sp)					# Pop return address to caller
addi	$sp, $sp, 4
jr	$ra

##############################################################################
##############################################################################
##
##	  You don't have to change anyghing below this line.
##
##############################################################################
##############################################################################


##############################################################################
#
# Strings used by main:
#
##############################################################################

	.data

NLNL:	.asciiz "\n\n"

STR_sum_of_fibonacci_a:
	.asciiz "The sum of the "
STR_sum_of_fibonacci_b:
	.asciiz " first Fibonacci numbers is "

STR_string_length:
	.asciiz	"\n\nstring_length(str) = "

STR_for_each_ascii:
	.asciiz "\n\nstring_for_each(str, ascii)\n"

STR_for_each_to_upper:
	.asciiz "\n\nstring_for_each(str, to_upper)\n\n"

STR_reverse_string:
.asciiz "\n\nreverse_string\n\n"

	.text
	.globl main

##############################################################################
#
# MAIN: Main calls various subroutines and print out results.
#
##############################################################################
main:
	addi	$sp, $sp, -4	# PUSH return address
	sw	$ra, 0($sp)

	##
	### integer_array_sum
	##

	li	$v0, 4
	la	$a0, STR_sum_of_fibonacci_a
	syscall

	lw 	$a0, ARRAY_SIZE
	li	$v0, 1
	syscall

	li	$v0, 4
	la	$a0, STR_sum_of_fibonacci_b
	syscall

	la	$a0, FIBONACCI_ARRAY
	lw	$a1, ARRAY_SIZE
	jal 	integer_array_sum

	# Print sum
	add	$a0, $v0, $zero
	li	$v0, 1
	syscall

	li	$v0, 4
	la	$a0, NLNL
	syscall

	la	$a0, STR_str
	jal	print_test_string

	##
	### string_length
	##

	li	$v0, 4
	la	$a0, STR_string_length
	syscall

	la	$a0, STR_str
	jal 	string_length

	add	$a0, $v0, $zero
	li	$v0, 1
	syscall

	##
	### string_for_each(string, ascii)
	##

	li	$v0, 4
	la	$a0, STR_for_each_ascii
	syscall

	la	$a0, STR_str
	la	$a1, ascii
	jal	string_for_each

	##
	### string_for_each(string, to_upper)
	##

	li	$v0, 4
	la	$a0, STR_for_each_to_upper
	syscall

	la	$a0, STR_str
	la	$a1, to_upper
	jal	string_for_each

	la	$a0, STR_str
	jal	print_test_string


	##
	### reverse_string
	##
	li	$v0, 4
	la	$a0, STR_reverse_string
	syscall

	la	$a0, STR_str
	jal	reverse_string

	la	$a0, STR_str
	jal	print_test_string
	##


	lw	$ra, 0($sp)	# POP return address
	addi	$sp, $sp, 4


	jr	$ra

##############################################################################
#
#  DESCRIPTION : Prints out 'str = ' followed by the input string surronded
#		 by double quotes to the console.
#
#        INPUT: $a0 - address to a NUL terminated string.
#
##############################################################################
print_test_string:

	.data
STR_str_is:
	.asciiz "str = \""
STR_quote:
	.asciiz "\""

	.text

	add	$t0, $a0, $zero

	li	$v0, 4
	la	$a0, STR_str_is
	syscall

	add	$a0, $t0, $zero
	syscall

	li	$v0, 4
	la	$a0, STR_quote
	syscall

	jr	$ra


##############################################################################
#
#  DESCRIPTION: Prints out the Ascii value of a character.
#
#        INPUT: $a0 - address of a character
#
##############################################################################
ascii:
	.data
STR_the_ascii_value_is:
	.asciiz "\nAscii('X') = "

	.text

	la	$t0, STR_the_ascii_value_is

	# Replace X with the input character

	add	$t1, $t0, 8	# Position of X
	lb	$t2, 0($a0)	# Get the Ascii value
	sb	$t2, 0($t1)

	# Print "The Ascii value of..."

	add	$a0, $t0, $zero
	li	$v0, 4
	syscall

	# Append the Ascii value

	add	$a0, $t2, $zero
	li	$v0, 1
	syscall


	jr	$ra
