CC=gcc
CFLAGS=-fPIC
CFLAGS_SHARED=-shared
OBJ=mma8451.o
LIBNAME=libmma8451.so
TESTOBJ=mma8451-test.o
TESTNAME=mma8451-test

all: compile

compile: $(LIBNAME) $(TESTNAME)

install: $(LIBNAME)
	cp $(LIBNAME) /usr/lib/$(LIBNAME)
	cp $(TESTNAME) /usr/bin/$(TESTNAME)

fix-i2c:
	echo -n 1 > /sys/module/i2c_bcm2708/parameters/combined

clean:
	rm -f $(OBJ) $(TESTOBJ) $(LIBNAME) $(TESTNAME)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIBNAME): $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(CFLAGS_SHARED)

$(TESTNAME): $(LIBNAME) $(TESTOBJ)
	gcc -o $@ $^ $(CFLAGS) -L. -lmma8451