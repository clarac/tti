# TTI Project makefile
# 
# @author Guilherme N. Ramos (gnramos@unb.br)


# Sources
TTI_SRC_DIR = tti
TEST_SRC_DIR = test

TEST_TARGET = ttest


# TORCS_BASE, MAKE_DEFAULT, and LD_LIBRARY_PATH may already have been defined 
# (depending on how TORCS was installed.)
ifndef TORCS_BASE
	TORCS_BASE = /usr/src/torcs/torcs-1.3.4
    $(info [TTI] TORCS_BASE undefined, using "${TORCS_BASE}".)
endif
ifndef MAKE_DEFAULT
    MAKE_DEFAULT = ${TORCS_BASE}/Make-default.mk
    $(info [TTI] MAKE_DEFAULT undefined, using "${MAKE_DEFAULT}".)
endif
ifeq (,$(findstring torcs/lib,$LD_LIBRARY_PATH))
    LD_LIBRARY_PATH = $LD_LIBRARY_PATH:/usr/local/lib/torcs/lib
    $(info [TTI] TORCS library dir not found in LD_LIBRARY_PATH, using "/usr/local/lib/torcs/lib".)
endif


# Targets
.PHONY: tti

tti: link_tti_dir export_variables
	mkdir -p ${TORCS_BASE}/export/include/TTI/scr
	$(MAKE) -C $(TTI_SRC_DIR)
	$(MAKE) -C $(TTI_SRC_DIR) install
	$(MAKE) -C $(TTI_SRC_DIR) export

test: tti
	$(MAKE) -C $(TEST_SRC_DIR) TEST_TARGET=${TEST_TARGET}
	./${TEST_SRC_DIR}/${TEST_TARGET}

link_tti_dir:
ifeq "$(wildcard ${TORCS_BASE}/src/tti)" ""
    CURRENT_DIR = $(shell pwd)
    $(info [TTI] Linking "tti/" directory to "${TORCS_BASE}/src/".)
    $(shell ln -s ${CURRENT_DIR}/tti/ ${TORCS_BASE}/src/)
endif

# Set compiler flag.
set_cxx_flag:
GXX_GTE_43 := $(shell expr `g++ -dumpversion | cut -f1` \>= 4.3)
ifneq "$(GXX_GTE_43)" "1"
    $(error Installed g++ version does not support C++11 standard.)
endif
GXX_GTE_47 := $(shell expr `g++ -dumpversion | cut -f1` \>= 4.7)
ifeq "$(GXX_GTE_47)" "1"
    CXX_STD = c++11
else
    CXX_STD = c++0x
endif

export_variables: set_cxx_flag
export TORCS_BASE
export MAKE_DEFAULT
export LD_LIBRARY_PATH
export CXX_STD

clean:
	@$(MAKE) -C $(TTI_SRC_DIR) clean
	@$(MAKE) -C $(TEST_SRC_DIR) clean TEST_TARGET=${TEST_TARGET}

# Build documentation
doc: Doxyfile
	doxygen Doxyfile
