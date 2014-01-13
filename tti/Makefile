# TTI makefile
# 
# @author Clara Caldeira
# @author Guilherme N. Ramos (gnramos@unb.br)

# Define library files.
SOLIBRARY = libtti.so
SOURCES   = TTI.cpp  SimpleParser.cpp TTIWrapperDriver.cpp \
            WrapperBaseDriver.cpp CarControl.cpp CarState.cpp RaceSet.cpp

# dir to which header files are exported
EXPDIR  = include/TTI
EXPORTS = TTI.h TTIWrapperDriver.h WrapperBaseDriver.h RaceSet.h Client.h \
          CarControl.h CarState.h SimpleParser.h BaseDriver.h

include ${MAKE_DEFAULT}

LDFLAGS  += -std=c++${CXX_STD} -pthread
CXXFLAGS += -std=c++${CXX_STD} -pthread
CFLAGSD  += -std=c++${CXX_STD} -pthread


clean:
	@rm *.o ${SOLIBRARY}