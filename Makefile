IDIR = ./include
ODIR = ./src/obj
SRCDIR = ./src
BUILDDIR = ./build

CC = gcc
CFLAGS = -I$(IDIR)

_OBJ = main.o util.o ethernet.o
_DEPS = util.h ethernet.h common.h

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ) )
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))


$(ODIR)/%.o : $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


$(BUILDDIR)/atcp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
		rm -f $(ODIR)/*.o $(BUILDDIR)/atcp
