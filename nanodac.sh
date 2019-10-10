#!/bin/bash
export LANG=C
export ORACLE_SID=OCTAL
export  NLS_LANG=american_america.we8iso8859p15
export ORACLE_HOME=/usr/lib/oracle/12.2/client64
export LD_LIBRARY_PATH=/usr/lib/oracle/12.2/client64/lib
export PATH=$PATH:$ORACLE_HOME/bin
export SQLLDR=$ORACLE_HOME/bin/sqlldr

USER="proginfo/qlzM9zr3M@MECAPROTEC"
[ "$1" = "mpa" ] && USER="proginfo/ormsdnfmmsgnisgn@MPA"

EXEDIR=/home/mecadmin/bin
WORKDIR=/tmp
cd $WORKDIR
DATE=`date +'%Y%m%d%H%M%S'`
PREFIX=$DATE
FDATA=$WORKDIR/nanodac.dat
FCTL=$EXEDIR/nanodac.ctl

$EXEDIR/nanodac > $FDATA

$SQLLDR userid=$USER data=$FDATA control=$FCTL errors=200000 # > /tmp/traceldr.log 2&>1
