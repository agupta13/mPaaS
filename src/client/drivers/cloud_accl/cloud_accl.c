
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/module.h>




struct input_dev *cloudaccl_input_dev;        /* Representation of an input device */
static struct platform_device *cloudaccl_dev; /* Device structure */

/* Sysfs method */
static ssize_t
write_cloudaccl(struct device *dev,
          struct device_attribute *attr,
          const char *buffer, size_t count)
{
    int x,y;

    sscanf(buffer, "%d%d", &x, &y);
    /* Report relative ax via the
       event interface */
    input_report_rel(cloudaccl_input_dev, REL_X, x);
    input_report_rel(cloudaccl_input_dev, REL_Y, y);
    input_sync(cloudaccl_input_dev);
    return count;
}

/* Attach the sysfs write method */
DEVICE_ATTR(ax, 0644, NULL, write_cloudaccl);

/* Attribute Descriptor */
static struct attribute *cloudaccl_attrs[] = {
    &dev_attr_ax.attr,
    NULL
};

/* Attribute group */
static struct attribute_group cloudaccl_attr_group = {
    .attrs = cloudaccl_attrs,
};

/* Driver Initialization */
int __init
cloudaccl_init(void)
{
    /* Register a platform device */
    cloudaccl_dev = platform_device_register_simple("cloudaccl", -1, NULL, 0);
    if (IS_ERR(cloudaccl_dev)){
        printk ("cloudaccl_init: error\n");
        return PTR_ERR(cloudaccl_dev);
    }

    /* Create a sysfs node to read simulated ax */
    sysfs_create_group(&cloudaccl_dev->dev.kobj, &cloudaccl_attr_group);

    /* Allocate an input device data structure */
    cloudaccl_input_dev = input_allocate_device();
    if (!cloudaccl_input_dev) {
        printk("Bad input_allocate_device()\n"); return -ENOMEM;
    }

    /* Announce that the virtual mouse will generate
       relative ax */
    set_bit(EV_REL, cloudaccl_input_dev->evbit);
    set_bit(REL_X, cloudaccl_input_dev->relbit);
    set_bit(REL_Y, cloudaccl_input_dev->relbit);

    /* Register with the input subsystem */
    input_register_device(cloudaccl_input_dev);

    printk("Cloud Accelerometer Driver Initialized.\n");
    return 0;
}

/* Driver Exit */
void
cloudaccl_cleanup(void)
{
    /* Unregister from the input subsystem */
    input_unregister_device(cloudaccl_input_dev);

    /* Cleanup sysfs node */
    sysfs_remove_group(&cloudaccl_dev->dev.kobj, &cloudaccl_attr_group);

    /* Unregister driver */
    platform_device_unregister(cloudaccl_dev);
    printk("Cloud Accelerometer Driver Removed.\n");
    return;
}

module_init(cloudaccl_init);
module_exit(cloudaccl_cleanup);

MODULE_AUTHOR("Arpit");
MODULE_DESCRIPTION("cloudaccl book code");
MODULE_LICENSE("Dual BSD/GPL");


