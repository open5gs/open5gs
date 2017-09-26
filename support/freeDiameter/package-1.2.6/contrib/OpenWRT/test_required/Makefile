OWRT_ENV_ROOT=/root/openwrt-env/openwrt
STAGING_UCLIBC_ROOT=$(OWRT_ENV_ROOT)/staging_dir/target-mipsel_uClibc-0.9.31
STAGING_GCC_ROOT=$(OWRT_ENV_ROOT)/staging_dir/toolchain-mipsel_gcc-4.3.3+cs_uClibc-0.9.31

CFLAGS=-Os -pipe -mips32 -mtune=mips32 -funit-at-a-time -fhonour-copts -msoft-float  -I$(STAGING_UCLIBC_ROOT)/usr/include -I$(STAGING_UCLIBC_ROOT)/include -I$(STAGING_GCC_ROOT)/usr/include -I$(STAGING_GCC_ROOT)/include 
LDFLAGS=-L$(STAGING_UCLIBC_ROOT)/usr/lib -L$(STAGING_UCLIBC_ROOT)/lib -L$(STAGING_GCC_ROOT)/usr/lib -L$(STAGING_GCC_ROOT)/lib 

GCC=mipsel-openwrt-linux-uclibc-gcc
PATH:=$(STAGING_GCC_ROOT)/bin/:$(PATH)

testcase:	testcase.o
	$(GCC) $(LDFLAGS) -lpthread testcase.o -o testcase

testcase.o:	testcase.c
	$(GCC) $(CFLAGS) -o testcase.o -c testcase.c
