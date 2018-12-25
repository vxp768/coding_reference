#include <linux/init.h>      //macros used to markup functions e.g. __init__ __exit__
#include <linux/module.h>    //Core header for looking LKMs in to kernel
#include <linux/kernel.h>    //Contains types, macros, functions for the kernel


//use of glboal variable is discouraged ....use static to limit the scope
static char *name="world";
module_param(name, charp, S_IRUGO);   //module_param(name, type, permissions)
				      // S_IRUGO allows read access for user/group/others
		
MODULE_PARM_DESC(name, "name to display");

static int howmany = 1;
module_param(howmany, int, S_IRUGO);
MODULE_PARM_DESC(howmany, "number of times to display");

/* __init means that for built-in (not LKM) it is needed only at 
   initialization and can be freed after init
*/
static int __init hello_init(void)
{
    int i;
    for(i=0; i<howmany; i++) {
        printk("Hello :%s\n",name);
    }
return 0;
}

/*
  __exit macro notifies that if it is built for built-in (not LKM) then
  this functio is not needed
*/
static void __exit hello_exit(void)
{
    printk("GoodBye\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saurabh");
MODULE_DESCRIPTION("hello");
MODULE_VERSION("1.0");

