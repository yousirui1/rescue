VERSION = 1.1

# arm -> linux arm rk3188 
# x86 -> window x86 mingw32 dll
# x64 -> linux x64

TARGET_ARCH = x64
CROSS_COMPILE = #arm-
DEBUG = -g #-O2

TOP_DIR := $(shell pwd)
OBJ_DIR := $(TOP_DIR)/obj
outdir := $(TOP_DIR)/bin
QT_DIR := $(TOP_DIR)/qt_pro
libdir := $(TOP_DIR)/lib

P2V_DIR := $(TOP_DIR)/virt-p2v
BT_DIR := $(TOP_DIR)/libtorrent

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
QMAKE= qmake
CP = cp

CONFIG_COMPILER = gnu

exeobj = rescue

dllobj =

libobj = 

mainobj = main.o tools.o log.o inirw.o device.o qcow2.o linux.o gpt.o disk.o StoreConfig.o event.o socket.o cJSON.o client.o \
		 ui.o queue.o task.o shell.o configs.o error.o  version.o tftp.o http.o alarm.o

cppobj = bt_client.o

p2vobj := p2v

btobj := bt

qtobj := qt

DISPLAY := QT

DEFINES := -D QT

CFLAGS := -I. -I./qt_pro -I./include/ -I./cJSON -L./lib -I./libtorrent/include

ifeq ($(TARGET_ARCH), arm)
CFLAGS += 
else ifeq ($(TARGET_ARCH), x86)
CFLAGS += -lmingw32 -lm -lws2_32 -lpthreadGC2 -lgdi32 
else ifeq ($(TARGET_ARCH), x64)
CFLAGS += -lpthread  -lxml2 -lpcre -lglib-2.0 -lboost_system   -ltorrent -std=c++11 -lqt #-luuid
endif

export CC CXX TOP_DIR OBJ_DIR DEBUG P2V_DIR BT_DIR

all: $(exeobj)

CXXFLAGS = $(CFLAGS)

VPATH = .:./include:./lib:./src:./libguestfs:./miniexpect:./cJSON:

$(p2vobj):
	$(MAKE) -C $(P2V_DIR) -j8

$(btobj):
	$(MAKE) -C $(BT_DIR) -j8

$(qtobj):
	cd $(QT_DIR) && $(QMAKE)
	$(MAKE) -C $(QT_DIR) -j8
	cp lib/libqt.so.1 /home/ysr/ftp/

$(mainobj):%.o:%.c 
	$(CC) -Wall $(DEBUG) $(CFLAGS) $(DEFINES) -c $< -o $@ 
    
$(cppobj):%.o:%.cpp
	$(CXX) -Wall $(DEBUG) $(CXXFLAGS) $(DEFINES) -c $< -o $@

$(exeobj):$(mainobj) $(cppobj) $(p2vobj) #$(btobj) 
	$(CXX) $(DEBUG)  -o $(outdir)/$(exeobj) $(mainobj) $(cppobj) $(wildcard $(OBJ_DIR)/*.o)  $(CXXFLAGS) 
	rm -f *.o
	@echo "Version $(VERSION)"
	@echo "Build  $(TARGET_ARCH) program $(exeobj) OK"
	$(CP) $(outdir)/$(exeobj) /home/ysr/ftp/

pack:
	sudo $(CP) $(outdir)/$(exeobj)  /opt/rootfs/initrd_voi/root/
	sudo $(CP) $(libdir)/libqt.so.1 /opt/rootfs/initrd_voi/root/lib
	sudo $(CP) $(libdir)/libtorrent.so /opt/rootfs/initrd_voi/root//lib

clean:
	rm -f *.o $(outdir)/$(exeobj) $(outdir)/*.dll $(outdir)/*.a $(OBJ_DIR)/*.o 
	$(MAKE) -C $(QT_DIR) clean
