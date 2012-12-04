CFLAGS = -g
EXT=x86_64
PAL = $(shell uname -i)
ifneq ($(PAL),$(EXT))
	CFLAGS += -march=i686
endif

#apache installed directory
APDIR=/webservice/server/apache2

#if you apache not first install to APDIR,maybe you need apache apr library
APRDIR=/home/chlaws/packet/apr-1.4.6/include
APR_UTIL_DIR=/home/chlaws/packet/apr-util-1.4.1/include
CFLAGS+=-I$(APRDIR) -I$(APR_UTIL_DIR)

#if you have header on other directory,set here
DEPINC=
#if you want other library ,set here
DEPLIB=

CFLAGS+=$(DEPINC) 


build :obj dynamic

obj:$(OBJ)
	g++ $(CFLAGS) -c -fPIC -I$(APDIR)/include -I./  -I`$(APDIR)/bin/apxs -q INCLUDEDIR` -I`$(APDIR)/bin/apxs -q CFLAGS` `$(APDIR)/bin/apxs -q CFLAGS_SHLIB`    -Wall -o mod_demo.o mod_demo.cpp
dynamic:mod_demo.o
	g++ $(CFLAGS) -fPIC -shared -u demo_module -o mod_demo.so  $< $(DEPLIB) -lrt `$(APDIR)/bin/apxs -q LIBS_SHLIB` 

install :
	cp -r mod_demo.so $(APDIR)/modules/
