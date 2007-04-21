#!/bin/sh
#http://mij.oltrelinux.com/devel/autoconf-automake/

#1 CLEANING - vsetky veci del

#2 AUTOSCAN
#autoscan

#3 MOVE to configure.ac
#mv configure.scan configure.ac

#4 VIM configure.ac
#AC_INIT(mailod, 0.1, garaj@fei.sk)
#AC_CONFIG_SRCDIR([src/mailod.c])

#5 AUTOCONF
#autoconf

#6 VIM Makefile.am
#AUTOMAKE_OPTIONS = foreign
#SUBDIRS = src

#7 VIM src/Makefile.am
## what flags you want to pass to the C compiler & linker
#AM_CFLAGS = --pedantic -Wall -std=c99 -O2 -g
#AM_LDFLAGS =
#
##this lists the binaries to produce, the (non-PHONY, binary) targets in
##the previous manual Makefile
#bin_PROGRAMS = xmailod
#xmailod_SOURCES = mailod.c const.h database_function.h email.h hash_function.h common_function.h logging.h

#8 VIM configure.ac
#AC_INIT(mailod, 0.1, garaj@fei.sk)
#AM_INIT_AUTOMAKE(mailod, 0.1)
#AC_CONFIG_FILES([src/Makefile])
##NIE#AC_OUTPUT(Makefile src/Makefile)

#9 ACLOCAL

#10 AUTOHEADER

#11 automake --add-missing

#12 autoconf




#echo Running autoscan
#autoscan
#echo Moving configure.scan to configure.ac
#mv configure.scan configure.ac

#AC_INIT(mailod, 0.1, garaj@fei.sk)  >> configure.ac
#AM_INIT_AUTOMAKE(mailod, 0.1)


#echo Running aclocal.
#aclocal


#vytvorit configure.h.in
echo Running autoheader.
autoheader


echo Running automake.
automake --add-missing


#finaly script configure
echo Running autoconf
autoconf
