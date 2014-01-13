# TTI Project makefile
# 
# @author Guilherme N. Ramos (gnramos@unb.br)


# Sources
TTI_SRC_DIR = tti
TEST_SRC_DIR = test

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


# Targets
.PHONY: tti

tti: set_cxx_flag
	$(MAKE) -C $(TTI_SRC_DIR) CXX_STD=${CXX_STD}
	$(MAKE) -C $(TTI_SRC_DIR) install CXX_STD=${CXX_STD}
	$(MAKE) -C $(TTI_SRC_DIR) export CXX_STD=${CXX_STD}

test: set_cxx_flag
	$(MAKE) -C $(TEST_SRC_DIR) CXX_STD=${CXX_STD}

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

clean:
	@$(MAKE) -C $(TTI_SRC_DIR) clean
	@$(MAKE) -C $(TEST_SRC_DIR) clean

# Build documentation
doc: Doxyfile
	doxygen Doxyfile