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
#define BUFF_SIZE 30

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
	char name[11];
	char surname[11];
	int index;
	int points;
};

char buff1[BUFF_SIZE];
char buff2[BUFF_SIZE];
struct storage s[10];
struct storage pom;
struct storage a;
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
int provera( struct storage a,struct storage s[]){
	int true=-1, i;
	for(i=0;i<10;i++)
	{
		if (!(strcmp(a.name, s[i].name)) && !(strcmp(a.surname,s[i].surname)) && (a.index == s[i].index))
			true=i;
	}
	return true+1;
}

void sort(struct storage s[]){
	int i, j;
	
	for (i = 0; i < 10; ++i) 
	{
		for (j = i + 1; j < 10; ++j)
		{
			if (s[i].points > s[j].points) 
			{
				a.points = s[i].points;
				a.index  = s[i].index;
				strcpy(a.name, s[i].name);
				strcpy(a.surname, s[i].surname);
				
				s[i].points =  s[j].points;
				s[i].index  = s[j].index;
				strcpy(s[i].name, s[j].name);
				strcpy(s[i].surname, s[j].surname);
				
				s[j].points = a.points;
				s[j].index  = a.index;
				strcpy(s[j].name, a.name);
				strcpy(s[j].surname, a.surname);
			}
		}
	}
	
}

ssize_t storage_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) 
{	
	long int len;
	int i, ret;
	if (endRead){
		endRead = 0;
		i = 0;
		printk(KERN_INFO "Succesfully read from file\n");
		return 0;
	}
	
	sort(s);
	ret = copy_to_user(buffer, buff1, len);
	ret = copy_to_user(buffer, buff2, len);
	
	for(i=1; i<11; i++)
	{
		if(i==11) 
			endRead = 1;
		else
		{
			if (s[i-1].index != 0)
				printk(KERN_INFO "%s %s EE%d_2020 = %d \n", s[i-1].name, s[i-1].surname, s[i-1].index, s[i-1].points);
		}
	}
	return len;
}

ssize_t storage_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) 
{
	int ret, i;
	int format1, format2;
	int br_ind1, br_ind2, bodovi;
	char ime1[11], ime2[11];
	char prezime1[11], prezime2[11];
	void delete(struct storage s[], char del_ime[], char del_prezime[], int del_index){
	for(i=0;i<10;i++){
			if (!(strcmp(s[i].name, del_ime)) && !(strcmp(s[i].surname,del_prezime)) && s[i].index == del_index){
				strcpy(s[i].name, " ");
				strcpy(s[i].surname, " ");
				s[i].index=0;
				s[i].points=0;			
			printk(KERN_INFO "Succesfully deleted value from position\n",i);
			}			
		}	
	}
	format1 = copy_from_user(buff1, buffer, length);
	if(format1)
		return -EFAULT;
	buff1[length-1] = '\0';
	format2 = copy_from_user(buff2, buffer, length);
	if(format2)
		return -EFAULT;
	buff2[length-1] = '\0';

// semafor
	/*if(down_interruptible(&sem))
		return -ERESTARTSYS;
	while(position == 10)
	{
		up(&sem);
		if(wait_event_interruptible(writeQ,(position<10)))
			return -ERESTARTSYS;
		if(down_interruptible(&sem))
			return -ERESTARTSYS;
	}*/
	format1 = sscanf(buff1,"%s %s EE%d_2020 = %d",ime1, prezime1, &br_ind1, &bodovi);
	format2 = sscanf(buff2,"izbrisi = %s %s %d",ime2, prezime2, &br_ind2);
	// ako unesem 4 parametra - format1 = 4, format2 =0, za izbrisi - format1=format2 = 2
	//printk(KERN_INFO "format1 = %d format2 = %d\n", format1, format2);
	if(format1 == 4)
	{
		if(position < 10) 
		{
			strcpy(pom.name,ime1);
			strcpy(pom.surname,prezime1);
			pom.index=br_ind1;
			pom.points=bodovi;
			if(provera(pom,s))
			{
				s[provera(pom,s)-1].points=bodovi;
                printk(KERN_INFO "Succesfully changed value in position %d \n", (provera(pom,s)-1));
			} else 
			{
				strcpy(s[position].name, ime1);
				strcpy(s[position].surname, prezime1);
				s[position].index = br_ind1;
				s[position].points = bodovi;
				printk(KERN_INFO "Succesfully wrote value into position %d \n", position); 
				printk(KERN_INFO "%s %s EE%d_2020 = %d  ", s[position].name, s[position].surname, s[position].index, s[position].points);
                position++;
			}
		}else 
			printk(KERN_WARNING "Storage is full\n"); 
	} else 
		printk(KERN_WARNING "Wrong command format");
	
	if(format2 == 3)
	{
		delete(s,ime2,prezime2, br_ind2);
	}

	/*up(&sem);
	wake_up_interruptible(&readQ);*/

	return length;
}

static int __init storage_init(void)
{
	int ret = 0;
	int i=0;
	
	sema_init(&sem,1);
// inicijalizacija 
	for(i=0; i<10;i++){
	strcpy(s[i].name, " ");
        strcpy(s[i].surname, " ");
        s[i].index=0;
        s[i].points=0;
}

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
