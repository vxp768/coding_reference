#obj-m defines a loadable module goal, whereas obj-y indicates a built-in object goal
obj-m+=hello.o


#The $(shell uname -r) is a useful call to return the current kernel build version — this ensures a degree of portability for the Makefile. 

#The -C option switches the directory to the kernel directory before performing any make tasks. i

#The M=$(PWD) variable assignment tells the make command where the actual project files exist. i

#The modules target is the default target for external kernel modules. i

#An alternative target is modules_install which would install the module (the make command would have to be executed with superuser permissions and the module installation path is required).


all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
