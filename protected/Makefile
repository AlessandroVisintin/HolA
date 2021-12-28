SRC = $(PROTECTED)my_function

OBJ = $(foreach src, $(SRC), $(OUT)$(shell basename $(src))_$(DEV).o)

all: $(OBJ)

define COMPILE
$(OUT)$(shell basename $(src))_$(DEV).o: $(src).c
	$(GCC) $(FLAGS) $(INCL) -c $$< -o $$@
endef

$(foreach src, $(SRC), $(eval $(COMPILE)))