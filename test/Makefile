# TTI test makefile
# 
# @author Clara Caldeira
# @author Guilherme N. Ramos (gnramos@unb.br)
# 

# dir to which header files are exported
EXPDIR  = export/include

INCLUDE_DIRS = ${TORCS_BASE} ${TORCS_BASE}/${EXPDIR} ${TORCS_BASE}/${EXPDIR}/TTI
INCLUDE_DIRS := $(foreach d, $(INCLUDE_DIRS), -I$d)

ifndef TORCS_BASE
    $(error [TTI] You must define the TORCS_BASE! For example: "export TORCS_BASE=/path/to/torcs-1.3.4" or "make TORCS_BASE=/path/to/torcs-1.3.4")
endif

ttest: ttest.o SimpleDriver.o
	g++ -L${TORCS_BASE}/export/lib -L/usr/lib  -o ttest ttest.o SimpleDriver.o -ltti -ltgf -ltxml

ttest.o: set_cxx_flag ttest.cpp 
	g++ ${INCLUDE_DIRS} -std=c++${CXX_STD} -pthread -c ttest.cpp 

SimpleDriver.o: SimpleDriver.h SimpleDriver.cpp 
	g++ -${INCLUDE_DIRS} -c SimpleDriver.cpp 

# Set compiler flag.
set_cxx_flag:
GXX_GTE_43 := $(shell expr `g++ -dumpversion | cut -f1` \>= 4.3)
ifneq "$(GXX_GTE_43)" "1"
    $(error Installed g++ version does not support C++11 standard.)
endif
GXX_GTE_47 := $(shell expr `g++ -dumpversion | cut -f1` \>= 4.7)
ifeq "$(GXX_GTE_47)" "1"
    CXX_STD = 11
else
    CXX_STD = 0x
endif