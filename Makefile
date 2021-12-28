export V ?= 0

ifneq ($(MAKECMDGOALS), clean)
ifndef DEV
$(error DEV is not set ( make DEV={pc/ra/do} ))
endif
export DEV
endif

ifeq ($(MAKECMDGOALS), parser)
ifeq ($(DEV), ra)
$(error parser requires DEV=pc ( make parser DEV=pc ))
endif
endif

export HOME = /home/flavio/
export ROOT = $(HOME)HiDRA/
export OPTEE = $(HOME)devel/optee-A/
export CROSS = $(OPTEE)toolchains/aarch64/bin/aarch64-linux-gnu-

export GLOBAL = $(ROOT)global/
export PROTECTED = $(ROOT)protected/
export HOST = $(ROOT)host/
export OUT = $(ROOT)out/
export TA = $(ROOT)ta/
export PARSER = $(ROOT)parser/

export OPERATION = $(GLOBAL)operation/
export BUFFER = $(GLOBAL)buffer/
export LIST = $(GLOBAL)list/

export GATEWAY = $(HOST)gateway/
export NETWORK = $(HOST)network/
export SETUP = $(HOST)setup/

export TEST = $(OUT)test/
export LIB = $(OUT)lib/

export CRYPTOGRAPHY = $(TA)cryptography/
export DEVICE = $(TA)device/
export TASK = $(TA)task/
export INIT = $(TASK)init/
export CERTIFICATION = $(TASK)certification/
export JOIN = $(TASK)join/
export STABILIZE = $(TASK)stabilize/
export RECTIFY = $(TASK)rectify/
export ATTEST = $(TASK)attest/

export POUT = $(PARSER)out/
export PARTICIPANT = $(PARSER)participant/
export FLOW = $(PARSER)flow/

ifeq ($(DEV), ra)

TEEC_EXPORT = $(OPTEE)optee_client/out/export

export GCC = $(CROSS)gcc
export FLAGS = -Wall -pedantic -DENV=0
export INCL = -I$(GLOBAL) -I$(TEEC_EXPORT)/include
LIBR = -L$(TEEC_EXPORT)/lib -lteec -L$(OUT)lib -lcryptora -lpthread

else

export GCC = gcc
export INCL = -I$(GLOBAL)
LIBR = -L$(OUT)lib -lcryptopc -lpthread

ifeq ($(DEV), pc)

export FLAGS = -Wall -pedantic -DENV=1

else

export FLAGS = -Wall -pedantic -DENV=2

endif

endif

OBJS = $(wildcard $(OUT)*_$(DEV).o)
OBJP = $(wildcard $(POUT)*.o)

# Tests / Main
T0 = $(TEST)buffer_$(DEV)_test.o
T1 = $(TEST)list_$(DEV)_test.o
T2 = $(TEST)operation_$(DEV)_test.o
T3 = $(TEST)network_$(DEV)_test.o
T4 = $(TEST)setup_$(DEV)_test.o
T5 = $(TEST)cryptography_$(DEV)_test.o
T6 = $(TEST)device_$(DEV)_test.o

main: nobj
	$(GCC) -o $(OUT)main_$(DEV) $(OBJS) $(LIBR)

parse: gdir padir
	$(GCC) -o $(POUT)parser $(OUT)global_pc.o $(OUT)list_pc.o $(OBJP) $(LIBR)

extract: gdir
	$(GCC) $(FLAGS) $(INCL) -c $(PARSER)extract.c -o $(POUT)extract.o
	$(GCC) -o $(POUT)extract $(POUT)extract.o $(OUT)global_pc.o $(LIBR)

reader: gdir
	$(GCC) $(FLAGS) $(INCL) -c $(PARSER)reader.c -o $(POUT)reader.o
	$(GCC) -o $(POUT)reader $(POUT)reader.o $(OUT)global_pc.o $(LIBR)

buffer_test: tobj $(T0)
	$(GCC) -o $(TEST)buffer_$(DEV)_test $(T0) $(OBJS) $(LIBR)

list_test: tobj $(T1)
	$(GCC) -o $(TEST)list_$(DEV)_test $(T1) $(OBJS) $(LIBR)

operation_test: tobj $(T2)
	$(GCC) -o $(TEST)operation_$(DEV)_test $(T2) $(OBJS) $(LIBR)

network_test: tobj $(T3)
	$(GCC) -o $(TEST)network_$(DEV)_test $(T3) $(OBJS) $(LIBR)

setup_test: tobj $(T4)
	$(GCC) -o $(TEST)setup_$(DEV)_test $(T4) $(OBJS) $(LIBR)

cryptography_test: tobj $(T5)
	$(GCC) -o $(TEST)cryptography_$(DEV)_test $(T5) $(OBJS) $(LIBR)

device_test: tobj $(T6)
	$(GCC) -o $(TEST)device_$(DEV)_test $(T6) $(OBJS) $(LIBR)

tobj: gdir htdir pdir ttdir

nobj: gdir hndir pdir tndir

gdir:
	$(MAKE) -C global --no-builtin-variables

htdir:
	$(MAKE) -C host TEST="1" --no-builtin-variables

hndir:
	$(MAKE) -C host TEST="0" --no-builtin-variables

pdir:
	$(MAKE) -C protected --no-builtin-variables

ttdir:
	$(MAKE) -C ta TEST="1" --no-builtin-variables

tndir:
	$(MAKE) -C ta CROSS_COMPILE="$(CROSS)" TEST="0" --no-builtin-variables

padir:
	$(MAKE) -C parser --no-builtin-variables

$(T0): $(BUFFER)test.c
	$(GCC) $(FLAGS) $(INCL) -c $< -o $@

$(T1): $(LIST)test.c
	$(GCC) $(FLAGS) $(INCL) -c $< -o $@

$(T2): $(OPERATION)test.c
	$(GCC) $(FLAGS) $(INCL) -c $< -o $@

$(T3): $(NETWORK)test.c
	$(GCC) $(FLAGS) $(INCL) -c $< -o $@

$(T4): $(SETUP)test.c
	$(GCC) $(FLAGS) $(INCL) -c $< -o $@

$(T5): $(CRYPTOGRAPHY)test.c
	$(GCC) $(FLAGS) -DTEST=1 $(INCL) -c $< -o $@

$(T6): $(DEVICE)test.c
	$(GCC) $(FLAGS) -DTEST=1 $(INCL) -c $< -o $@

clean:
	rm -f $(OUT)*.o $(OUT)main* $(TEST)* $(POUT)*
	$(MAKE) -C ta TEST=0 DEV=ra clean
