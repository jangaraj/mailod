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

#7

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
