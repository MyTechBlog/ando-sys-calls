#include <linux/kernel.h>       
#include <linux/module.h>       
#include <linux/moduleparam.h>  
#include <linux/unistd.h>       
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/stat.h>

#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/netlink.h>
#include <linux/init.h>
#include <linux/rcupdate.h>
#include <linux/fdtable.h>
#include <linux/fs.h> 
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/slab.h>

#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>

MODULE_LICENSE("Dual BSD/GPL");

void __user *sample;

void **sys_call_table;

/* getuid() prototype */

asmlinkage int (*getuid_call)();


/* Back up pointer towards orginal sys_read() method */

asmlinkage long (*original_call_read) (unsigned int,char *,size_t); 



/* Hooked sys_read() method definition */

asmlinkage long our_sys_read(unsigned int fd, char  *buf, size_t count)
{
 uid_t gtuid ;
 gtuid = getuid_call();
 
 if(gtuid == 10028)
  printk ("our_sys_read ---> uid = %d  \n ", gtuid);

        return original_call_read(fd,buf,count);
}

/* Init module */

int init_module()
{
        
        sys_call_table = (void*)0xc0023aa4;

        original_call_read = sys_call_table[__NR_read];

        sys_call_table[__NR_read]  = our_sys_read;

 getuid_call = sys_call_table[__NR_getuid];

        return 0;
}

/*      CLEAN_UP Module */

void cleanup_module()
{
        printk(  "Clean up!!\n");

        sys_call_table[__NR_read]  = original_call_read;

}
