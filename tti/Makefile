# TTI .so
SOLIBRARY = libtti.so
SOURCES   = TTI.cpp  SimpleParser.cpp WrapperBaseDriver.cpp CarControl.cpp CarState.cpp RaceSet.cpp

# dir to which header files are exported

EXPDIR  = include

EXPORTS = TTI.h WrapperBaseDriver.h RaceSet.h Client.h CarControl.h CarState.h SimpleParser.h BaseDriver.h

# needs MAKE_DEFAUT to be configured to TORCS_BASE/Make-default.mk

include ${MAKE_DEFAULT}

# change to std=c++0x -pthread for gcc earlier then 4.3 

LDFLAGS += -std=c++11 -pthread
CXXFLAGS += -std=c++11 -pthread
CFLAGSD += -std=c++11 -pthread

# get documentation

doc:
	doxygen Doxyfile