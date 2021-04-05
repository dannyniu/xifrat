LDFLAGS = `pkg-config --libs openssl`
CFLAGS = `pkg-config --cflags openssl`
OBJS = sign.o rng.o PQCgenKAT_sign.o \
../../src/xifrat-sign.o \
../../src/xifrat-primitives.c \
../../src-crypto/shake.c \
../../src-crypto/sponge.c \
../../src-crypto/keccak-f-1600.c \
../../src-crypto/endian.c

PQCgenKAT_sign : $(OBJS)
	gcc $(LDFLAGS) $(CFLAGS) -o $@ $^

%.o : %.c
	gcc $(CFLAGS) -o $@ -c $<

rng.o : rng.h
api.o : api.h

PQCgenKAT_sign.o : rng.h api.h 
