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
	      
	// Enabeling CMU clock 
	ldr r1, cmu_base_addr
	
	ldr r2, [r1, #CMU_HFPERCLKEN0]

	mov r3, #1
	
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO

	orr r2, r2, r3
	
	str r2, [r1, #CMU_HFPERCLKEN0]

	// Setting pins connected to the LEDs as output pins

	mov r1, #2

	ldr r2, gpio_pa_base

	str r1, [r2, #GPIO_CTRL]

	ldr r1, gpio_set_output_modeh

	str r1, [r2, #GPIO_MODEH]

	// Setting pins connected to the buttons as input pins
	
	ldr r4, gpio_pc_model

	ldr r5, gpio_pc_base

	str r4, [r5, #GPIO_MODEL]

  // Enabeling external pullup for input pins

	mov r4, #0xff  

	str r4, [r5, #GPIO_DOUT]

	// Setting initial value in output registers

	mov r4, #1

	lsl r4, r4, #8

	mvn r4, r4 

_polling_loop:

	ldr r1, [r5, #GPIO_DIN]

	mov r6, #0x10 // Mask for detecting button press on button 5

  mov r7, #0x40 // Mask for detecting button press on button 7

	and r3, r1, r6 

	cbz r3, _handle_button_5 // Button 5 is pressed if bit 4 is 0

	and r3, r1, r7

	cbz r3, _handle_button_7 // Button 5 is pressed if bit 4 is 0

	str r4, [r2, #GPIO_DOUT]

	B _polling_loop



_handle_button_5:
	
	lsr r4, r4, #8 // Moving output vector into the first 8 bit of the register

  mov r8, #0xff // Mask for extracting only the bits of the output vector

	and r4, r4, r8 // Does extraction

	mov r8, #0x01 // Mask for extracting bit 0 in the output vector

	and r8, r8, r4 // Does extraction

	lsl r4, r4, #7 // Moving the output vector back to the output window shifted one bit to the right 

	lsl r8, r8, #15 // Movong the first bit of the output vector to last position 

  and r4, r4, r8 // Adding the last bit to the output word

	bx lr

_handle_button_7:
	
	lsr r4, r4, #8 // Moving output vector into the first 8 bit of the register

  mov r8, #0xff // Mask for extracting only the bits of the output vector

	and r4, r4, r8 // Does extraction

	mov r8, #0x80 // Mask for extracting bit 7 in the output vector

	and r8, r8, r4 // Does extraction

	lsl r4, r4, #9 // Moving the output vector back to the output window shifted one bit to the left

	lsl r8, r8, #1 // Movong the first bit of the output vector to last position 

  and r4, r4, r8 // Adding the last bit to the output word

	bx lr



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

	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  

	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b . // do nothing


