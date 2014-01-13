# TTI test makefile
# 
# @author Clara Caldeira
# @author Guilherme N. Ramos (gnramos@unb.br)

ttest: ttest.o SimpleDriver.o
	g++ -L${TORCS_BASE}/export/lib -L/usr/lib  -o ttest ttest.o SimpleDriver.o -ltti -ltgf -ltxml

ttest.o: set_cxx_flag ttest.cpp 
	g++ -I${TORCS_BASE}/export/include -I${TORCS_BASE} -I. -std=c++${CXX_STD} -pthread -c ttest.cpp 

SimpleDriver.o: SimpleDriver.h SimpleDriver.cpp 
	g++ -I${TORCS_BASE}/export/include -I${TORCS_BASE} -I. -c SimpleDriver.cpp 

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