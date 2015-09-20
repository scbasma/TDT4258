        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset: 
	      
	ldr r1, cmu_base_addr
	
	ldr r2, [r1, #CMU_HFPERCLKEN0]

	mov r3, #1
	
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO

	orr r2, r2, r3
	
	str r2, [r1, #CMU_HFPERCLKEN0]

	mov r1, #2

	ldr r2, gpio_pa_base

	str r1, [r2, #GPIO_CTRL]

	ldr r1, gpio_set_output_modeh

	str r1, [r2, #GPIO_MODEH]

	B _button_init

_button_init:
	
	ldr r4, gpio_pc_model

	ldr r5, gpio_pc_base

	str r4, [r5, #GPIO_MODEL]

	mov r4, #0xff

	str r4, [r5, #GPIO_DOUT]

	B _enable_interrupts

_enable_interrupts:
    //
	ldr r1, gpio_extipsell_constant
	
	ldr r2, gpio_base

	str r1, [r2, #GPIO_EXTIPSELL]

	mov r1, #0xff

    //setting rise and fall
    //letting interrupt happen on both rise and fall
	str r1, [r2, #GPIO_EXTIFALL]

	str r1, [r2, #GPIO_EXTIRISE]
    //interupt generation
	str r1, [r2, #GPIO_IEN]

    //can't hold 0x802 in one, do mov and then add to make it work

	mov r1, #50
	
	add r1, #2000

	ldr r2, isero 

	str r1, [r2, #0]

    //loading gpio_pa_base i r2 for senere bruk
	ldr r2, gpio_pa_base
	
    //enable deep sleep
	mov r3, #0b110

	ldr r1, scr

	str r3, [r1, #0]

	// Setting initial value in output registers

	mov r4, #1

	lsl r4, r4, #8

	mvn r4, r4 

	str r4, [r2, #GPIO_DOUT]

    //wait for instructions
	wfi





gpio_pc_model:
	.long GPIO_PC_MODEL
	
gpio_set_output_modeh:
	.long GPIO_SET_OUTPUT_MODEH

gpio_pc_base:
	.long GPIO_PC_BASE

gpio_pa_base:
	.long GPIO_PA_BASE

gpio_base:
	.long GPIO_BASE

cmu_base_addr:
	.long CMU_BASE

gpio_extipsell_constant:
	.long GPIO_EXTIPSELL_CONSTANT

isero: 
	.long ISER0
exception_return_to_thread_constant:
	.long EXCEPTION_RETURN_TO_THREAD_CONSTANT

emu_base:
	.long EMU_BASE

scr:
	.long SCR


	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:
    
    mov r7, #0xff
    ldr r3, gpio_base
    str r7, [r3, #GPIO_IFC]	  
    ldr r1, [r5, #GPIO_DIN]

    mov r6, #0x10 // Mask for detecting button press on button 5

    mov r7, #0x40 // Mask for detecting button press on button 7
    
    and r3, r1, r6 

    cbz r3, _handle_button_5 // Button 5 is pressed if bit 4 is 0

_return_button_5:

    and r3, r1, r7

    cbz r3, _handle_button_7 // Button 7 is pressed if bit 6 is 0

_return_button_7:
    mov r4, r4     

    str r4, [r2, #GPIO_DOUT]
    bx lr

_handle_button_5:
	
	lsr r4, r4, #8 // Moving output vector into the first 8 bit of the register

  mov r8, #0xff // Mask for extracting only the bits of the output vector

	and r4, r4, r8 // Does extraction

	mov r8, #0x01 // Mask for extracting bit 0 in the output vector

	and r8, r8, r4 // Does extraction

	lsl r4, r4, #7 // Moving the output vector back to the output window shifted one bit to the right 

	lsl r8, r8, #15 // Movong the first bit of the output vector to last position 

  	orr r4, r4, r8 // Adding the last bit to the output word

	B _return_button_5

_handle_button_7:
	
	lsr r4, r4, #8 // Moving output vector into the first 8 bit of the register

  	mov r8, #0xff // Mask for extracting only the bits of the output vector

	and r4, r4, r8 // Does extraction

	mov r8, #0x80 // Mask for extracting bit 7 in the output vector

	and r8, r8, r4 // Does extraction

	lsl r4, r4, #9 // Moving the output vector back to the output window shifted one bit to the left

	lsl r8, r8, #1 // Movong the first bit of the output vector to last position 

  	orr r4, r4, r8 // Adding the last bit to the output word
	
	B _return_button_7	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b . // do nothing


