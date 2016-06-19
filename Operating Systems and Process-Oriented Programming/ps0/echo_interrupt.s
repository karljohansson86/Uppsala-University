# ---------------------------------------------------------------------------
# PURPOSE
# 
# Demonstrating the use of interrupt-driven input. Compared to polling and 
# busy-waiting this is much more efficient since the CPU can do other 
# (potentially useful) work while waiting for user input.
#
# Both exceptions and interrupts transfers control from user mode, to 
# kernel mode and back to user mode again. 
#
# A user progam executes in user mode (text segment). When an interrupt
# happens, control is autmatically transferred to the
# exception handler executing in kernel mode (ktext segment). 
# 
# After the interrupt is handeld, control is transfered back to the user 
# program (user mode).
#
# Before returning from an exception, 4 must be added to the value of EPC
# in order to skip the instruction causing the exception.
# 
# When an interrupt occurs, the address of the instruction to be executed 
# next is stored in EPC. When returning rom an interrupt the value of EPC 
# must not be changed. If 4 was added to ECP before return, one instruction 
# in the program would simple never be executed most likely introducing a 
# hard to find bug in the program. 
#
# 
# BACKGROUND
# 
# Bit 1 of the Receiver Control register is the keyboard interrupt enable. 
# This bit may be both read and written by a program. The interrupt enable is
# initially 0. If it is set to 1 by a program, the terminal causes an
# interrupt at hardware level 1 whenever a character is typed and the ready
# bit becomes 1. However, for the interrupt to affect the processor, 
# interrupts must also be enabled in the status register.
# 
# Enabling receiver interrupts makes it possible for the CPU to do other work
# while waiting for user input. 
#
# Once a key is pressed a keyboard interrupt (hardware level 1) is
# generated and the value of the program counter (user level) is automatically
# saved in regiser 14 (EPC) in coprocessor 0. Next control is automatically 
# transfered to the exception handler where the interrupt is handled. 
#
# After the interrupt is handled the eret instruction is used to transfer
# control back to the instruction (user level) previously saved in EPC.
#
# 
# PROGRAM DESCRIPTION
# 
# First keyboard interrupts are enabled. 
#
# Then the program goes into aninfinite loop where a counter ($s0) is
# incremented by one for each iteration. This loop is only used to demonstrate
# that the CPU can do other (useful) work while waiting for user input. 
#
# Once a key is pressed and an interrupt will be generated and control is
# transfered to the exception handler. 
# 
# The exception handler can freely use the register $k0 and $k1 which are
# not supposed to be used by user level code. In order to not interfere with
# the running user level program, the values of any other registers used by
# the exception handler must be saved before usage. 
#
# At the begining of the exception handler the Cause register (register 13) 
# in coprocessor 0 is inspected to determine whether it is an exception or an
# interrupt.
# 
# If it is an interrupt, the Cause regiser is inspected once more
# to determine whether it is a keyboard interupt(hardware level 1) or some 
# other interrupt. 
# 
# If it was a keyboard interrupt, the ASCII value of the key
# pressed is loaded from the memory-mapped receiver data register and echoed 
# to the Run I/O console using the MARS built-in system call print_char. 
# 
# Next the values of the used registers are restored and controll is transfered 
# back to the value stored in the EPC register, i.e., back to the infinte loop.
# 
# 
# AUTHOR
# 
# Karl Marklund <karl.marklund@it.uu.se>
#
# 
# HISTORY 
# 
# 2015-12-09
# 
# First version. 
#
# 2016-01-15
#
# Reset of receiver interrupt pending (bit 8) in Cause register added at
# label __kbd_interrupt.
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# USER DATA SEGMENT
# 
# Data used by the user level program.
# ---------------------------------------------------------------------------
				
	.data

# Both SPIM and MARS simulates a memory-mapped console for input and output. 
# The console is controlled by four memory-mapped device registers. 
# Memory-mapped  means that each register appears as a special memory
# location.

# Addresses of the memory-mapped device registers. 
	
RECEIVER_CONTROL: 		.word 0xffff0000
RECEIVER_DATA:			.word 0xffff0004

TRANSMITTER_CONTROL:		.word 0xffff0008
TRANSMITTER_DATA:		.word 0xffff000c

MASK_RECEIVER_INTERRUPT:	.word 0x00000002
MASK_ENABLE_ALL_INTERRUPTS:	.word 0x0000ff01 

error_msg:		.asciiz "ERROR: program should never leave infinite loop\n"

   	
# ---------------------------------------------------------------------------
# MAIN
# 
# Text segment (code) for the user level program.
# ---------------------------------------------------------------------------

	.text
main:
	##### ENABLE RECEIVER INTERRUPTS #####
		
	# To enable receiver interrupts, the interrupt enable bit must be set 
	# to 1 in the memory-mapped receiver control register. 
	
	# Address of memory-mapped receiver control register. 
		
	### TODO: Load the value of RECEIVER_CONTRO to $t0.
	lw $t0,RECEIVER_CONTROL
	
		
	# Set the receiver control interrupt enable bit to 1.
	
	### TODO: Load the value of MASK_RECEIVER_INTERRUPT to $t1.
	lw $t1,MASK_RECEIVER_INTERRUPT 
	
	# Now the value of the memory-mapped receiver control register
	# can be updated. 
	
	### TODO: store the value in $t1 (value) to the address stored in $t0 (address).
	sw $t1,($t0)
	
	##### SPIM ONLY: ENABLE ALL INTERRUPTS #####
	
        # In MARS the status register is set to 0x0000ff11 on start-up, i.e., 
        # the interrupt enable bit (bit 0) is alredy set and all interrupt 
        # mask (bits 8-15) bits are set. 
        
        # If you use SPIM, the status register is set to 0x3000ff10 start-up, 
        # i.e., the interrupt enable (bit 0) is not set but all interrupt mask
        #  bits (bits 8-15) are set. 
        
        # If you use SPIM you must enable interrupts by setting the interrupt
        # enable bit in the Status register to 1. 
        
        
        ##### TRIGGER A TRAP EXCEPTION #####
        
        # To test if the exception/interrupt handler can distinguish between 
        # interrupts and exception, trigger a trap exception (exception code 13). 
        
        teqi $zero, 0
        
	##### INFINITE LOOP #####
	
loop_forever:
	
	# Infinite loop, simulating the CPU doing some useful work while 
	# waiting for user input. 
	
	addi $s0, $s0, 1
	j loop_forever
	
	##### CHECK FOR ERRORS #####
	
	# The program should never break out of the above infinite loop. 
	# Print error message if program breaks out of the infinite loop.
	
	li $v0, 4
	la $a0, error_msg
	syscall 
	
	# Terminate normally.
	
  	li   $v0, 10		# System call code (exit).	
   	syscall        			


# ---------------------------------------------------------------------------
# KERNEL DATA SEGMENT
# 
# Data used by the exception handler (kernel).
# ---------------------------------------------------------------------------

        .kdata

# Storage for saving registers when handling an exception/interrupt. 

__a0:   .word 0
__a1:   .word 0
__v0:   .word 0
__at:   .word 0
__t0:   .word 0

# Bit mask used to check the Status register for a pending hardware
# level 1 (keyboard) interrupt.

__MASK_STATUS_RECEIVER_INTERRUPT:	.word 0x00000100


# Strings used to print error messages. 
 
__unhandled_interrupt_msg: 	.asciiz "Unhandled interrupt\n"
__unhandled_exception_msg_1:  	.asciiz "Unhandled exception ("
__unhandled_exception_msg_2:	.asciiz ")\n"



# ---------------------------------------------------------------------------
# EXCEPTION HANDLER
#
# Kernel text segment, i.e., code for the exception/interrupt handler. 
# 
# The term exception is commonly used to refer to both exceptions and 
# interrupts. 
#
# Overall structure of the exception/interrupt handler:
#
# 1) Save contents in any registers (except $k0 and $k1) used by the
#    exception handler. 
#
# 2) Examine the cause register to find out if the reason for entering the
#    exception handler is an interrupt or and exception. If it is an 
#    exception, skip the offending instruction (EPC + 4) and go to step 6.
#
# 3) If it is an interrupt, find out if it is a keyboard interrupt
#    (hardware level 0). If it is not a keyboard interrupt, print a log 
#    message and go to step 6.
#
# 4) If it is a keyboard interrupt, read character from memory-mapped
#    receiver data register. 
#
# 5) Print the read character to the Run I/O console using the
#    MARS built in system call print_char.
#
# 6) Restore the contents the registers saved in step 1. 
#
# 7) Resume user level execution (eret instruction). 
# ---------------------------------------------------------------------------

        .ktext 0x80000180 	# This is the exception vector address for MIPS32.

__save_registers:

 	##################################
        ##### STEP 1: SAVE REGISTERS #####
	##################################
	
	## NOTE: the sw instruction is a pseudo instruction using $at. 
	## To save the current value of $at we first copy the value to $k0. 
	## Next we can use sw to save the value of $k0 (value of $at) to 
	## memory using the sw instruction.
		
        # .set noat             # SPIM - Turn of warnings for using the $at register.
        move $k0, $at		# Copy value of $at to $k0.
       	sw $k0, __at		# Save value of $at to memory.
        # .set at               # SPIM - Turn on warnings for using the $at register.

        ### TODO: save the values of $v0, $a0, $a1 and $t0 to memory. 
        ### You should use the labels __v0, __a0 etc declared in the .kdata segment. 
        sw $v0, __v0
        sw $a0, __a0
        sw $a1, __a1
        sw $t0, __t0

	##### NOTE: You may only use $k0, $k1, $v0, $a0, $a1 and $t0 in the remindee
	##### exception/interrupt handler. 
	
	##############################################
	##### STEP 2: EXAMINE THE CAUSE REGISTER #####
	##############################################
	
	### TODO: Use the mfc0 instruction to move the value in the coprocessor 0 
	### Cause register ($13) to $k0. 
	
	mfc0 $k0, $13
        
        ### TODO: Extract the exception code from the Cause register and store
        ### the exception code in $k1. 
        
        ### TIP 1: You can shift $k0 two steps to the right to get the 5 bit 
        ### exception code in bit position 0-4.
        
        ### TIP 2: Remember that other bits (for example pending interrupt bits) 
        ### may be set in the Cause register. You can use the andi (AND Immediate) 
        ### instruction to clear all but the 5 least significant bits. 
	srl $k1,$k0,2 
	andi $k1,$k1,31

	
	##### IS IT AN EXCEPTION OR AN INTERRUPT? #####
	
	# If we enter the handler due to an interrupt the exception code will
	# be set to 0.
	
	### Use a branch instruction (for example bne) to jump to the label
	### __unhandled_exception if the exception code not equals zero.
	
        bne $k1, $zero, __unhandled_exception

__interrupt:
	
	#################################################
	#### STEP 3 - What kind of interrupt is it? #####
	#################################################
	        
        # If a receiver interrupt, the receiver interrupt pending bit (bit 8)
        # will be set to 1 in Cause register.  
	
	# $k0 - holds value of the cause register. 
	
	### Check if bit 8 is set to 1 in Cause register. 

	### TODO: Load the value at __MASK_STATUS_RECEIVER_INTERRUPT to $t0.
	lw $t0, __MASK_STATUS_RECEIVER_INTERRUPT
	 
	### TODO: Use the bit mask in $t0 to determine if the receiver interrupt pending
	### bit (bit 8) is set in $k0 (cause).
	###	
	### If a receiver interrupt is pending, set $k1 to the same value as $t0 
	### (__MASK_STATUS_RECEIVER_INTERRUPT), otherwise, make sure $k1 is not 
	### equal to $t0.
	###
	### TIP: This can be done with a single instruction.
	and $k1,$t0,$k0 
       	
       	# If not a receiver interrupt, jump to __unhandled_interrupt.
       	
       	bne $k1, $t0, __unhandled_interrupt
       	
        
__kbd_interrupt:
	
	# $t0 - __MASK_STATUS_RECEIVER_INTERRUPT
	
	# Reset Cause register, i.e., set bit 8 (receiver interrupt pending) to zero.
	
	not $t0, $t0
	and $k0, $k0, $t0
	mtc0 $k0, $13

	###################################
	##### STEP 4 - Read character #####
	###################################
	
        ### TODO: load address of the memory-mapped receiver data register
        ### to $k1 using the value stored at RECEIVER_DATA.
	lw $k1,RECEIVER_DATA
        
        # TODO: Load the ASCII value from the memory-mapped receiver data 
        # register to $a0.
        lw $a0,($k1) 

	####################################
	##### STEP 5 - Print character #####
	####################################
	
        li $v0, 11 # System call 11 (print_char)
        syscall
       
       	j __restore_registers


__unhandled_exception:
	
	# $k1 - exception code.
	
	li $v0, 4
	la $a0, __unhandled_exception_msg_1
	syscall
	
	li $v0, 1
	move $a0, $k1
	syscall
	
	li $v0, 4
	la $a0, __unhandled_exception_msg_2
	syscall
	
	
__return_from_exception:

   	# Skip instruction causing the exception, otherwise the same exception
   	# will trigger again.

   	mfc0 $k0, $14    # Coprocessor 0 register $14 (EPC) has address of trapping instruction.
   	addi $k0, $k0, 4 # Add 4 to point to next instruction.
   	mtc0 $k0, $14    # Store new address back into $14 (EPC).
   
   	j __restore_registers
   
__unhandled_interrupt:

	li $v0, 4
	la $a0, __unhandled_interrupt_msg
	syscall


__restore_registers:

	######################################
   	##### STEP 6 - Restore registers #####
	######################################
	
	### TODO: restore $v0, $a0, $a1, $t0 by loading the values you saved 
	### in the .kdata segment in step 1 (label __save_registers).
	
	lw $v0, __v0
        lw $a0, __a0
        lw $a1, __a1
        lw $t0, __t0
	
	# Restore $at.
	
  	# .set noat	# SPIM - Turn of warnings for using the $at register.
     	lw $at, __at
        # .set at       # SPIM - Turn on warnings for using the $at register.

__resume:
	
	################################################
	##### STEP 7 - Resume user level execution #####
	################################################
	
	
	# Use the eret (Exception RETurn) instruction to set the program counter
	# (PC) to the value saved in the ECP register (register 14 in coporcessor 0).
	
	eret

