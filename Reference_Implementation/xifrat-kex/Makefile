LDFLAGS = `pkg-config --libs openssl`
CFLAGS = `pkg-config --cflags openssl`
OBJS = kex.o rng.o PQCgenKAT_kem.o \
../../src/xifrat-kex.o \
../../src/xifrat-primitives.c \
../../src-crypto/shake.c \
../../src-crypto/sponge.c \
../../src-crypto/keccak-f-1600.c \
../../src-crypto/endian.c

PQCgenKAT_kem : $(OBJS)
	gcc $(LDFLAGS) $(CFLAGS) -o $@ $^

%.o : %.c
	gcc $(CFLAGS) -o $@ -c $<

rng.o : rng.h
api.o : api.h

PQCgenKAT_kem.o : rng.h api.h 
