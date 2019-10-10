
# chemins 
ODIR=./
EXEDIR=.

#  <<<<<<<<  ici, ca change souvent
EXEC2=$(EXEDIR)/nanodac
OBJS2=nanodac.o

LIBS=

#  <<<<<<<<  a partir d'ici , ca doit plus changer tellement 
#options de compil
IPATH= -I. -I../h -I../../h -I../src -I/usr/include/modbus  -L/usr/local/lib -lmodbus
CFLAGS= -g $(IPATH)
LDFLAGS=
CC=gcc

all: $(EXEC) $(EXEC1) ${EXEC2}

# make depend
depend:
	makedepend *.c -Y -p$(ODIR)/ -- $(CFLAGS)
# sauve 
sauve:
	tar cvf /dev/fd0 *.c *.h Makefile	
	tar cvf /home/drieux/testmodbus.tar *.c *.h Makefile	

install:
	cp nanodac /home/mecadmin/bin
	cp nanodac.sh /home/mecadmin/bin
	cp nanodac.ctl /home/mecadmin/bin
# executable
$(EXEC2): $(OBJS2)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(EXEC2) $(OBJS2) $(LIBS)

# les dependances des .o crees par makedepend
# DO NOT DELETE THIS LINE -- make depend depends on it.

