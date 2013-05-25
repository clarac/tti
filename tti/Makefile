
#INC = -I${TORCS_BASE}/src/libs/tgf -I${TORCS_BASE}/src/linux

#tti: tti.o
#	g++ -o tti tti.o

#tti.o: TTI.cpp
#	g++ -Wall -c ${INC} -o tti.o TTI.cpp


ROBOT       = tti
MODULE      = libtti.so
MODULEDIR   = lib
SOURCES     = tti.cpp

SHIPDIR     = 
SHIP        = 
SHIPSUBDIRS =

PKGSUBDIRS  = ${SHIPSUBDIRS}

include ${MAKE_DEFAULT}
