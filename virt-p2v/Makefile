
P2V_OBJ := p2v_transform.o p2v_config.o conversion.o cpuid.o inhibit.o kernel-cmdline.o kernel-config.o kernel.o \
      nbd.o physical-xml.o rtc.o ssh.o utils.o guestfs-utils.o miniexpect.o cleanups.o libxml2-cleanups.o

VPATH = .:./include:./lib:./src:./libguestfs:./miniexpect:

CFLAGS := -I. -I./include/  -I/usr/include/gtk-2.0/ -I/usr/include/glib-2.0/ -I/usr/lib/glib-2.0/include \
          -I/usr/include/cairo -I./include/gnulib -I/usr/include/pango-1.0 -I/usr/include/libxml2 \
        -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/atk-1.0 

all:$(ECHO) $(P2V_OBJ)

$(P2V_OBJ):%.o:%.c 				
	$(CC) $(DEBUG)  $(CFLAGS) -c $< -o $(OBJ_DIR)/$@  

ECHO:
	@echo $(P2V_OBJ)
clean:
	-rm $(P2V_OBJ)

