#include "base.h"

char http_install_sh[] = {"#!/bin/sh \n"
					 	"mkdir -p /boot/linux \n"
						"wget http://%s/boot/vmlinuz-5.2.8-lfs-9.0 -O /boot/linux/vmlinuz-5.2.8-lfs-9.0_new \n"
						"wget -P /root/ http://%s/boot/voi.zip \n"
					 	"outpu=`unzip -o /root/voi.zip -d /boot/ `\n"
					 	"if [ $? -ne 0 ]; then \n"
    					"	echo \"failed\" \n"
						"else \n"
    						"echo \"successd\" \n"
						"fi \n"
					};

char install_sh[] = {"#!/bin/sh \n"
					 "mkdir -p src dst \n"
					 "mount /dev/%s1 dst \n"
					 "mount /dev/%s1 src \n"
					 "mkdir -p dst/linux \n"
					 "cp -f src/EFI/linux/vmlinuz-5.2.8-lfs-9.0 dst/linux/ \n"				
					 "outpu=`unzip -o src/voi.zip -d dst/ `\n"
					 "if [ $? -ne 0 ]; then \n"
    					"echo \"failed\" \n"
					"else \n"
    					"echo \"successd\" \n"
					"fi \n"
					"umount src dst \n"
					"rm -rf src dst \n"
					};

#if 1
					 //"tftp -g -r %s %s	\n"
					 //"mv /vmlinuz-5.2.8-lfs-9.0 /boot/linux/vmlinuz-5.2.8-lfs-9.0 \n"
char upgrad_sh[] = {"#!/bin/sh \n"
					 "outpu=`unzip -o /root/voi.zip -d /boot/ `\n"
					 "if [ $? -ne 0 ]; then \n"
    					"echo \"failed\" \n"
					"else \n"
    					"echo \"successd\" \n"
					"fi \n"
					};
#endif


#if 0
char upgrad_sh[] = {"#!/bin/sh \n"
					 "tftp -g -r voi.zip %s	\n"
					 "outpu=`unzip -o /voi.zip -d /boot/ `\n"
					 "if [ $? -ne 0 ]; then \n"
    					"echo \"failed\" \n"
					"else \n"
    					"echo \"successd\" \n"
					"fi \n"
					};

char upgrad_sh_1[] = {"#!/bin/sh \n"
					 "tftp -g -r vmlinuz-5.2.8-lfs-9.0 %s	\n"
					 "mv /vmlinuz-5.2.8-lfs-9.0 /boot/linux/vmlinuz-5.2.8-lfs-9.0 \n"
					 "if [ $? -ne 0 ]; then \n"
    					"echo \"failed\" \n"
					"else \n"
    					"echo \"successd\" \n"
					"fi \n"
					};

#endif

char mount_sh[] = {"#!/bin/sh \n"
 					 "mount /dev/%s1 /boot \n" 
					 "if [ $? -ne 0 ]; then \n"
    				 "	echo \"failed\" \n"
					 "else \n"
					 "	echo \"successd\" \n"
					 "fi \n"
					};

#if 0
char mount_nv_sh[] = {"#!/bin/sh \n"
 					 "mount /dev/%sp1 /boot \n" 
					 "if [ $? -ne 0 ]; then \n"
    				 "	echo \"failed\" \n"
					 "else \n"
					 "	echo \"successd\" \n"
					 "fi \n"
					};
#endif


char mount_mmc_sh[] = {"#!/bin/sh \n"
 					 "mount /dev/%sp1 /boot \n" 
					 "if [ $? -ne 0 ]; then \n"
    				 "	echo \"failed\" \n"
					 "else \n"
					 "	echo \"successd\" \n"
					 "fi \n"
					};


char umount_sh[] = {"#!/bin/sh \n"
 					 "umount  /boot \n" 
					 "if [ $? -ne 0 ]; then \n"
    				 "	echo \"failed\" \n"
					 "else \n"
					 "	echo \"successd\" \n"
					 "fi \n"
					};


char check_data_sh[] = {"#!/bin/sh \n"
 					 "date -s %s \n" 
					 "hwclock --hctosys \n"
					 "if [ $? -ne 0 ]; then \n"
    				 "	echo \"failed\" \n"
					 "else \n"
					 "	echo \"successd\" \n"
					 "fi \n"};


