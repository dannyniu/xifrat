LDFLAGS = `pkg-config --libs openssl`
CFLAGS = `pkg-config --cflags openssl`
OBJS = kex.o rng.o PQCgenKAT_kem.o \
../../src/xifrat-kex.o \
../../src/xifrat-funcs.o \
../../src-crypto/shake.o \
../../src-crypto/sponge.o \
../../src-crypto/keccak-f-1600.o \
../../src-crypto/endian.o

PQCgenKAT_kem : $(OBJS)
	gcc $(LDFLAGS) $(CFLAGS) -o $@ $^

%.o : %.c
	gcc $(CFLAGS) -o $@ -c $<

rng.o : rng.h
api.o : api.h

PQCgenKAT_kem.o : rng.h api.h 
