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

SDL_DIR := ./SDL/$(TARGET_ARCH)
FFMPEG_DIR := ./ffmpeg/$(TARGET_ARCH)

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

outdir = ./bin

exeobj = parted

dllobj =

libobj = 

mainobj = main.o parted.o strlist.o ui.o xmalloc.o exception.o  command.o device.o architecture.o disk.o geom.o natmath.o \
		  filesys.o constraint.o unit.o timer.o  table.o linux.o gpt.o pt-tools.o efi_crc32.o fat.o bootsector.o loop.o \
		  dvh.o reiserfs.o interface.o
cppobj = 

DISPLAY := QT

DEFINES := -D QT

CFLAGS := -I. -I./qt_pro -I./include/ 

ifeq ($(TARGET_ARCH), arm)
CFLAGS += 
else ifeq ($(TARGET_ARCH), x86)
CFLAGS += -lmingw32 -lm -lws2_32 -lpthreadGC2 -lgdi32 
else ifeq ($(TARGET_ARCH), x64)
CFLAGS +=  -luuid
endif

all: $(exeobj)

CXXFLAGS = $(CFLAGS)

VPATH = .:./include:./lib:./src:./libguestfs:./miniexpect:

$(mainobj):%.o:%.c 
	$(CC) -Wall $(DEBUG) $(CFLAGS) $(DEFINES) -c $< -o $@ 
    
$(cppobj):%.o:%.cpp
	$(CXX) -Wall $(DEBUG) $(CXXFLAGS) $(DEFINES) -c $< -o $@

$(exeobj):$(mainobj) $(cppobj) 
	$(CXX) $(DEBUG)  -o $(outdir)/$(exeobj) $(mainobj) $(cppobj)  $(CXXFLAGS) 
	rm -f *.o
	@echo "Version $(VERSION)"
	@echo "Build  $(TARGET_ARCH) program $(exeobj) OK"

clean:
	rm -f *.o $(outdir)/$(exeobj) $(outdir)/*.dll $(outdir)/*.a

