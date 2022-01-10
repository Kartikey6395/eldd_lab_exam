obj-m := char_drv.o

KDIR=/lib/modules/$(shell uname -r)/build/

PWD=$(shell pwd)

all:
		make -C $(KDIR) M=$(PWD) modules
		gcc -o dev1_read dev1_read.c
		gcc -o dev1_write dev1_write.c
		gcc -o dev2_write dev2_write.c
		gcc -o dev2_read dev2_read.c
		
install:
		make -C $(KDIR) M=$(PWD) modules_install

clean:
		make -C $(KDIR) M=$(PWD) clean