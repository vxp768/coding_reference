#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

#include <asm/tlbflush.h>

static struct file_operations fops;

MODULE_AUTHOR("Henry Wong <henry@stuffedcow.net>");
MODULE_DESCRIPTION("Allow retrieving CR3 and CR4 and flushing TLB.");
MODULE_LICENSE("GPL");

static inline uintptr_t get_cr3(void)
{
	uintptr_t val;
	asm volatile("mov %%cr3,%0\n" : "=r"(val), "=m"(__force_order));
	return val;
}
static inline uintptr_t get_cr4(void)
{
	uintptr_t val;
	asm volatile("mov %%cr4,%0\n" : "=r"(val), "=m"(__force_order));
	return val;
}

static void do_flush_tlb_all(void *info)
{
	native_write_cr3(native_read_cr3());
}

static int __init pgmod_init(void)
{
	printk(KERN_INFO "pgmod says hello\n");
	proc_create("pgmod", 0666,NULL,&fops);
	return 0;
}

static void __exit pgmod_cleanup(void)
{
	remove_proc_entry("pgmod", NULL);
	printk(KERN_INFO "pgmod says bye\n");
}


static int pgmod_proc_open(struct inode *sp_inode, struct file *sp_file)
{
	//printk(KERN_INFO "proc_open");
	return 0;
}
static int pgmod_proc_release(struct inode *sp_inode, struct file *sp_file)
{
	//printk(KERN_INFO "proc_release");
	return 0;
}
static ssize_t pgmod_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset)
{
	char kbuf[32];		// Make sure the string doesn't overflow this.
	int len=0;
	
	if (*offset>=1)
	{
		return 0;
	}

	len += sprintf(kbuf+len, "%llx\n", (uint64_t)get_cr3());
	len += sprintf(kbuf+len, "%llx\n", (uint64_t)get_cr4());
	len++;	// NULL-termination
		
	if (copy_to_user(buf, kbuf, len))
		printk(KERN_INFO "copy_to_user failed");

	(*offset)++;		// Advance offset by 1 arbitrary unit.

	return len;
}
static ssize_t pgmod_proc_write(struct file *sp_file, const char __user *buf, size_t size, loff_t *offset)
{
	//printk(KERN_INFO "proc_write: Flushing all TLBs");
	on_each_cpu(do_flush_tlb_all, NULL, 1);
	return size;
}


static struct file_operations fops = {
	.open = pgmod_proc_open,
	.read = pgmod_proc_read,
	.write = pgmod_proc_write,
	.release = pgmod_proc_release
};

module_init(pgmod_init);
module_exit(pgmod_cleanup);
