# Makefile for kstat -- Stat box in kernel 

TARGET_MODULE_CLOUD:=accl-cloud

$(TARGET_MODULE_CLOUD)-objs := cloud-accl.o ksocket.o
obj-m := $(TARGET_MODULE_CLOUD).o

KDIR  := /lib/modules/$(shell uname -r)/build
KMOD  := /lib/modules/$(shell uname -r)/kernel
PWD   := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod ./$(TARGET_MODULE_CLOUD).ko

unload:
	sudo rmmod ./$(TARGET_MODULE_CLOUD).ko
	
perm:
	sudo chmod 0777 /sys/devices/platform/cloudaccl/enable
	sudo chmod 0777 /sys/devices/platform/cloudaccl/rate
	
echo1:
	echo 1 > /sys/devices/platform/cloudaccl/enable

echo0:
	echo 0 > /sys/devices/platform/cloudaccl/enable
	
rate100:
	echo 100 > /sys/devices/platform/cloudaccl/rate
	

