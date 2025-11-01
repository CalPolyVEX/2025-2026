# VEXcode makefile 2019_03_26_01 (cross-platform tidy)

# show compiler output
VERBOSE ?= 0

# detect platform
ifeq ($(OS),Windows_NT)
    SEP := \\
    SHELL := sh.exe
	.SHELLFLAGS := -c
    PYTHON ?= py
    UV ?= uv
    WHERE ?= where
else
    SEP := /
    SHELL := /bin/sh
    PYTHON ?= python3
    UV ?= uv
    WHERE ?= command -v
endif

# toolchain options
include vex$(SEP)mkenv.mk

# sources
SRC_C  := $(wildcard src/*.cpp) \
          $(wildcard src/*.c) \
          $(wildcard src/*/*.cpp) \
          $(wildcard src/*/*.c)

OBJ := $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(SRC_C))))

# headers (for change detection only)
SRC_H := $(wildcard include/*.h)

# additional dependencies
SRC_A := Makefile

# project header include dir (many mkrules use this)
INC_F := include

# virtualenv dir
VENV_DIR := .venv

# script to run before build
SCRIPT := tools$(SEP)target.py

# default target
.PHONY: all
all: prebuild $(BUILD)/$(PROJECT).bin
	@echo -----------------------ignore the build filename ext.^^

# Prebuild: ensure uv venv exists and run target script
.PHONY: prebuild
prebuild: setup
	@echo $(SCRIPT)
ifeq ($(OS),Windows_NT)
	@$(PYTHON) -m $(UV) run "$(SCRIPT)"
else
	@$(UV) run "$(SCRIPT)"
endif

# Setup virtual environment and dependencies (idempotent)
.PHONY: setup
ifeq ($(OS),Windows_NT)
setup:
	@$(WHERE) $(UV) >NUL 2>NUL || ( \
	  echo UV not found. Please install it: https://docs.astral.sh/uv/ && exit /b 1 )
	@if exist "$(VENV_DIR)" ( \
	  echo Virtual environment already present. ) else ( \
	  echo Setting up virtual environment... & \
	  $(UV) venv & \
	  echo Installing dependencies... & \
	  $(UV) pip install --upgrade pip & \
	  $(UV) sync )
else
setup:
	@if [ -z "$$($(WHERE) $(UV))" ]; then \
	  echo "UV not found. Please install it: https://docs.astral.sh/uv/"; exit 1; \
	fi
	@if [ -d "$(VENV_DIR)" ]; then \
	  echo "Virtual environment already present."; \
	else \
	  echo "Setting up virtual environment..."; \
	  $(UV) venv; \
	  echo "Installing dependencies..."; \
	  $(UV) pip install --upgrade pip; \
	  $(UV) sync; \
	fi
endif

# optional: add include dir to compile flags if your mkrules doesn't already
CPPFLAGS += -I$(INC_F)

# ensure objects depend on headers and this Makefile
$(OBJ): $(SRC_H) $(SRC_A)

# include build rules last
include vex$(SEP)mkrules.mk
