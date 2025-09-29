#
# 3dfx.linux.mak - Standard definitions for 3Dfx Makefiles on Linux
#

#
# Compiler/linker
#
CC = gcc
LD = ld
AR = ar
AS = as

#
# Standard flags
#
CFLAGS_STD = -fPIC -Wall -m64
LDFLAGS_STD = -shared

#
# Debug flags
#
ifeq ($(DEBUG),1)
CFLAGS_DBG = -g
LDFLAGS_DBG = -g
else
CFLAGS_DBG =
LDFLAGS_DBG =
endif

#
# Optimization flags
#
ifeq ($(DEBUG),1)
CFLAGS_OPT =
else
CFLAGS_OPT = -O2 -ffast-math -msse2
endif

#
# Final flags
#
CFLAGS = $(CFLAGS_STD) $(CFLAGS_DBG) $(CFLAGS_OPT) $(DEFS) $(INCLUDES)
LDFLAGS = $(LDFLAGS_STD) $(LDFLAGS_DBG)

#
# Standard rules
#
.SUFFIXES: .c .o .S

.c.o:
	$(CC) $(CFLAGS) -c $<

.S.o:
	$(CC) $(CFLAGS) -c $<

#
# Phony targets
#
.PHONY: all clean realclean

#
# Default rule
#
all:
	@echo "No default target."

#
# Cleanup rules
#
clean:
	@echo "No generic clean rule."

realclean: clean
	@echo "No generic realclean rule."