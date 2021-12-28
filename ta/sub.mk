global-incdirs-y += ./
srcs-y += ../global/global.c ../global/list/list.c ../global/operation/operation.c 
srcs-y += ta.c
srcs-y += cryptography/aes.c cryptography/dh.c cryptography/keys.c cryptography/rsa.c
srcs-y += device/alist.c device/flist.c device/device.c
srcs-y += task/task.c task/init/init.c task/certification/certification.c
srcs-y += task/join/join_outgoing.c task/join/join_incoming.c
srcs-y += task/stabilize/stabilize_outgoing.c task/stabilize/stabilize_incoming.c
srcs-y += task/rectify/rectify_outgoing.c task/rectify/rectify_incoming.c
srcs-y += task/attest/attest_outgoing.c task/attest/attest_incoming.c
 
# To remove a certain compiler flag, add a line like this
#cflags-template_ta.c-y += -Wno-strict-prototypes