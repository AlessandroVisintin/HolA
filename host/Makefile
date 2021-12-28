SRC = $(HOST)host $(NETWORK)network $(NETWORK)client \
 		$(NETWORK)server $(SETUP)address $(SETUP)setup \
 		$(GATEWAY)gateway $(NETWORK)monitor

OBJ = $(foreach src, $(SRC), $(OUT)$(shell basename $(src))_$(DEV).o)

all: $(OBJ)

define COMPILE
$(OUT)$(shell basename $(src))_$(DEV).o: $(src).c
	$(GCC) $(FLAGS) -DTEST=$(TEST) $(INCL) -c $$< -o $$@
endef

$(foreach src, $(SRC), $(eval $(COMPILE)))