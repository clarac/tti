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

# TORCS_BASE, MAKE_DEFAULT, and LD_LIBRARY_PATH may already have been defined 
# (depending on how TORCS was installed.)
ifndef TORCS_BASE
    $(error [TTI] You must define the TORCS_BASE! For example: "export TORCS_BASE=/path/to/torcs-1.3.4" or "make TORCS_BASE=/path/to/torcs-1.3.4")
endif
ifndef MAKE_DEFAULT
    MAKE_DEFAULT = ${TORCS_BASE}/Make-default.mk
    $(info [TTI] MAKE_DEFAULT undefined, using "${MAKE_DEFAULT}".)
endif
ifeq (,$(findstring torcs/lib,$LD_LIBRARY_PATH))
    LD_LIBRARY_PATH = $LD_LIBRARY_PATH:/usr/local/lib/torcs/lib
    $(info [TTI] TORCS library dir not found in LD_LIBRARY_PATH, using "/usr/local/lib/torcs/lib".)
endif

# Set compiler flag.
GXX_GTE_43 := $(shell expr `g++ -dumpversion | cut -f1` \>= 4.3)
ifneq "$(GXX_GTE_43)" "1"
    $(error [TTI] Installed g++ version does not support C++11 standard.)
endif
GXX_GTE_47 := $(shell expr `g++ -dumpversion | cut -f1` \>= 4.7)
ifeq "$(GXX_GTE_47)" "1"
    CXX_STD = 11
else
    CXX_STD = 0x
endif

include ${MAKE_DEFAULT}

LDFLAGS  += -std=c++${CXX_STD} -pthread
CXXFLAGS += -std=c++${CXX_STD} -pthread
CFLAGSD  += -std=c++${CXX_STD} -pthread

# get documentation

doc:
	doxygen Doxyfile