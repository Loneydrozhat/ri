TOPDIR = ..

TARGETS = util.o \
          vector.o \
		  int.o \
          hash.o 

include ${TOPDIR}/Makefile.conf

util.o: util.c
	${COMPILE} util.c 

vector.o: vector.c
	${COMPILE} vector.c 

hash.o: hash.c
	${COMPILE} hash.c 

int.o: int.c
	${COMPILE} int.c 

clean:
	rm -f *.o core

install:

