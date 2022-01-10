
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/completion.h>
#include <linux/string.h>
#include "ioctl.h"

#define DEV Dev_ioctl

struct stats
{
    int size;
    char buff[20];
    int r_w;
};

//function Prototypes
int DEV_open(struct inode *inode, struct file *filp);
int DEV_release(struct inode *inode, struct file *filp);
ssize_t DEV_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
ssize_t DEV_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);
long  DEV_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

struct stats init_ptr = {0,'\0',0};

char Kbuff[20];


struct file_operations fops =
{

    .open    = DEV_open,
    .read    = DEV_read,
    .write   = DEV_write,
    .release = DEV_release,

};

//structure for character driver
struct cdev *my_cdev;

//init Module
static int __init CharDevice_init(void)
{
    int result;
    int MAJOR,MINOR;
    dev_t DevNum;
    DevNum = MKDEV(255,0);//Creating device number
    MAJOR = MAJOR(DevNum);
    MINOR = MINOR(DevNum);
    printk("\n major no--%d ... minor no --%d\n",MAJOR,MINOR);
    result = register_chrdev_region(DevNum,1,"Dev_ioctl");    //register device region
    if(result<0)
    {
        printk(KERN_ALERT "\nthe region is not obtainable");
        return(-1);
    }

    my_cdev = cdev_alloc(); //allocte memory to char device structure
    my_cdev->ops = &fops;   //link our file operatons to char device

    result = cdev_add(my_cdev,DevNum,1); //notify the kernel about new dvice
    if(result<0)
    {
        printk(KERN_ALERT "\nthe char device has not been created...!!\n");
        unregister_chrdev_region(DevNum,1);
        return(-1);
    }
    return 0;
}

//cleanup module
static void __exit CharDevice_exit(void)
{
    int result;
    int MAJOR,MINOR;
    dev_t DevNum;
    DevNum = MKDEV(255,0);//Creating 32bit device number 
    MAJOR = MAJOR(DevNum);
    MINOR = MINOR(DevNum);
    printk("\n major no--%d ... minor no --%d\n",MAJOR,MINOR);
    
    unregister_chrdev_region(DevNum,1);
    cdev_del(my_cdev);
    printk("\n unregistered my device and allocated memory\n");
    return;

}

//open system call
int DEV_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "DEV_ioctl is now open mode \n");
    return 0;
}

//syscal for ioctl functions
long DEV_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{   
    struct stats *ptr;
    int result,ret;
    
    int Ubuff[30];
    
    switch(cmd)
    {
        case GETSTATS:
            
            result = copy_to_user((struct stats *)arg, ptr, sizeof(struct stats));
            if(result == 0)
            {
            printk("Data read successfully by user");
            
            }
            else if(result > 0)
            {
            printk("Data remaining not read completely \n");
            }
            else
            {
            printk("error in writing to user from kernel buff \n");
            ret  =-EFAULT;
            return ret;
            }
        printk("Stats passing to user are \n");
        printk("SIZE of the buffer %d\n",ptr->size);
        printk("BUFFER DATA -- %s\n",ptr->buff);
        printk("ACTIVITY --%d\n",ptr->r_w);
        printk("copy from user return %d ",result);
        break;

        default:
            printk("wrong input\n");
            return(-EINVAL);
    }

     printk("/*----*********----*/\n");
     return 0;
}

//read system call
ssize_t DEV_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
    
    struct stats *k_read;
    unsigned long result;
    size_t ret;
    
    //copy Kbuff from kernel to Ubuff into user : copy-to-user
    result = copy_to_user(Ubuff,Kbuff,sizeof(Kbuff));
    if(result == 0)
    {
        
        k_read->r_w = 0;
        
        printk("Data read successfully by user into kernel");
        printk(KERN_ALERT "DATA--%s\n",Ubuff);
        
        printk("R_W = %d\n",k_read->r_w);
        ret = count;
        return ret;
    }
    else if(result > 0)
    {
        printk("Data remaining not read completely \n");
        ret = count - result;
        printk("bytes of data read succesfully== %ld\n",ret);
        printk("bytes left to read to user ==%ld \n",result);
        return ret;

    }
    else
    {
        printk("error in writing to user from kernel buff \n");
        ret  =-EFAULT;
        return ret;
    }
    
}

//write syscall for device
ssize_t DEV_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp)
{
    
    struct stats *k_write;
    unsigned long result;
    size_t ret;
    int len;
    //copy from Ubuff from user to Kbuff in kernel : copy-from-user
    result = copy_from_user(Kbuff,Ubuff,count);
    if(result==0)
    {
        strcpy(k_write->buff,Kbuff);
        k_write->r_w = 1;
        len = strlen(Kbuff);
        k_write->size = len;
        printk("Data written successfully by user into kernel");
        printk(KERN_ALERT "DATA--%s\n",Kbuff);
        printk("Size of Kbuff= %d \n",len);
        printk("R_W = %d\n",k_write->r_w);
        ret = count;

        
        return ret;
    }
    if(result>0)
    {
        printk("Data remaining.. not written completely \n");
        ret = count - result;
       
        printk("bytes of data written succesfully== %ld\n",ret);
        printk("bytes left to write into kernel ==%ld \n",result);
        return ret;
    }
    else
    {
        printk("error in reading from user to kernel buff \n");
        ret  =-EFAULT;
        return ret;
    }
    
return 0;

}

//close system call
int DEV_release(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "DEV_ioctl is now released from its duties \n");
    return 0;
}

module_init(CharDevice_init);
module_exit(CharDevice_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("KT");
MODULE_DESCRIPTION("IOCTL prog to getstats");

