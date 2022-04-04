LDFLAGS = `pkg-config --libs openssl`
CFLAGS = `pkg-config --cflags openssl`
OBJS = sign.o rng.o PQCgenKAT_sign.o \
../../src/xifrat-funcs.o \
../../src/xifrat-sign.o \
../../src-crypto/shake.o \
../../src-crypto/sponge.o \
../../src-crypto/keccak-f-1600.o \
../../src-crypto/endian.o

PQCgenKAT_sign : $(OBJS)
	gcc $(LDFLAGS) $(CFLAGS) -o $@ $^

%.o : %.c
	gcc $(CFLAGS) -o $@ -c $<

rng.o : rng.h
api.o : api.h

PQCgenKAT_sign.o : rng.h api.h 
