/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/device.h>
#include <asm/io.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/fs.h>


#define GPIO_PC_BASE 0x40006048
#define GPIO_MODEL 0x04
#define GPIO_DIN 0x1c
#define GPIO_DOUT 0x0c

#define GPIO_IRQ_BASE 0x40006100
#define GPIO_EXTIPSEL 0x0
#define GPIO_EXTIRISE 0x8
#define GPIO_EXTIFALL 0xc
#define GPIO_IEN 0x10
#define GPIO_IFC 0X1c

#define DEVICE_NAME "gamepad"
#define MINIOR_NUMBER 0
#define DEVICE_NUMBER 1

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */


static ssize_t gamepad_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t gamepad_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
static int gamepad_open(struct inode *inode, struct file *filp);
static int gamepad_release(struct inode *inode, struct file *filp);
static int gamepad_fasync(int fd, struct file* filp, int mode);

static void* pGPIOPC;
static void* pGPIOIRQ;
static dev_t devNr;
static struct cdev gamepad_cdev;
static struct file_operations gamepad_fops = {
.owner = THIS_MODULE,
.read = gamepad_read,
.write = gamepad_write,
.open = gamepad_open,
.release = gamepad_release,
.fasync = gamepad_fasync,
};
struct class* cl;
static irqreturn_t gpio_interrupt_handler(int, void* ,struct pt_regs*);
struct fasync_struct* async_queue;

uint32_t button_array;



static int __init template_init(void)
{	
	
	// Allocating region for driver
	if(alloc_chrdev_region(&devNr, MINIOR_NUMBER, DEVICE_NUMBER, DEVICE_NAME) < 0){
		printk("UNABLE TO MAKE DRIVER AS KERNEL MODULE");
		return -1;
	}
	// Requesting memory area for GPIO registers
	if(request_mem_region(GPIO_PC_BASE, 0x24, "GPIO") == NULL){
		printk("UNABLE TO RESERVE MEMORY REGION");
		return -1;
	}
	// Requesting memory area for GPIO interrupt registers
	if(request_mem_region(GPIO_IRQ_BASE, 0x20, "GPIO_IRQ") == NULL){
		printk("UNABLE TO RESERVE MEMORY REGION");
		return -1;
	}
	// Making virtual memory adresses
	pGPIOPC = ioremap_nocache(GPIO_PC_BASE, 0x24);
	pGPIOIRQ = ioremap_nocache(GPIO_IRQ_BASE, 0x24);
	
	// requesting interrupt handlers for gpio interrupts
	request_irq(17, (irq_handler_t)gpio_interrupt_handler, 0, DEVICE_NAME, &gamepad_cdev);
	request_irq(18, (irq_handler_t)gpio_interrupt_handler, 0, DEVICE_NAME, &gamepad_cdev);
	
	// Configuring GPIO_PC as input
	iowrite32(0x33333333, pGPIOPC + GPIO_MODEL);
	iowrite32(0xff, pGPIOPC + GPIO_DOUT);
	
	// Initiating driver as char device
	cdev_init(&gamepad_cdev, &gamepad_fops);
	gamepad_cdev.owner = THIS_MODULE;
	cdev_add(&gamepad_cdev, devNr, DEVICE_NUMBER);
	cl = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(cl, NULL, devNr, NULL, DEVICE_NAME);

	// Configuring interrupt registers
	iowrite32(0xff, pGPIOIRQ + GPIO_EXTIFALL);
	iowrite32(0xff, pGPIOIRQ + GPIO_EXTIRISE);
	iowrite32(0x22222222, pGPIOIRQ + GPIO_EXTIPSEL);
	iowrite32(0x00ff, pGPIOIRQ + GPIO_IEN);
	iowrite32(0xff, pGPIOIRQ + GPIO_IFC);
	
	
	printk("Hello World, here is your module speaking\n");
	return 0;
}


irqreturn_t gpio_interrupt_handler(int irq, void* dev_id, struct pt_regs* regs){
	iowrite32(0xff, pGPIOIRQ + GPIO_IFC);
	printk("inside gpio interrupt handler\n");
	button_array = ioread32(pGPIOPC + GPIO_DIN);
	printk("async_queue is : %x\n", async_queue);
	if(async_queue){
		printk("Inside if async_queue\n");
		kill_fasync(&async_queue, SIGIO, POLL_IN);
	}
	return IRQ_HANDLED;
}


static int gamepad_fasync(int fd, struct file* filp, int mode){
	//async_queue = filp->private_data->async_queue;
	printk("Inside gamepad fasync\n");
	return fasync_helper(fd, filp, mode, &async_queue);
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */


static void __exit template_cleanup(void)
{
	 printk("Short life for a small module...\n");
}

static ssize_t gamepad_read(struct file *filp, char __user *buff, size_t count, loff_t *offp){
	copy_to_user(buff, button_array, 1);
	return 1;
}

static ssize_t gamepad_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp){
	return 1;
}

static int gamepad_open(struct inode *inode, struct file *filp){
	return 0;
}


static int gamepad_release(struct inode *inode, struct file *filp){
	return 0;
}
module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

