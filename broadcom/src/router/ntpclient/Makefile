# Under Solaris, you need to 
#    CFLAGS += -xO2 -Xc
#    LDLIBS += -lnsl -lsocket
# Some versions of Linux may need
#    CFLAGS += -D_GNU_SOURCE
# To cross-compile
#    CC = arm-linux-gcc

CFLAGS += -Wall -O2
CFLAGS +=-I$(SRCBASE)/include
LDFLAGS += -L$(TOP)/nvram -lnvram

all: ntpclient

test: ntpclient
	./ntpclient -d -r <test.dat

ntpclient: ntpclient.o phaselock.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

install: ntpclient
	$(STRIP) ntpclient
	install -d $(INSTALLDIR)/usr/sbin
	install ntpclient $(INSTALLDIR)/usr/sbin

clean:
	rm -f ntpclient *.o
