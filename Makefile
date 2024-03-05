SRCDIR := src
LIBDIR := lib
BINDIR := bin
OBJDIR := obj
INCDIR := include

GCC := gcc -Wall -g
ARC := ar rcs

testrle: libfile librle $(SRCDIR)/rledemo.c
	$(GCC) -o $(BINDIR)/rledemo $(SRCDIR)/rledemo.c -I./$(INCDIR) -L./$(LIBDIR) -lrle -lfile

librle: $(OBJDIR)/librle.o
	$(ARC) $(LIBDIR)/librle.a $(OBJDIR)/librle.o

$(OBJDIR)/librle.o : dirs $(SRCDIR)/librle.c $(INCDIR)/librle.h
	$(GCC) -c -o $(OBJDIR)/librle.o $(SRCDIR)/librle.c -I./$(INCDIR)

libfile: $(OBJDIR)/libfile.o
	$(ARC) $(LIBDIR)/libfile.a $(OBJDIR)/libfile.o

$(OBJDIR)/libfile.o : dirs $(SRCDIR)/libfile.c $(INCDIR)/libfile.h
	$(GCC) -c -o $(OBJDIR)/libfile.o $(SRCDIR)/libfile.c -I./$(INCDIR)

testhuff: libfile libhuffman $(SRCDIR)/huffdemo.c
	$(GCC) -o $(BINDIR)/huffdemo $(SRCDIR)/huffdemo.c -I./$(INCDIR) -L./$(LIBDIR) -lfile -lhuffman

libhuffman: $(OBJDIR)/libhuffman.o
	$(ARC) $(LIBDIR)/libhuffman.a $(OBJDIR)/libhuffman.o

$(OBJDIR)/libhuffman.o : dirs $(SRCDIR)/libhuffman.c $(INCDIR)/libhuffman.h
	$(GCC) -c -o $(OBJDIR)/libhuffman.o $(SRCDIR)/libhuffman.c -I./$(INCDIR)

libcodetable: $(OBJDIR)/libcodetable.o
	$(ARC) $(LIBDIR)/libcodetable.a $(OBJDIR)/libcodetable.o

$(OBJDIR)/libcodetable.o: dirs $(SRCDIR)/libcodetable.c $(INCDIR)/libcodetable.h
	$(GCC) -c -o $(OBJDIR)/libcodetable.o $(SRCDIR)/libcodetable.c -I./$(INCDIR)
	
dirs:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)

.PHONY: clean

clean:
	rm -rf $(BINDIR)/*
	rm -rf $(OBJDIR)/*
	rm -rf $(LIBDIR)/*