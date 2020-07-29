INSDIR=/usr/lib/geany/
CC = gcc
CFLAGS = `pkg-config --cflags --libs geany` -fPIC
LDFLAGS = -shared
RM = rm
CP = cp

qop.so: qop.o
	$(CC) -o $@ $^ $(LDFLAGS)

qop.o: qop.c
	$(CC) -c $^ $(CFLAGS)

.PHONY: all

all: qop.so

clean:
	$(RM) -f *.o *.so

install:
	$(CP) -f qop.so $(INSDIR)
