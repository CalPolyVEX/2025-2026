# VEXcode makefile 2019_03_26_01

# show compiler output
VERBOSE = 0

# include toolchain options
include vex/mkenv.mk

# location of the project source cpp and c files
SRC_C  = $(wildcard src/*.cpp) 
SRC_C += $(wildcard src/*.c)
SRC_C += $(wildcard src/*/*.cpp) 
SRC_C += $(wildcard src/*/*.c)

OBJ = $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(SRC_C))) )

# location of include files that c and cpp files depend on
SRC_H  = $(wildcard include/*.h)

# additional dependancies
SRC_A  = makefile

# project header file locations
INC_F  = include

# build targets
all: 
	
	@make prebuild 
	@make $(BUILD)/$(PROJECT).bin
	@echo "-----------------------ignore the build filename ext.^^"
	

prebuild: setup

	$(VENV_DIR)/bin/python3 $(SCRIPT) 



# Define virtual environment directory
VENV_DIR := .venv

# Detect operating system
ifeq ($(OS),Windows_NT)
    PYTHON := $(VENV_DIR)\Scripts\python.exe
    PIP := $(VENV_DIR)\Scripts\pip.exe
    VENV_ACTIVATE := $(VENV_DIR)\Scripts\activate
else
    PYTHON := $(VENV_DIR)/bin/python3
    PIP := $(VENV_DIR)/bin/pip3
    VENV_ACTIVATE := . $(VENV_DIR)/bin/activate
endif

# Python script and argument
SCRIPT := tools/target.py


# Create virtual environment and install dependencies
setup:
	 
	@if [ -d "$(VENV_DIR)" ]; then \
		echo "nothing" > /dev/null; \
	else \
		echo "Setting up virtual environment..."; \
		python3 -m venv $(VENV_DIR); \
		. $(VENV_DIR)/bin/activate; \
		echo "Installing dependencies..."; \
		$(PIP) install --upgrade pip; \
		$(PIP) install -r tools/requirements.txt; \
	fi



# include build rules
include vex/mkrules.mk



