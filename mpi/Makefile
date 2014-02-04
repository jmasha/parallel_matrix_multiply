CC = mpicc
OPT = -o3 -Wall
OPT2 = -g -Wall

OSTYPE := $(shell uname)

#
# LINUX SECTION
#
ifneq (,$(findstring Linux,$(OSTYPE)))
ARCH_OPT = -DHAVE_MMAP
PIC = -fpic
EXE_EXT =
endif

#
# MAC OSX SECTION
#
ifneq (,$(findstring Darwin,$(OSTYPE)))
ARCH_OPT = -DHAVE_MMAP -arch x86_64
PIC = -fPIC
EXE_EXT =
endif


#----------------------------------------------------------------------
# CFLAGS AND LIBS
#----------------------------------------------------------------------
CFLAGS = $(ARCH_OPT) $(OPT)

MATRIX_SEQ = $(addsuffix $(EXE_EXT), matrix_mult_seq)
MATRIX_PAR = $(addsuffix $(EXE_EXT), matrix_mult_par)

all: $(MATRIX_PAR)

clean: 
	rm -f *.o
	rm -f *.*~
	rm -f $(MATRIX_SEQ)
	rm -f $(MATRIX_PAR)

#seq target
matrix_mult_seq$(EXE_EXT): matrix_mult_seq.c matrix_mult_seq.h matrix_mult_common.h
	$(CC) $(CFLAGS) matrix_mult_seq.c matrix_mult_seq.h matrix_mult_common.h -o $@
	chmod 755 $@

#par target
matrix_mult_par$(EXE_EXT): matrix_mult_par.c matrix_mult_par.h matrix_mult_common.h
	$(CC) $(CFLAGS) matrix_mult_par.c matrix_mult_par.h matrix_mult_common.h -o $@
	chmod 755 $@
