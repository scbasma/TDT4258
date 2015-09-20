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
	ldr r1, gpio_extipsell_constant
	
	ldr r2, gpio_base

	str r1, [r2, #GPIO_EXTIPSELL]

	mov r1, #0xff

	str r1, [r2, #GPIO_EXTIFALL]

	str r1, [r2, #GPIO_EXTIRISE]

	str r1, [r2, #GPIO_IEN]

	mov r1, #50
	
	add r1, #2000

	ldr r2, isero 

	str r1, [r2, #0]

	ldr r2, gpio_pa_base
	
	mov r3, #0b110

	ldr r1, scr

	str r3, [r1, #0]

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
    lsl r1, r1, #8
    str r1, [r2, #GPIO_DOUT]
    bx lr	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b . // do nothing

