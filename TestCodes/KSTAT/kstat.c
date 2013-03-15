/* CSC 573 Kernel Programming Test Module*/
/* Illustrative Kernel Codes */
/* Kernel Programming */
/* Instructor: Arpit Gupta */




#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <net/sock.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/proc_fs.h>


#define MODULE_NAME "kstat"

 
typedef unsigned long int u_32;
typedef unsigned short int u_16;




// For sysfs
static int to_begin = 1 ;
module_param(to_begin, int , 0644) ;


// For procfs
u_32 nout_pkts = 0;
u_16 counter = 0;



/** For Post-Routing**/
static struct nf_hook_ops nf_ops_out;   

/* Handles for our procfs Directory and File */
static struct proc_dir_entry* file_kstat_nout;
static struct proc_dir_entry* dir_kstatDir;


/*
 * To READ from proc file
 */
ssize_t read_procfile_nout(char* buffer, char** buffer_location, off_t offset, int buffer_length,int* eof,void* data)
{
        int len =0;
        if(offset > 0){
            *eof = 1;
            return len;
        }
        char temp[255];
        len = sprintf(temp,"%ld",nout_pkts);
        //temp[len-1]= "\n";
        len = sprintf(buffer,"%s",temp);
        return len;
}

/***************************************************
 *
 * Function that performs operations on outgoing packet
 *
 ****************************************************/

unsigned int outgoing_hook(unsigned int hooknum,
            struct sk_buff *skb,
            const struct net_device *in,
            const struct net_device *out,
            int (*okfn)(struct sk_buff*))
{

    struct iphdr *iph;
    struct tcphdr *tcph;
    unsigned int tcplen,i,found=0;
 
    if (!skb)return NF_ACCEPT;
 
    iph = ip_hdr(skb);
    if (!iph)return NF_ACCEPT;
    
    if (iph->protocol==IPPROTO_TCP)
    { 
       nout_pkts+=1;
       if(nout_pkts%counter==0){
          printk(KERN_ALERT " Outgoing hook function called for nout: %ld\n",nout_pkts);
       }
    }
	 
    
    return NF_ACCEPT;
}


static int __init init(void){

   printk(KERN_ALERT " KSTAT Initialization Started \n");
   
   
   // Create a KSTAT Directory
   dir_kstatDir = proc_mkdir("kstat",NULL);
   if(dir_kstatDir==NULL){
      /*Do NOTHING*/
      return -ENOMEM;
   }
   printk(KERN_ALERT "proc directory created\n");
   
   
   // Create a proc file entry
   file_kstat_nout = create_proc_entry("nout",0644,dir_kstatDir);
   if(file_kstat_nout == NULL){
      /*Do NOTHING*/
      return -ENOMEM;
   }
   
   // Read Write Functions for proc file entries/entry
   file_kstat_nout->read_proc = read_procfile_nout;
   printk(KERN_ALERT "proc entries created\n");
   //file_kstat_nout->write_proc = write_procfile_nout;  
   
   // Initialize Hook Functions
   nf_ops_out.hook = outgoing_hook;
   nf_ops_out.pf = PF_INET;
   nf_ops_out.hooknum =4;
   nf_ops_out.priority = NF_IP_PRI_FIRST; 
   
   
   // Read config information
   printk(KERN_ALERT "Reading the configuration file\n"); 
   // for counter variable
   counter = 100;
   
   
   // Register the Hook functions   
   nf_register_hook(&nf_ops_out);
   printk(KERN_ALERT "hook functions registered\n");
      
	
   // A non 0 return means init_module failed; module can't be loaded.
   printk(KERN_ALERT " KSTAT Initialization Completed \n");
   return 0;
}


static void __exit cleanup(void){

  printk(KERN_ALERT "KSTAT Exit Started\n");
  
  // Delete proc entries
  remove_proc_entry ("nout", dir_kstatDir);
  remove_proc_entry("kstat",NULL);
  printk(KERN_ALERT "proc entries deleted\n");
  
  // Unregister the hook functions
  nf_unregister_hook(&nf_ops_out);
  printk("Unregistered the hooks\n");
  
  
  printk(KERN_ALERT "KSTAT Exit Completed\n");
}  

/* init and cleanup functions */
module_init(init);
module_exit(cleanup);

MODULE_LICENSE("GPL");

