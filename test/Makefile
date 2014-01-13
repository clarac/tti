# TTI test makefile
# 
# @author Clara Caldeira
# @author Guilherme N. Ramos (gnramos@unb.br)

# dir to which header files are exported under ${TORCS_BASE}.
EXPDIR  = export/include

INCLUDE_DIRS = ${TORCS_BASE} ${TORCS_BASE}/${EXPDIR} ${TORCS_BASE}/${EXPDIR}/TTI
INCLUDE_DIRS := $(foreach dir, $(INCLUDE_DIRS), -I${dir})

LIB_DIRS = ${TORCS_BASE}/export/lib /usr/lib
LIB_DIRS := $(foreach dir, $(LIB_DIRS), -L${dir})

LIBS = tti tgf txml
LIBS := $(foreach lib, $(LIBS), -l${lib})

#TEST_TARGET = ttest

ifndef TEST_TARGET
    $(error You must define the TEST_TARGET!)
endif

# Targets
all: ${TEST_TARGET}

${TEST_TARGET}: ttest.o SimpleDriver.o 
	g++ ${LIB_DIRS} -o ${TEST_TARGET} ttest.o SimpleDriver.o ${LIBS}

ttest.o: ttest.cpp
	g++ ${INCLUDE_DIRS} -std=${CXX_STD} -pthread -c ttest.cpp 

SimpleDriver.o: SimpleDriver.h SimpleDriver.cpp 
	g++ ${INCLUDE_DIRS} -c SimpleDriver.cpp

clean:
	@rm *.o ${TEST_TARGET}