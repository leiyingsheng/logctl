#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define SIZE 100
//MODULE_LICENSE("leo BSD/GPL");

int major = 0;


int  drv_test_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "drv_test_open\n");

	return 0;
}


int drv_test_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "drv_test_release\n");
	
	return 0;
}


ssize_t drv_test_read (struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	int ret;
	printk(KERN_ALERT "drv_test_read\n");


	*offset  += count;
	
	return count;

size_err:
	printk(KERN_ALERT "read size Err \n");
	return -EINVAL;	
copy_to_user_err:
	printk(KERN_ALERT "copy_to_user Failed\n");
	return -EFAULT;	
}


ssize_t drv_test_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	printk(KERN_ALERT "%s",buf);
	

	return count;
}


struct file_operations   test_fops ={

	.open = drv_test_open,
	.release = drv_test_release,
	.read = drv_test_read,

};


//模块装载函数
int test_init(void)
{
	int ret = 0;
	printk(KERN_ALERT "xxx_init\n");

       major = register_chrdev(0,      //主设备号
                                          "drv_test",       //设备驱动名
                                         &test_fops);  //设备驱动服务接口集合	

	if(major)
		printk(KERN_ALERT "major = %d  \n",major);
	
	return 0;  //成功返回
}


void test_exit(void)
{
	printk(KERN_ALERT "xxx_exit\n");

       unregister_chrdev(major,    //主设备号
                                       "drv_test");       //设备驱动名	

}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");
