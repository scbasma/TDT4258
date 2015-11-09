/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>


#define GPIO_PC_BASE 0x40006048
#define GPIO_MODEL 0x04
#define GPIO_DIN 0x1c
#define GPIO_DOUT 0x0c

#define DRIVER_NAME "gamepad"
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



static void* pGPIOPC;
static dev_t devNr;
static struct cdev gamepad_cdev;
static struct file_operations gamepad_fops{
.owner = THIS_MODULE,
.read = gamepad_read,
.write = gamepad_write,
.open = gamepad_open,
.release = gamepad_release
};


static int __init template_init(void)
{	

	if(!alloc_chrdev_region(&devNr, MINIOR_NUMBER, DEVICE_NUMBER, DEVICE_NAME)){
		printk("UNABLE TO MAKE DRIVER AS KERNEL MODULE");
		return -1;
	}
	if(request_mem_region(GPIO_PC_BASE, 0x24, "GPIO") == NULL){
		printk("UNABLE TO RESERVE MEMORY REGION");
		return -1;
	}
	pGPIOPC = ioremap_nocache(GPIO_PC_BASE, 0x24);
	
	iowrite32(0x33333333, pGPIOPC + GPIO_MODEL);
	iowrite8(0xff, pGPIOPC + GPIO_DOUT);
	
	cdev_init(&gamepad_cdev, &gamepad_fops);
	cdev_add(&gamepad_cdev, devNr, DEVICE_NUMBER);
	cl = class_create(THIS_MODULE, DEvICE_NAME);
	device_create(cl, NULL, devNr, NULL, DEVICE_NAME);
	
	printk("Hello World, here is your module speaking\n");
	return 0;
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
	uint32 temp = ioread(pGPIOPC + GPIO_DOUT);
	copy_to_user(buff, &temp ,1);
	return 1;
}

static ssize_t gamepad_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp){
	return 1;
}

static int gamepad_open(struct inode *inode, struct file *filp){
	return 1;
}


static int gamepad_release(struct inode *inode, struct file *filp){
	return 1;
}
module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

