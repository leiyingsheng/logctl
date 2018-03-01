#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define SIZE 100
MODULE_LICENSE("leo BSD/GPL");

int major = 0;

/**
*
*open()函数
*/

int  drv_test_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "drv_test_open\n");

	return 0;
}

/**
*close()函数
*
*/
int drv_test_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "drv_test_release\n");
	
	return 0;
}


/**
*write()函数
*
*/


ssize_t drv_test_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	int ret = 0;
	
	printk(KERN_ALERT "%s",buf);
	return ret;
	
}


struct file_operations   test_fops ={

	.open = drv_test_open,
	.release = drv_test_release,
	.write = drv_test_write,
};


//模块装载函数
int test_init(void)
{
	int ret = 0;
	printk(KERN_EMERG "test_init\n");

       major = register_chrdev(0,      //主设备号
                                          "drv_test",       //设备驱动名
                                         &test_fops);  //设备驱动服务接口集合	

	if(major)
		printk(KERN_ALERT "major = %d  \n",major);
	
	return 0;  //成功返回
}


void test_exit(void)
{
	printk(KERN_ALERT "test_exit\n");

       unregister_chrdev(major,    //主设备号
                                       "drv_test");       //设备驱动名	

}

module_init(test_init);
module_exit(test_exit);
