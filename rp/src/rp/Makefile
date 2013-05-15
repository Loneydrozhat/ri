TOPDIR = ..

TARGETS = rp.o \
          rp_common.o \
          rp_app.o \
          rp_join.o 

include ${TOPDIR}/Makefile.conf

rp.o: rp.c 
	${COMPILE} rp.c 

rp_common.o: rp_common.c 
	${COMPILE} rp_common.c 

rp_app.o: rp_app.c 
	${COMPILE} rp_app.c 
	${LINK} ${UTILOBJS} ${RPOBJS} ${RPCOMMONOBJS} rp_app.o -o rp

rp_join.o: rp_join.c 
	${COMPILE} rp_join.c 
	${LINK} ${UTILOBJS} ${RPCOMMONOBJS} rp_join.o -o rp_join

clean:
	rm -f *.o core rp rp_join

strip:
	strip rp rp_join

install:
	cp -f rp rp_join ${INSTALLDIR}
