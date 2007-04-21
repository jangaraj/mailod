#!/bin/sh
#http://mij.oltrelinux.com/devel/autoconf-automake/
#echo Running autoscan
#autoscan
#echo Moving configure.scan to configure.ac
#mv configure.scan configure.ac

#AC_INIT(mailod, 0.1, garaj@fei.sk)  >> configure.ac
#AM_INIT_AUTOMAKE(mailod, 0.1)

#echo Running aclocal.
#aclocal -I m4 $ACLOCAL_FLAGS

echo Running autoheader.
autoheader
echo Running automake.
automake -a
echo Running autoconf
autoconf
