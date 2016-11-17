IDIR = ./include
ODIR = ./src/obj
SRCDIR = ./src
BUILDDIR = ./build

CC = gcc
CFLAGS = -I$(IDIR)

_OBJ = $(patsubst $(SRCDIR)/%.c,%.o,$(wildcard $(SRCDIR)/*.c) )#main.o util.o ethernet.o arp.o



#_DEPS = util.h ethernet.h common.h arp.h

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ) )

#$(info $$_OBJ is [${_OBJ}])

#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

DEPS = $(wildcarad $(IDIR)/%.h)


$(ODIR)/%.o : $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


$(BUILDDIR)/atcp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
		rm -f $(ODIR)/*.o $(BUILDDIR)/atcp
