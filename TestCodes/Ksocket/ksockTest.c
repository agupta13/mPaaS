
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter_ipv4.h>


#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/types.h>
//#include <linux/smp_lock.h>

#include <linux/netdevice.h>
/*
 * For forming tcp and ip headers
 */
#include <linux/ip.h>
#include <linux/tcp.h>

#include <linux/in.h>
#include <linux/inet.h>
#include <linux/inetdevice.h>

#include <linux/delay.h>

#include <linux/string.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <net/sock.h>
#include <net/ip.h>
#include <net/tcp.h>

#include <asm/processor.h>
#include <asm/uaccess.h>
/*
 * For file access
 */
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/proc_fs.h>

#include "ksocket.h"
#define MODULE_NAME  "ksockTest"

static int port = 4444;
module_param(port, int, 0444);



int tcp_srv(void *arg)
{
	ksocket_t sockfd_srv, sockfd_cli;
	struct sockaddr_in addr_srv;
	struct sockaddr_in addr_cli;
	char buf[512], *tmp;
	int addr_len, len;

#ifdef KSOCKET_ADDR_SAFE
		mm_segment_t old_fs;
		old_fs = get_fs();
		set_fs(KERNEL_DS);
#endif

	sprintf(current->comm, "ksockTest Server"); /* kernel thread name*/
//	lock_kernel();	 /* This seems to be required for exit_mm */
//	exit_mm(current);
//	/* close open files too (stdin/out/err are open) */
//	exit_files(current);	
	
	sockfd_srv = sockfd_cli = NULL;
	memset(&addr_cli, 0, sizeof(addr_cli));
	memset(&addr_srv, 0, sizeof(addr_srv));
	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(port);
	addr_srv.sin_addr.s_addr = INADDR_ANY;
	addr_len = sizeof(struct sockaddr_in);
	
	sockfd_srv = ksocket(AF_INET, SOCK_STREAM, 0);
	printk("sockfd_srv = 0x%p\n", sockfd_srv);
	if (sockfd_srv == NULL)
	{
		printk("socket failed\n");
		return -1;
	}
	if (kbind(sockfd_srv, (struct sockaddr *)&addr_srv, addr_len) < 0)
	{
		printk("bind failed\n");
		return -1;
	}

	if (klisten(sockfd_srv, 10) < 0)
	{
		printk("listen failed\n");
		return -1;
	}

	sockfd_cli = kaccept(sockfd_srv, (struct sockaddr *)&addr_cli, &addr_len);
	if (sockfd_cli == NULL)
	{
		printk("accept failed\n");
		return -1;
	}
	else
		printk("sockfd_cli = 0x%p\n", sockfd_cli);
	
	tmp = inet_ntoa(addr_cli.sin_addr);
	printk("got connected from : %s %d\n", tmp, ntohs(addr_cli.sin_port));
	kfree(tmp);
	
	len = sprintf(buf, "%s", "Hello, welcome to ksocket tcp srv service\n");
	ksend(sockfd_cli, buf, len, 0);
	
	/*while (1)
	{
		memset(buf, 0, sizeof(buf));
		len = krecv(sockfd_cli, buf, sizeof(buf), 0);
		if (len > 0)
		{
			printk("got message : %s\n", buf);
			ksend(sockfd_cli, buf, len, 0);
			if (memcmp(buf, "quit", 4) == 0)
				break;
		}
	}*/

	kclose(sockfd_cli);
	kclose(sockfd_srv);
#ifdef KSOCKET_ADDR_SAFE
		set_fs(old_fs);
#endif
	
	return 0;
}

int init_kcache_module(void) {
	
	kernel_thread(tcp_srv, NULL, 0);
	printk("ksockTest :: module loaded\n");

	return 0;
}


void cleanup_kcache_module(void) {


	printk("ksockTest :: module unloaded\n");

}


module_init(init_kcache_module);
module_exit(cleanup_kcache_module);

MODULE_LICENSE("GPL");
