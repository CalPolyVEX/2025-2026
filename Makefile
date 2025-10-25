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


ifeq ($(OS),Windows_NT)
    SEP := \\
else
    SEP := /
endif

SCRIPT := tools${SEP}target.py
REQUIREMENTS := tools${SEP}requirements.txt
VENV_DIR := .venv


# build targets
all: 
	
	@make prebuild 
	@make $(BUILD)/$(PROJECT).bin
	@echo "-----------------------ignore the build filename ext.^^"
	

prebuild: setup
	@uv run python $(SCRIPT) 




# Create virtual environment and install dependencies
setup:
	 
	@if [ -d "$(VENV_DIR)" ]; then \
		echo "nothing" > /dev/null; \
	else \
		echo "Setting up virtual environment..."; \
		uv venv; \
		echo "Installing dependencies..."; \
		uv pip install --upgrade pip; \
		uv sync; \
	fi



# include build rules
include vex/mkrules.mk



