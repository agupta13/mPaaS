#include "cloud_led_file.h"
#include <linux/fs.h> 	     /* file stuff */
#include <linux/kernel.h>    /* printk() */
#include <linux/errno.h>     /* error codes */
#include <linux/module.h>  /* THIS_MODULE */
#include <linux/cdev.h>      /* char device stuff */
#include <asm/uaccess.h>  /* copy_to_user() */

// Socket related header files
#include <linux/socket.h>
#include <linux/net.h>
#include <net/sock.h>
#include "ksocket.h"
static int port = 4444;


static const char    g_s_Hello_World_string[] = "Hello world from kernel mode!\n\0";
static const ssize_t g_s_Hello_World_size = sizeof(g_s_Hello_World_string);

/*===============================================================================================*/
static ssize_t device_file_read(
                           struct file *file_ptr
                        , char __user *user_buffer
                        , size_t count
                        , loff_t *possition)
{
   printk( KERN_NOTICE "Simple-driver: Device file is read at offset = %i, read bytes count = %u"
            , (int)*possition
            , (unsigned int)count );

   if( *possition >= g_s_Hello_World_size )
      return 0;

   if( *possition + count > g_s_Hello_World_size )
      count = g_s_Hello_World_size - *possition;

   if( copy_to_user(user_buffer, g_s_Hello_World_string + *possition, count) != 0 )
      return -EFAULT;   

   *possition += count;
   return count;
}
/*===============================================================================================*/
static struct file_operations simple_driver_fops = 
{
   .owner   = THIS_MODULE,
   .read    = device_file_read,
};

static int device_file_major_number = 0;

static const char device_name[] = "Simple-driver";

/*===============================================================================================*/
int test_socket(void)
{
	ksocket_t sockfd_cli;
	struct sockaddr_in addr_srv;
	char buf[1024], *tmp;
	int addr_len, len;

	#ifdef KSOCKET_ADDR_SAFE
		mm_segment_t old_fs;
		old_fs = get_fs();
		set_fs(KERNEL_DS);
	#endif

	memset(&addr_srv, 0, sizeof(addr_srv));
	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(port);
	addr_srv.sin_addr.s_addr = inet_addr("127.0.0.1");;
	addr_len = sizeof(struct sockaddr_in);
	
	sockfd_cli = ksocket(AF_INET, SOCK_STREAM, 0);
	printk("sockfd_cli = 0x%p\n", sockfd_cli);
	if (sockfd_cli == NULL)
	{
		printk("socket failed\n");
		return -1;
	}
	if (kconnect(sockfd_cli, (struct sockaddr*)&addr_srv, addr_len) < 0)
	{
		printk("connect failed\n");
		return -1;
	}

	//tmp = inet_ntoa(&addr_srv.sin_addr);
	//printk("connected to : %s %d\n", tmp, ntohs(addr_srv.sin_port));
	//kfree(tmp);
	
	len = sprintf(buf, "%s", "Hello, This is what I send\n");
	ksend(sockfd_cli, buf, len, 0);
	memset(buf, 0, sizeof(buf));
	krecv(sockfd_cli, buf, 1024, 0);
	printk("got message : %s\n", buf);

	kclose(sockfd_cli);
	#ifdef KSOCKET_ADDR_SAFE
			set_fs(old_fs);
	#endif
	
	return 0;


}

int register_device(void)
{
      
	int result;
	
	result = test_socket();
	if(result < 0)
	{
		printk( KERN_ALERT "led-driver: Server socket is not created." );
	}
	
	printk( KERN_ALERT "led-driver: register_device() is called." );

	result = register_chrdev( 0, device_name, &simple_driver_fops );
	if( result < 0 )
	{
	 printk( KERN_ALERT "Simple-driver:  can\'t register character device with errorcode = %i", result );
	 return result;
	}

	device_file_major_number = result;
	printk( KERN_ALERT "Simple-driver: registered character device with major number = %i and minor numbers 0...255"
		  , device_file_major_number );

	return 0;
}
/*-----------------------------------------------------------------------------------------------*/
void unregister_device(void)
{
   printk( KERN_ALERT "Simple-driver: unregister_device() is called" );
   if(device_file_major_number != 0)
   {
      unregister_chrdev(device_file_major_number, device_name);
   }
}
