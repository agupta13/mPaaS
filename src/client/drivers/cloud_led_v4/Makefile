TARGET_MODULE_CORE:=led-core
TARGET_MODULE_CLASS:=led-class
TARGET_MODULE_CLOUD:=led-cloud
#TARGET_MODULE_SOCKET:=ksocket

# If we running by kernel building system
ifneq ($(KERNELRELEASE),)
	#$(TARGET_MODULE_SOCKET)-objs := ksocket.o
	$(TARGET_MODULE_CORE)-objs := led-core.o
	$(TARGET_MODULE_CLASS)-objs := led-class.o
	$(TARGET_MODULE_CLOUD)-objs := cloud-led.o ksocket.o
	obj-m := $(TARGET_MODULE_CLOUD).o

# If we are running without kernel build system
else
	BUILDSYSTEM_DIR?=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)


all : 
# run kernel build system to make module
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules

clean:
# run kernel build system to cleanup in current directory
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) clean

load:
	sudo insmod ./$(TARGET_MODULE_CLOUD).ko

unload:
	sudo rmmod ./$(TARGET_MODULE_CLOUD).ko

perm:
	sudo chmod 0777 /sys/class/leds/LED/brightness	
    
echo1:
	echo 1 > /sys/class/leds/LED/brightness

echo0:
	echo 0 > /sys/class/leds/LED/brightness
	
rate100:
	echo 100 > /sys/devices/platform/cloudaccl/rate

endif

