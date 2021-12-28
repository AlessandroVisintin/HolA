S0 = aes aesni arc4 aria asn1parse asn1write
S1 = base64 bignum blowfish camellia ccm certs
S3 = chacha20 chachapoly cipher cipher_wrap cmac
S4 = ctr_drbg des ecdsa ecp ecp_curves entropy
S5 = entropy_poll gcm havege hmac_drbg md md5
S6 = md_wrap oid padlock pem pk pk_wrap
S7 = pkcs11 pkcs12 pkcs5 pkparse platform
S8 = platform_util poly1305 ripemd160 rsa
S9 = rsa_internal sha1 sha256 sha512 threading
S10 = timing x509 x509_crl x509_crt

S01 = $(S0) $(S1) $(S2) $(S3) $(S4) $(S5)
S02 = $(S6) $(S7) $(S8) $(S9) $(S10)

N = $(S01) $(S02)

define COMPILE
$(src)_$(DEV).o: $(src).c
	$(GCC) -c $$< -o $$@
endef

OBJS = $(foreach src, $(N), $(src)_$(DEV).o)

ifeq ($(DEV), ra)

all: $(OBJS) 
	$(CROSS)ar rcs $(LIB)libcryptora.a $(OBJS)

else

all: $(OBJS) 
	ar rcs $(LIB)libcryptopc.a $(OBJS)

endif

$(foreach src, $(N), $(eval $(COMPILE)))

clean:
	rm -f *.o