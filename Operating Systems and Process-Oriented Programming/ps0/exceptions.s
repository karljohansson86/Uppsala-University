# ---------------------------------------------------------------------------
# PURPOSE
# 
# Demonstrating basic exception handling.
#
# BACKGROUND
# 
# Exceptions are used to signal internal faults in the program such as 
# division by zero, invalid memory access, arithmetic overflow etc). 
#
# A user progam executes in user mode (text segment). When an exception
# happens, control is autmatically transferred to the
# exception handler executing in kernel mode (ktext segment). 
# 
# After the exception is handeld, control is transfered back to the user 
# program (user mode). 
#
# When an exception occurs, the address of the instruction causing the 
# exception is automatically saved in register $14 (Exception Program Counter, 
# EPC) in coprocessor 0. Each instruction is four bytes, so when returning 
# from the exception handler, 4 must be addedd to the value stored in EPC in 
# order to not re-execute the very same instruction causing the exception.
#
#
# PROGRAM DESCRIPTION
# 
# A small user level program (main) triggers an arithmetic overflow exception,
# an address error exception and a trap exception. A custom exception handler 
# handles all exceptions by printing the exception code and the name of the 
# exception. 
# 
# 
# AUTHOR
# 
# Karl Marklund <karl.marklund@it.uu.se>
#
# 
# HISTORY
#
# 2016-01-04
#
# First version. A simplified and modified version of the SPIM
# default exception handler with additional comments. 
# 
# Link to the default SPIM exception handler:
#
# https://sourceforge.net/p/spimsimulator/code/HEAD/tree/CPU/exceptions.s
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# MAIN
# 
# Text segment (code) for the user level program.
# ---------------------------------------------------------------------------

        .text
        .globl main

main:

	# A huge interger 0x7fffffffe = [32 bits] = 0111 1111 1111 1111 1111 1111 1111 1110
	
        li $a0, 0x7ffffffe	# NOTE: a pseudo instruction using the $at register. 
        
	# Add one.
	  
        addi $v0, $a0, 1	# $v0 = 0x7fffffff
       	
       	# $v0 now holds the largets 32 bit positive sigend (two's complement) integer. 
      
        # 0x7fffffff = [32 bits] = 0111 1111 1111 1111 1111 1111 1111 1111
    
    
      	##### ARITHMETIC OVERFLOW EXCEPTION #####
	       
      	# Adding one to 0x7ffff triggers an arithmetic overflow exception 
      	# (exception code 12).
        
        addi $t0, $v0, 1	# Arithmetic overflow!!!


	##### ADDRESS ERROR EXCEPTION (STORE) #####
	
        # Trying to store data on illegal memory address triggers a address
        # error exception (exception code 5).
        
        sw $a0, 124($zero)      # Address error!!!

	
	##### TRAP EXCEPTION #####
        
        # Use the teqi (Trap EQual Immediate) instruction to trigger a trap
        # exception (exception code 13).
        
	teqi $zero, 0		# Trap!!!


      	# Terminate normally.
	
  	li   $v0, 10   			
   	syscall      
   	

# ---------------------------------------------------------------------------
# EXCEPTION HANDLER
#
# Kernel text segment, i.e., code for the exception handler.
#
# Overall structure of the exception handler:
#
# 1) Save contents in any registers (except $k0 and $k1) used by the
#    exception handler. $k0 and $k1 are not supposed to be used by user
#    level code. 	
#	
# 2) Examine the cause register to extract the exception code.
# 
# 3) Print the exception code. 
#	 
# 4) Print the name of exception. 
#	 
# 5) Restore the contents of the registers saved in step 1. 
#	
# 6) Resume user level execution (eret instruction) after adding 4 to EPC in
#    order to skip the offending instruction. 
# ---------------------------------------------------------------------------
        
        .ktext 0x80000180  # This is the exception vector address for MIPS32.
        
__kernel_entry_point:

	##### STEP 1 - Save registers #####
	
	## Must save (and later restore) any register we use when handling
	## the exception.
        
        ## Can't trust $sp (may be corrupted) and consequently the stack 
        ## cannont be used to store data. Instead, the register values 
        ## of the program causing the exception are stored to memory (.kdata).

        ## The registers $k0 and $k1 should only be used in the exception
        ## handler and not by used level code. Therefore $k0 and $k1 can be 
        ## used without saving and restoring.

	## Register $at is used by various pseudo instructions. Both the user 
	## mode program and the kernel mode exception handler may use pseudo 
	## instructions, hence we must first save the value in $at before we
	## can use any pseudo instructions in the exception handler. 
	
	## NOTE: the sw instruction is a pseudo instruction using $at. 
	## To save the current value of $at we first copy the value to $k0. 
	## Next we can use sw to save the value of $k0 (value of $at) to 
	## memory using the sw instruction.
	
        # .set noat             # SPIM - Turn of warnings for using the $at register.
        move $k0, $at		# Copy value of $at to $k0.
       	sw $k0, __at		# Save value of $at to memory.
        # .set at               # SPIM - Turn on warnings for using the $at register.

        sw $v0, __v0		# Save value of $v0.
        sw $a0, __a0		# Save value of $a0
        
        ## NOTE: In the reminder of the exception handler, only the following 
        ## registers may be used: $k0, $k1, $v0, $a0 and $at (should only be 
        ## be used by pseudo instructions). 

	
	##### STEP 2 - Extract exception code #####
	
	mfc0 $k0, $13           	# Cause register.
        srl $k1, $k0, 2         	# Shift two steps to the right.
        andi $k1, $k1, 0x0000001f	# Set bits 5-31 to zero (bit masking).
        
        
        ##### STEP 3 - Print exception code ####
        
        # Print "Exception code: ".

        li $v0, 4               # Syscall 4 (print_str).
        la $a0, msg
        syscall

	# Print exception code value. 
	
        move $a0, $k1
        li $v0, 1               # Syscall 1 (print_int).
        syscall

        
        ##### STEP 4 - Print exception name #####
        
        # Names of exceptions are storred in an array of pointers to strings 
        # ordered by exception code. Each element (pointer) of the array is 
        # four bytes. The offset of the name for exception code N is therefore N*4.

	# Multiply the exception code by four by shifting two steps to the left. 
	
	sll $k1, $k1, 2

	# Load address of exception name from array.
        lw $a0 __name_array($k1)      
        
        li $v0, 4               # Syscall 4 (print_str).
        syscall

        ## Print new line.

        li $v0, 4               # Syscall 4 (print_str).
        la $a0, NL
        syscall

	
	##### STEP 5 - Restore registers #####

        lw $v0, __v0
        lw $a0, __a0

        # .set noat		# SPIM - Turn of warnings for using the $at register.
        lw $at, __at           	# Restore $at
        # .set at		# SPIM - Turn on warnings for using the $at register.


        # mtc0 $0, $13       	# Clear the Cause register.

        # mfc0 $k0, $12         # Get Status register.
        # ori $k0, 0x1          # Enable interrupts.
        # mtc0 $k0, $12

        ##### STEP 6 - Resume user level execution #####
        
        ## Get value of EPC (Address of instruction causing the exception).
        
        mfc0 $k0, $14
        
        # Skip offending instruction by adding 4 to the value stored in EPC. 
        # Otherwise the same instruciton would be executed again causing the same 
        # exception again.
        
        addi $k0, $k0, 4        
                               
        # Update EPC in coprocessor 0.
        
        mtc0 $k0, $14
            
	# Use the eret (Exception RETurn) instruction to set the program counter
	# (PC) to the value saved in the ECP register (register 14 in coporcessor 0).
		      
        eret                    


# ---------------------------------------------------------------------------
# KERNEL DATA SEGMENT
# 
# Data used by the exception handler (kernel).
# ---------------------------------------------------------------------------
      
      	.kdata

# Storage for saving registers used by the exception handler. 

__at:	.word 0
__v0:   .word 0
__a0:   .word 0

# Strings used to print the exception code (number) and name (string).

msg:    .asciiz "Exception code: "
NL:     .asciiz "\n"

# One string for each of the exception codes 0-31.

__e0_:	.asciiz "  [Interrupt] "
__e1_:	.asciiz	"  [TLB]"
__e2_:	.asciiz	"  [TLB]"
__e3_:	.asciiz	"  [TLB]"
__e4_:	.asciiz	"  [Address error in inst/data fetch] "
__e5_:	.asciiz	"  [Address error in store] "
__e6_:	.asciiz	"  [Bad instruction address] "
__e7_:	.asciiz	"  [Bad data address] "
__e8_:	.asciiz	"  [Error in syscall] "
__e9_:	.asciiz	"  [Breakpoint] "
__e10_:	.asciiz	" [Reserved instruction] "
__e11_:	.asciiz	""
__e12_:	.asciiz	" [Arithmetic overflow] "
__e13_:	.asciiz	" [Trap] "
__e14_:	.asciiz	""
__e15_:	.asciiz	" [Floating point] "
__e16_:	.asciiz	""
__e17_:	.asciiz	""
__e18_:	.asciiz	" [Coproc 2]"
__e19_:	.asciiz	""
__e20_:	.asciiz	""
__e21_:	.asciiz	""
__e22_:	.asciiz	" [MDMX]"
__e23_:	.asciiz	" [Watch]"
__e24_:	.asciiz	" [Machine check]"
__e25_:	.asciiz	""
__e26_:	.asciiz	""
__e27_:	.asciiz	""
__e28_:	.asciiz	""
__e29_:	.asciiz	""
__e30_:	.asciiz	" [Cache]"
__e31_:	.asciiz	""

### Names of exceptions are storred in an array of pointers to strings 
### ordered by exception code. Each element (pointer) of the array is 
### four bytes. The offset of the name for exception code N is therefore N*4.

__name_array:

	.word __e0_, __e1_, __e2_, __e3_, __e4_, __e5_, __e6_, __e7_, __e8_, __e9_
	.word __e10_, __e11_, __e12_, __e13_, __e14_, __e15_, __e16_, __e17_, __e18_
	.word __e19_, __e20_, __e21_, __e22_, __e23_, __e24_, __e25_, __e26_, __e27_
	.word __e28_, __e29_, __e30_, __e31_
