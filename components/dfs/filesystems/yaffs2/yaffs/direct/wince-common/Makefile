# Makefile for YAFFS direct test
#
#
# YAFFS: Yet another Flash File System. A NAND-flash specific file system.
#
# Copyright (C) 2003-2010 Aleph One Ltd.
#
#
# Created by Charles Manning <charles@aleph1.co.uk>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# NB Warning this Makefile does not include header dependencies.
#
# $Id: Makefile,v 1.24 2010-02-17 02:18:57 charles Exp $

#EXTRA_COMPILE_FLAGS = -DYAFFS_IGNORE_TAGS_ECC

CFLAGS =      -DCONFIG_YAFFS_DIRECT -DCONFIG_YAFFS_SHORT_NAMES_IN_RAM -DCONFIG_YAFFS_YAFFS2  
CFLAGS +=     -DCONFIG_YAFFS_PROVIDE_DEFS -DCONFIG_YAFFSFS_PROVIDE_VALUES -DNO_Y_INLINE
CFLAGS +=    -Wall -g $(EXTRA_COMPILE_FLAGS) -Wstrict-aliasing 
#CFLAGS +=    -fno-strict-aliasing
CFLAGS +=    -O0
#CFLAGS +=    -DVALGRIND_TEST

#CFLAGS+=   -Wshadow -Wpointer-arith -Wwrite-strings -Wstrict-prototypes -Wmissing-declarations
#CFLAGS+=   -Wmissing-prototypes -Wredundant-decls -Wnested-externs -Winline

COMMONTESTOBJS = yaffscfg2k.o yaffs_ecc.o yaffs_fileem.o yaffs_fileem2k.o yaffsfs.o yaffs_guts.o \
		 yaffs_packedtags1.o yaffs_ramdisk.o yaffs_ramem2k.o \
		 yaffs_tagscompat.o yaffs_packedtags2.o yaffs_tagsvalidity.o yaffs_nand.o \
		 yaffs_checkptrw.o  yaffs_qsort.o\
		 yaffs_nameval.o \
		 yaffs_norif1.o  ynorsim.o nor_stress.o yaffs_fsx.o \
		 yaffs_allocator.o \
		 yaffs_bitmap.o \
		 yaffs_yaffs1.o \
		 yaffs_yaffs2.o \
		 yaffs_verify.o

#		 yaffs_checkptrwtest.o\

YAFFSTESTOBJS  = $(COMMONTESTOBJS) yaffs_test.o


ALLOBJS = $(sort $(YAFFSTESTOBJS))

YAFFSSYMLINKS = devextras.h yaffs_ecc.c yaffs_ecc.h yaffs_guts.c yaffs_guts.h yaffsinterface.h yportenv.h yaffs_tagscompat.c yaffs_tagscompat.h \
          yaffs_packedtags1.c yaffs_packedtags1.h yaffs_packedtags2.c yaffs_packedtags2.h  yaffs_nandemul2k.h \
          yaffs_nand.c yaffs_nand.h yaffs_getblockinfo.h yaffs_list.h \
          yaffs_tagsvalidity.c yaffs_tagsvalidity.h yaffs_checkptrw.h yaffs_checkptrw.c \
          yaffs_nameval.c yaffs_nameval.h \
          yaffs_qsort.c yaffs_qsort.h yaffs_trace.h \
          yaffs_allocator.c yaffs_allocator.h \
          yaffs_yaffs1.c yaffs_yaffs1.h \
          yaffs_yaffs2.c yaffs_yaffs2.h \
          yaffs_bitmap.c yaffs_bitmap.h \
          yaffs_verify.c yaffs_verify.h

YAFFSDIRECTSYMLINKS =  yaffsfs.c yaffs_flashif.h yaffs_flashif2.h\
		       yaffsfs.h yaffs_malloc.h ydirectenv.h \
		       yaffs_flashif.c yaffscfg.h\
		       yaffs_nandif.c yaffs_nandif.h



BASICTESTSYMLINKS = yramsim.c yramsim.h

SYMLINKS = $(YAFFSSYMLINKS) $(YAFFSDIRECTSYMLINKS) $(BASICTESTSYMLINKS)






symlinks: $(SYMLINKS)


$(YAFFSSYMLINKS):
	ln -s ../../$@ $@

$(YAFFSDIRECTSYMLINKS):
	ln -s ../$@ $@

$(BASICTESTSYMLINKS):
	ln -s ../basic-test/$@ $@


clean:
	rm -f $(SYMLINKS)
