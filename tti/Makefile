
SOLIBRARY = libtti.so
SOURCES   = TTI.cpp  SimpleParser.cpp WrapperBaseDriver.cpp CarControl.cpp CarState.cpp RaceSet.cpp

EXPDIR  = include

EXPORTS = TTI.h WrapperBaseDriver.h RaceSet.h Client.h CarControl.h CarState.h SimpleParser.h BaseDriver.h

include ${MAKE_DEFAULT}

LDFLAGS += -std=c++11 -pthread
CXXFLAGS += -std=c++11 -pthread
CFLAGSD += -std=c++11 -pthread


doc:
	doxygen Doxyfile