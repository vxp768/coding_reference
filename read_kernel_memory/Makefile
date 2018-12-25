CXXFLAGS=-O2 -falign-labels=16
# Bulldozer/Piledriver has problems with code alignment. And falign-loops 
# doesn't seem to align all loops!!

EXTRA_CFLAGS=-O2

obj-m += pgmod.o

all: pgmod.ko paging

pgmod.ko: pgmod.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm paging

paging: paging.cc
