# Makefile for the mkfs.minix project

mkfs.minix: mkfs.minix.o create_msblk.o create_inodes.o create_mroot.o

mkfs.minix.o: mkfs.minix.c mkfs.minix.h
create_msblk.o: create_msblk.c mkfs.minix.h
create_inodes.o: create_inodes.c mkfs.minix.h
create_mroot.o: create_mroot.c mkfs.minix.h
