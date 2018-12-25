#include <linux/init.h>         //macros for __init __exit
#include <linux/module.h>	//core headers for loading module	
#include <linux/kernel.h>	//types macros for kernel
#include <linux/device.h>	//header to support kernel driver module
#include <linux/fs.h>		//headers for linux file system 
#include <asm/uaccess.h>	//for copy to user function

#define DEVICE_NAME    "saurabh"
#define CLASS_NAME     "kum"

static int majornumber; //determined automatically
static char message[256];
static short size_of_message;
static int open_cnt;

static struct class *charclass = NULL;
static struct device *chardev = NULL;

//prototype of functions for char driver
static int dev_open(struct inode* , struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char *, size_t, loff_t *);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);

static struct file_operations fops = 
{
    .open    = dev_open,
    .release = dev_release,
    .read    = dev_read,
    .write   = dev_write,
};

static int __init char_init(void)
{
   printk("initializing SAU char device\n");

   majornumber = register_chrdev(0, DEVICE_NAME, &fops);
   if(majornumber < 0) {
       printk("major number allocation failed\n");
       return majornumber;
   }

   printk("char device registered with majornumber:%d\n",majornumber);

   //Register the device class
   charclass = class_create(THIS_MODULE,CLASS_NAME);
   if (IS_ERR(charclass)) {
       unregister_chrdev(majornumber, DEVICE_NAME);
       printk("class creation failed\n");
       return PTR_ERR(charclass);
   }
   printk("Dev class created successfully\n");

   //Register the device
   chardev = device_create(charclass, NULL, MKDEV(majornumber,0), NULL, DEVICE_NAME);
   if (IS_ERR(chardev)) {
       class_destroy(charclass);
       unregister_chrdev(majornumber, DEVICE_NAME);
       printk("device create failed\n");  
       return PTR_ERR(chardev);
   }
   printk("device created successfully\n"); 
return 0;
}


static void __exit char_exit(void)
{
	device_destroy(charclass, MKDEV(majornumber,0));
	class_unregister(charclass);
	class_destroy(charclass);
	unregister_chrdev(majornumber, DEVICE_NAME);
	printk("Goodbye char device\n");
}

// this function is called everyime dev is opened from userspace
static int dev_open (struct inode *indoep, struct file *filep)
{
	open_cnt++;
	printk("Device opened..count is %d\n",open_cnt);
	return 0;
}

// this funcition is called when dev is closed/released
static int dev_release (struct inode *inodep, struct file *filep)
{
	printk("dev closed successfully\n");
	return 0;
}

static ssize_t dev_read (struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int error_count = 0;
    	
	error_count = copy_to_user(buffer, message, size_of_message);
	if (error_count==0){ 
		printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", size_of_message);
		return (size_of_message=0);  // clear the position to the start and return 0
	}
	else {
		printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
		return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
	}

}

static ssize_t dev_write (struct file *filep, const char *buffer, size_t len, loff_t *offset) 
{
	sprintf(message, "%s(%lu letters)", buffer, len);   // appending received string with its length
	size_of_message = strlen(message);                 // store the length of the stored message
	printk(KERN_INFO "EBBChar: Received %lu characters from the user\n", len);
	return len;
}

module_init(char_init);
module_exit(char_exit);

MODULE_AUTHOR("saurabh");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("char device driver");
MODULE_VERSION("1.0");
