
SOLIBRARY = libtti.so
SOURCES   = tti.cpp SimpleParser.cpp WrapperBaseDriver.cpp CarControl.cpp CarState.cpp

EXPDIR  = include

EXPORTS = tti.h WrapperBaseDriver.h

include ${MAKE_DEFAULT}

LDFLAGS += -std=c++11 -pthread
CXXFLAGS += -std=c++11 -pthread