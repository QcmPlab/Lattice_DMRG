FC=mpif90

SF_INC:=$(shell pkg-config --cflags scifor)
SF_LIB:=$(shell pkg-config --libs scifor)

TYPE=DBLE

FLAG= -cpp -D_DEBUG -D_$(TYPE) -O0 -p -g -Wsurprising -Waliasing -fwhole-file -fcheck=all -fbacktrace -fbounds-check  -ffree-line-length-none -fPIC -w -fallow-argument-mismatch -fopenmp


define VER
MODULE VERSION
  implicit none
  character(len=41),parameter :: git_code_version = "$(shell git rev-parse HEAD)"	
END MODULE VERSION
endef
export VER


SRC=..
DOTS=../DOTS
FOCK=../FOCK
LIST=../LIST
MATRIX=../MATRIX


test: sites blocks


sites: MYOBJ=VERSION.o INPUT_VARS.o AUX_FUNCS.o MATRIX_SPARSE.o QN_TUPLE_BASIS.o LIST_OPERATORS.o LIST_SECTORS.o HLOCAL.o 
sites: VERSION.o INPUT_VARS.o AUX_FUNCS.o MATRIX_SPARSE.o QN_TUPLE_BASIS.o LIST_OPERATORS.o LIST_SECTORS.o HLOCAL.o 
	${FC} $(FLAG) -D_TEST ${MYOBJ} SITES.f90 -o $(HOME)/.bin/testSITES ${SF_INC} ${SF_LIB}

blocks: MYOBJ=VERSION.o INPUT_VARS.o AUX_FUNCS.o HLOCAL.o MATRIX_SPARSE.o MATRIX_BLOCKS.o QN_TUPLE_BASIS.o LIST_SECTORS.o LIST_OPERATORS.o  SITES.o
blocks: VERSION.o INPUT_VARS.o AUX_FUNCS.o HLOCAL.o MATRIX_SPARSE.o MATRIX_BLOCKS.o QN_TUPLE_BASIS.o LIST_SECTORS.o LIST_OPERATORS.o  SITES.o
	${FC} $(FLAG) -D_TEST ${MYOBJ} BLOCKS.f90 -o $(HOME)/.bin/testBLOCKS ${SF_INC} ${SF_LIB}



VERSION.o:
	@echo "$$VER" > $(SRC)/VERSION.f90
	$(FC) $(FLAG) $(SF_INC) -c $(SRC)/VERSION.f90

INPUT_VARS.o: 
	$(FC) $(FLAG) $(SF_INC) -c $(SRC)/INPUT_VARS.f90

AUX_FUNCS.o: 
	$(FC) $(FLAG) $(SF_INC) -c $(SRC)/AUX_FUNCS.f90

HLOCAL.o: 
	$(FC) $(FLAG) $(SF_INC) -c $(FOCK)/HLOCAL.f90

MATRIX_SPARSE.o: 
	$(FC) $(FLAG) $(SF_INC) -c $(MATRIX)/MATRIX_SPARSE.f90

MATRIX_BLOCKS.o: 
	$(FC) $(FLAG) $(SF_INC) -c $(MATRIX)/MATRIX_BLOCKS.f90

QN_TUPLE_BASIS.o: 
	$(FC) $(FLAG) $(SF_INC) -c $(LIST)/QN_TUPLE_BASIS.f90

LIST_SECTORS.o: 
	$(FC) $(FLAG) $(SF_INC) -c $(LIST)/LIST_SECTORS.f90

LIST_OPERATORS.o:
	$(FC) $(FLAG) $(SF_INC) -c $(LIST)/LIST_OPERATORS.f90

SITES.o:
	$(FC) $(FLAG) $(SF_INC) -c $(DOTS)/SITES.f90

clean: 
	@echo "Cleaning:"
	@rm -fv *.mod *.o *~ 
	@echo ""


.f90.o:	
	$(FC) $(FLAG) $(SF_INC) -c $< 
