SRC = $(GLOBAL)global $(LIST)list $(OPERATION)operation

OBJ = $(foreach src, $(SRC), $(OUT)$(shell basename $(src))_$(DEV).o)


all: $(OBJ) crypto

crypto:
	$(MAKE) -C mbedtls --no-builtin-variables

define COMPILE
$(OUT)$(shell basename $(src))_$(DEV).o: $(src).c
	$(GCC) $(FLAGS) $(INCL) -c $$< -o $$@
endef

$(foreach src, $(SRC), $(eval $(COMPILE)))