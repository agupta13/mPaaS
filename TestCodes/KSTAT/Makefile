# Makefile for kstat -- Stat box in kernel 

obj-m := kstat.o
KDIR  := /lib/modules/$(shell uname -r)/build
KMOD  := /lib/modules/$(shell uname -r)/kernel
PWD   := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.ko *.o *.mod.* .H* .tm* .*cmd Module.symvers *modules.order

