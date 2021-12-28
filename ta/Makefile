ifneq ($(TEST)$(DEV), 0ra)

SRC = $(TA)ta $(CRYPTOGRAPHY)aes $(CRYPTOGRAPHY)keys \
		$(CRYPTOGRAPHY)dh $(CRYPTOGRAPHY)rsa $(DEVICE)device \
		$(DEVICE)flist $(DEVICE)alist $(TASK)task $(INIT)init \
		$(CERTIFICATION)certification $(JOIN)join_outgoing \
		$(JOIN)join_incoming $(STABILIZE)stabilize_outgoing \
		$(STABILIZE)stabilize_incoming $(RECTIFY)rectify_outgoing \
		$(RECTIFY)rectify_incoming $(ATTEST)attest_outgoing \
		$(ATTEST)attest_incoming

OBJ = $(foreach src, $(SRC), $(OUT)$(shell basename $(src))_$(DEV).o)

all: $(OBJ)

define COMPILE
$(OUT)$(shell basename $(src))_$(DEV).o: $(src).c
	$(GCC) $(FLAGS) -DTEST=$(TEST) $(INCL) -c $$< -o $$@
endef

$(foreach src, $(SRC), $(eval $(COMPILE)))

else

CFG_TEE_TA_LOG_LEVEL ?= 4
CPPFLAGS += -DCFG_TEE_TA_LOG_LEVEL=$(CFG_TEE_TA_LOG_LEVEL) -DENV=0 -DTEST=0
CPPFLAGS += -I$(GLOBAL)

# The UUID for the Trusted Application
BINARY=bcd075c3-3ea6-4e63-9177-c508712fcb14
TA_DEV_KIT_DIR=$(OPTEE)optee_os/out/arm/export-ta_arm64

-include $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk

ifeq ($(wildcard $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk), )
clean:
	@echo 'Note: $$(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk not found, cannot clean TA'
	@echo 'Note: TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR)'
endif

endif