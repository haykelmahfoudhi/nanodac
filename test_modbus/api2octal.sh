#!/bin/sh
export LANG=C
export ORACLE_HOME=/usr/lib/oracle/xe/app/oracle/product/10.2.0/server
export ORACLE_SID=OCTAL
export  NLS_LANG=american_america.we8iso8859p15
export LD_LIBRARY_PATH=$ORACLE_HOME/lib:$LD_LIBRARY_PATH
export SQLLDR=$ORACLE_HOME/bin/sqlldr

USER="proginfo/qlzM9zr3M@MECAPROTEC"
[ "$1" = "mpa" ] && USER="proginfo/ormsdnfmmsgnisgn@MPA"

EXEDIR=/home/drieux/PROJ/api2octal
WORKDIR=/tmp
cd $WORKDIR
DATE=`date +'%Y%m%d%H%M%S'`
PREFIX=$DATE
FDATA=$WORKDIR/api2octal.dat
FCTL=$EXEDIR/api2octal.ctl

$EXEDIR/api2octal > $FDATA

$SQLLDR userid=$USER data=$FDATA control=$FCTL errors=200000 # > /home/datalog/tmp/traceldr.log 2&>1
