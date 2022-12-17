#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/wait.h>
#include <linux/semaphore.h>
#define BUFF_SIZE 20

MODULE_LICENSE("Dual BSD/GPL");

dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct cdev *my_cdev;

DECLARE_WAIT_QUEUE_HEAD(readQ);
DECLARE_WAIT_QUEUE_HEAD(writeQ);
struct semaphore sem;
struct fasync_struct *async_queue;

struct storage{
	char name[];
	char surname[];
	int index;
	int points;
};

char buff[BUFF_SIZE];
struct storage s[];
int position =0;
int endRead = 0;
int storage_open(struct inode *pinode, struct file *pfile);
int storage_close(struct inode *pinode, struct file *pfile);
ssize_t storage_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
ssize_t storage_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);

struct file_operations my_fops =
{
	.owner = THIS_MODULE,
	.open = storage_open,
	.read = storage_read,
	.write = storage_write,
	.release = storage_close,
};


int storage_open(struct inode *pinode, struct file *pfile) 
{
		printk(KERN_INFO "Succesfully opened file\n");
		return 0;
}

int storage_close(struct inode *pinode, struct file *pfile) 
{
		printk(KERN_INFO "Succesfully closed file\n");
		return 0;
}
int provera(struct storage s[],position){
	int l = sizeof(s[])/sizeof(s[0]);
	int true=0;
	for(int i=0;i<l;i++)
	{
		if (!(strcmp(s[position].name, s[i].name) && !(strcmp(s[position].surname,s[i].surname) && s[position].index == s[i].index)
			true=i;
	}
	return true+1;
}

void sort(struct storage s[]){
	int l;
	l = sizeof(s[])/sizeof(s[0]);
	struct storage a;
	for (int i = 0; i < l; ++i) 
	{
		for (int j = i + 1; j < l; ++j)
		{
			if (s[i].points > s[j].points) 
			{
				a.points =  s[i].points;
				a.index=s.index;
				strcpy(s.name,a.name);
				strcpy(s.surname,a.surname);
				s[i].points =  s[j].points;
				s[i].index=s[j].index;
				strcpy(s[j].name,s[i].name);
				strcpy(s[j].surname,s[i].surname);
				s[j].points =  a.points;
				s[j].index=a.index;
				strcpy(a.name,s[j].name);
				strcpy(a.surname,s[j].surname);
			}
		}
	}
	
}

ssize_t storage_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) 
{	
	long int len;
	int ret;
	int l;
	l = sizeof(s[])/sizeof(s[0]);
	if (endRead){
		endRead = 0;
		position = 0;
		printk(KERN_INFO "Succesfully read from file\n");
		return 0;
	}
	sort(s);
	while(position < l && endRead == 0) 
	{
		if (!(s[position].name=='/0' && s[position].surname=='/0' && s[position].index==0 && s[position].points==0))
		{
			len = scnprintf(buff,BUFF_SIZE , "%s,%s,EE%d=%d \n", s[position].name, s[position].surname, s[position].index, s[position].points);
			ret = copy_to_user(buffer, buff, len);
			if(ret)
				return -EFAULT;
		}
		position++;
		if (position == 10) {
			endRead = 1;
		}
	}
	return len;
}

ssize_t storage_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) 
{
	int ret;
	int br_ind, bodovi;
	char ime[15];
	char prezime[15];

	ret = copy_from_user(buff, buffer, length);
	if(ret)
		return -EFAULT;
	buff[length-1] = '\0';

// semafor
	if(down_interruptible(&sem))
		return -ERESTARTSYS;
	while(position == 10)
	{
		up(&sem);
		if(wait_event_interruptible(writeQ,(position<10)))
			return -ERESTARTSYS;
		if(down_interruptible(&sem))
			return -ERESTARTSYS;
	}
	
	ret = sscanf(buff,"%s,%s,EE%d=%d",ime, prezime, &br_ind, &bodovi);

	if(ret==4)// parameters parsed in sscanf
	{
		if(position < 10){
			s[position].name = ime;
			s[position].surname = prezime;
			s[position].index = br_ind;
			s[position].points = bodovi;
			if(!(provera(s,position))){
				printk(KERN_INFO "Succesfully wrote value into position\n"); 
				position++;
			}
			else{
				s[provera(s,position)-1].points=s[position].points;
				printk(KERN_INFO "Succesfully wrote value into position\n");
			}
		}
		else
		{
			printk(KERN_WARNING "Storage is full\n"); 
		}
	}
	else
	{
		printk(KERN_WARNING "Wrong command format");
	}
	
	up(&sem);
	wake_up_interruptible(&readQ);

	return length;
}

void izbrisi(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	int ret;
	struct storage *s[];
	int l = sizeof(s[])/sizeof(s[0]);
	int br_ind, bodovi;
	char ime[15];
	char prezime[15];
	ret = copy_from_user(buff, buffer, length);
	if(ret)
		return -EFAULT;
	buff[length-1] = '\0';
	ret = sscanf(buff,"izbrisi=%s,%s,%d=%d",ime, prezime, &br_ind, &bodovi);
	if(ret==4)// parameters parsed in sscanf
	{
		for(int i=0;i<l;i++){
			if (!(strcmp(s[i]->name, ime) && !(strcmp(s[i]->surname,prezime) && s[i]->index == br_ind && s[i]->points == bodovi){
				s[i]->name='/0';
				s[i]->surname='/0';
				s[i]->index=0;
				s[i]->points=0;
			}
			printk(KERN_INFO "Succesfully wrote value into position\n"); 
		}
	}
	else
	{
		printk(KERN_WARNING "Wrong command format");
	}

}


static int __init storage_init(void)
{
	int ret = 0;
	int i=0;
	
	sema_init(&sem,1);

	//Initialize array

	for (i=0; i<10; i++)
		storage[i] = 0;

   ret = alloc_chrdev_region(&my_dev_id, 0, 1, "storage");
   if (ret){
      printk(KERN_ERR "failed to register char device\n");
      return ret;
   }
   printk(KERN_INFO "char device region allocated\n");

   my_class = class_create(THIS_MODULE, "storage_class");
   if (my_class == NULL){
      printk(KERN_ERR "failed to create class\n");
      goto fail_0;
   }
   printk(KERN_INFO "class created\n");
   
   my_device = device_create(my_class, NULL, my_dev_id, NULL, "storage");
   if (my_device == NULL){
      printk(KERN_ERR "failed to create device\n");
      goto fail_1;
   }
   printk(KERN_INFO "device created\n");

	my_cdev = cdev_alloc();	
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;
	ret = cdev_add(my_cdev, my_dev_id, 1);
	if (ret)
	{
      printk(KERN_ERR "failed to add cdev\n");
		goto fail_2;
	}
   printk(KERN_INFO "cdev added\n");
   printk(KERN_INFO "Hello world\n");

   return 0;

   fail_2:
      device_destroy(my_class, my_dev_id);
   fail_1:
      class_destroy(my_class);
   fail_0:
      unregister_chrdev_region(my_dev_id, 1);
   return -1;
}

static void __exit storage_exit(void)
{
   cdev_del(my_cdev);
   device_destroy(my_class, my_dev_id);
   class_destroy(my_class);
   unregister_chrdev_region(my_dev_id,1);
   
(KERN_INFO "Goodbye, cruel world\n");
}


module_init(storage_init);
module_exit(storage_exit);
