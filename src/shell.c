#include "base.h"

char install_sh[] = {"#!/bin/sh \n"
					 "mkdir -p src dst \n"
					 "mount /dev/%s1 dst \n"
					 "mount /dev/%s1 src \n"
					 "outpu=`unzip -o src/voi.zip -d dst/ `\n"
					 "if [ $? -ne 0 ]; then \n"
    					"echo \"failed\" \n"
					"else \n"
    					"echo \"successd\" \n"
					"fi \n"
					"umount src dst \n"
					"rm -rf src dst \n"
					};


char upgrad_sh[] = {"#!/bin/sh \n"
					 "tftp -g -r %s %s	\n"
					 "outpu=`unzip -o /voi.zip -d /boot/ `\n"
					 "if [ $? -ne 0 ]; then \n"
    					"echo \"failed\" \n"
					"else \n"
    					"echo \"successd\" \n"
					"fi \n"
					};

char mount_sh[] = {"#!/bin/sh \n"
 					 "mount /dev/%s1 /boot \n" 
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


