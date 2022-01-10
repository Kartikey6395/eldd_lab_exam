obj-m := char_drv.o

KDIR=/lib/modules/$(shell uname -r)/build/

PWD=$(shell pwd)

all:
		make -C $(KDIR) M=$(PWD) modules
		gcc -o user_ioctl user_ioctl.c
		gcc -o user_resd user_read.c
		gcc -o user_write user_write.c
		
install:
		make -C $(KDIR) M=$(PWD) modules_install

clean:
		make -C $(KDIR) M=$(PWD) clean