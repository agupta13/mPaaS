 /*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 */

#include <linux/acpi.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/in.h>

//Socket Header Files
#include <linux/socket.h>
#include <linux/net.h>
#include <net/sock.h>
#include "ksocket.h"

static int port = 9998;

MODULE_AUTHOR("Joice,Arpit");
MODULE_DESCRIPTION("Basic LED Control Driver");
MODULE_LICENSE("Dual BSD/GPL");

/* Error Result Codes: */
#define INVALID_DEVICE_ID	250
#define INVALID_PARAMETER	251
#define INVALID_BUFFER		252
#define INTERFACE_ERROR		253
#define UNSUPPORTED_COMMAND	254
#define UNSPECIFIED_ERROR	255

/* Device ID */
#define DEVICE_ID_PANEL_BACK	1

/* LED Commands */
#define CMD_LED_ON	16
#define CMD_LED_OFF	17
#define CMD_LED_BLINK	18
/*Defining all socket functions here.*/

static int led_perform_fn(u8 length,
		u8 result_code,
		u8 device_id,
		u8 command,
		u8 on_time,
		u8 off_time)
{
	printk(KERN_ALERT "In perform function\n");
	printk(KERN_ALERT "Creating Socket now\n");
	ksocket_t sockfd_cli;
	struct sockaddr_in addr_srv;
	char buf[1024], *tmp;
	int addr_len,len;

#ifdef KSOCKET_ADDR_SAFE
	mm_segment_t old_fs;
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif

	sprintf(current->comm, "ksktcli");	
	*buf = "Hello!";
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

	tmp = inet_ntoa(addr_srv.sin_addr);
	printk("connected to : %s %d\n", tmp, ntohs(addr_srv.sin_port));
	kfree(tmp);
	
	krecv(sockfd_cli, buf, 1024, 0);
	printk("got message : %s\n", buf);

	len = sprintf(buf, "%s", "This is dell led\n");
        ksend(sockfd_cli, buf, len, 0);
	
	printk(KERN_ALERT "Closing dell led socket now!\n");
	kclose(sockfd_cli);
#ifdef KSOCKET_ADDR_SAFE
		set_fs(old_fs);
#endif
	
	return 0;

}

static int led_on(void)
{
	printk(KERN_ALERT "LED On!!!!!!\n");
	return led_perform_fn(3,	/* Length of command */
		INTERFACE_ERROR,	/* Init to  INTERFACE_ERROR */
		DEVICE_ID_PANEL_BACK,	/* Device ID */
		CMD_LED_ON,		/* Command */
		0,			/* not used */
		0);			/* not used */
}

static int led_off(void)
{
	printk(KERN_ALERT "LED Off!!!!!!\n");
	return led_perform_fn(3,	/* Length of command */
		INTERFACE_ERROR,	/* Init to  INTERFACE_ERROR */
		DEVICE_ID_PANEL_BACK,	/* Device ID */
		CMD_LED_OFF,		/* Command */
		0,			/* not used */
		0);			/* not used */
}

static int led_blink(unsigned char on_eighths,
		unsigned char off_eighths)
{
	return led_perform_fn(5,	/* Length of command */
		INTERFACE_ERROR,	/* Init to  INTERFACE_ERROR */
		DEVICE_ID_PANEL_BACK,	/* Device ID */
		CMD_LED_BLINK,		/* Command */
		on_eighths,		/* blink on in eigths of a second */
		off_eighths);		/* blink off in eights of a second */
}

static void led_set(struct led_classdev *led_cdev,
		enum led_brightness value)
{
	if (value == LED_OFF)
		led_off();
	else
		led_on();
}

static int dell_led_blink(struct led_classdev *led_cdev,
		unsigned long *delay_on,
		unsigned long *delay_off)
{
	unsigned long on_eighths;
	unsigned long off_eighths;

	/* The Dell LED delay is based on 125ms intervals.
	   Need to round up to next interval. */

	on_eighths = (*delay_on + 124) / 125;
	if (0 == on_eighths)
		on_eighths = 1;
	if (on_eighths > 255)
		on_eighths = 255;
	*delay_on = on_eighths * 125;

	off_eighths = (*delay_off + 124) / 125;
	if (0 == off_eighths)
		off_eighths = 1;
	if (off_eighths > 255)
		off_eighths = 255;
	*delay_off = off_eighths * 125;

	led_blink(on_eighths, off_eighths);

	return 0;
}

static struct led_classdev led = {
	.name		= "LED",
	.brightness	= LED_OFF,
	.max_brightness = 1,
	.brightness_set = led_set,
	.blink_set	= dell_led_blink,
	.flags		= LED_CORE_SUSPENDRESUME,
};

static int __init led_init(void)
{
	int error = 0;
	printk(KERN_ALERT "Init called\n");

	/*if (!wmi_has_guid(DELL_LED_BIOS_GUID))
		return -ENODEV;

	error = led_off();
	if (error != 0)
		return -ENODEV;
	*/
	printk(KERN_ALERT "classdev_register return : %d\n",error);
	if(!(error = led_classdev_register(NULL, &led))){
		led_on();
		led_off();
	}
	return 0;
}

static void __exit led_exit(void)
{
	led_classdev_unregister(&led);
	led_off();
}

module_init(led_init);
module_exit(led_exit);
