# TTI makefile
# 
# @author Clara Caldeira
# @author Guilherme N. Ramos (gnramos@unb.br)

# Define library files.
SOLIBRARY = libtti.so
SOURCES   = TTI.cpp  TTIWrapperDriver.cpp RaceSet.cpp 
LIBSPATH = scr/*.o

# dir to which header files are exported
EXPDIR  = include/TTI
EXPORTS = TTI.h TTIWrapperDriver.h scr/WrapperBaseDriver.h RaceSet.h Client.h \
          scr/CarControl.h scr/CarState.h scr/SimpleParser.h scr/BaseDriver.h

SUBDIRS	= scr

include ${MAKE_DEFAULT}

LDFLAGS  += ${CXX_STD} -pthread
CXXFLAGS += ${CXX_STD} -pthread
CFLAGSD  += ${CXX_STD} -pthread
