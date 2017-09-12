
/*
**	Modified by TKP, 3/5/2012
**		SFSU S2012
**		CSc615 homework #3:  Hello World Module.
**	
**	I have changed many of the initials "mcm" to the initials "tkp"
**	in order to correctly reflect myself as the user who has modified
**	the messages in /var/log/messages and in the /proc/testing file
**
**
** Module demo based on Kernel Projects for Linux, Lab #4 and lkmpg2.6
** To compile:  make -C /usr/src/linux M=`pwd`
** Contents of Makefile:  obj-m := mod_demo2.o
** Use insmod/rmmod to install and remove this module
** To invoke the module read routine use 'cat /proc/testing'
** Look in /var/log/messages for the module output (note that
**   you must 'emerge syslog-ng' and 'rc-update add syslog-ng default'
**   in order for output logging to occur)
**
**	MCM, 10/8/2005
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/moduleparam.h>
#include <linux/utsname.h>

#include <linux/time.h> // This is needed to support CURRENT_TIME
#define	pfile_name "testing"

MODULE_LICENSE("Dual BSD/GPL");

struct proc_dir_entry *tkp_mod_proc_file;
struct new_utsname *u;
//struct timeval mytv;
int
myreader(char *buffer,
	char **buffer_location,
	off_t offset, int buffer_length, int *eof, void *data)
{

	int	ret;
	printk(KERN_ALERT "Hello from tkp on %s at %d,%.2d, %s.\n",u->nodename,(int)CURRENT_TIME.tv_sec,(int)CURRENT_TIME.tv_nsec;//->/var/log/messages
/* see lkmp2.6 for details on the logic here */
	if (offset > 0) {
		ret = 0;
	} else {
		ret = sprintf(buffer, "Hello from tkp on %s, at %.2d,%.2d.\n",u->nodename,(int)CURRENT_TIME.tv_sec,(int)CURRENT_TIME.tv_nsec);// ->/proc/testing
	}
	return (ret);
}
static int demo_init(void)
{
	printk(KERN_ALERT "Hello from tkp on %s: starting module\n",u->nodename);
	tkp_mod_proc_file = create_proc_entry(pfile_name,0644,NULL);
	if (tkp_mod_proc_file == NULL) {
//		remove_proc_entry(pfile_name,&proc_root); &proc_root obsolete!
		remove_proc_entry(pfile_name,NULL);
		printk(KERN_ALERT "Error: create proc entry\n");
		return -ENOMEM;
	}
	tkp_mod_proc_file->read_proc = myreader;	
//	tkp_mod_proc_file->owner = THIS_MODULE;	owner obsolete!
	tkp_mod_proc_file->mode	= S_IFREG | S_IRUGO; /* copied from lkmp2.6 */
	tkp_mod_proc_file->uid=0;
	tkp_mod_proc_file->gid=0;
	tkp_mod_proc_file->size=0;
	printk(KERN_ALERT "/proc/testing created\n");
	return 0;
}
static void demo_exit(void)
{
	printk(KERN_ALERT "tkp:Lab#4 mod.ko is being removed.\n");
//	remove_proc_entry(pfile_name,&proc_root); &proc_root obsolete
	remove_proc_entry(pfile_name,NULL);
	printk(KERN_ALERT "/proc/testing removed\n");
}
module_init(demo_init);
module_exit(demo_exit);
