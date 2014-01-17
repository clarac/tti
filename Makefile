# TTI Project makefile
# 
# @author Guilherme N. Ramos (gnramos@unb.br)


# Sources
TTI_SRC_DIR = tti
TEST_SRC_DIR = test

TEST_TARGET = ttest

TORCS_BASE_DEFAULT = /usr/src/torcs/torcs-1.3.4
TORCS_LIBRARY_PATH = /usr/local/lib/torcs/lib


# TORCS_BASE, MAKE_DEFAULT, and LD_LIBRARY_PATH may already have been defined 
# (depending on how TORCS was installed.)



# Targets
.PHONY: TTI

TTI: LINK_TTI_DIR EXPORT_VARIABLES
	@$(MAKE) -C $(TTI_SRC_DIR)
	@$(MAKE) -C $(TTI_SRC_DIR) install
	@$(MAKE) -C $(TTI_SRC_DIR) export

tti: TTI

test: TTI
	@$(MAKE) -C $(TEST_SRC_DIR)
	@./${TEST_SRC_DIR}/${TEST_TARGET}


SET_CXX_STD: 
    GXX_GTE_43 := $(shell expr `g++ -dumpversion | cut -f1` \>= 4.3)
    ifneq "$(GXX_GTE_43)" "1"
        $(error Installed g++ version does not support C++11 standard)
    endif
    GXX_GTE_47 := $(shell expr `g++ -dumpversion | cut -f1` \>= 4.7)
    ifeq "$(GXX_GTE_47)" "1"
        CXX_STD = -std=c++11
    else
        CXX_STD = -std=c++0x
    endif

SET_TORCS_BASE:
    ifndef TORCS_BASE
        TORCS_BASE = ${TORCS_BASE_DEFAULT}
        $(info [TTI] TORCS_BASE undefined, using "${TORCS_BASE}".)
    endif

SET_MAKE_DEFAULT: SET_TORCS_BASE
    ifndef MAKE_DEFAULT
        MAKE_DEFAULT = ${TORCS_BASE}/Make-default.mk
        $(info [TTI] MAKE_DEFAULT undefined, using "${MAKE_DEFAULT}".)
    endif

SET_LD_LIBRARY_PATH:
    ifeq (,$(findstring torcs/lib,$LD_LIBRARY_PATH))
        ifeq "$(wildcard ${TORCS_LIBRARY_PATH})" ""
            $(error TORCS_LIBRARY_PATH does not exist ("${TORCS_LIBRARY_PATH}"))
        endif
        LD_LIBRARY_PATH = $LD_LIBRARY_PATH:${TORCS_LIBRARY_PATH}
        $(info [TTI] TORCS library dir not found in LD_LIBRARY_PATH, using "${TORCS_LIBRARY_PATH}")
    endif

SETUP_VARIABLES: SET_MAKE_DEFAULT SET_LD_LIBRARY_PATH SET_CXX_STD
    ifeq "$(wildcard ${TORCS_BASE})" ""
        $(error ${TORCS_BASE} does not exist.)
    endif

EXPORT_VARIABLES: SETUP_VARIABLES
    export TORCS_BASE
    export MAKE_DEFAULT
    export LD_LIBRARY_PATH
    export CXX_STD
    export TEST_TARGET

LINK_TTI_DIR: SETUP_VARIABLES
    ifeq "$(wildcard ${TORCS_BASE}/src/tti)" ""
        CURRENT_DIR = $(shell pwd)
        $(info [TTI] Linking "tti/" directory to "${TORCS_BASE}/src/".)
        $(shell ln -s ${CURRENT_DIR}/tti/ ${TORCS_BASE}/src/)
    endif

clean:
	@$(MAKE) -C $(TTI_SRC_DIR) clean
	@$(MAKE) -C $(TEST_SRC_DIR) clean

# Build documentation
doc: Doxyfile
	doxygen Doxyfile
