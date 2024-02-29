SRCDIR := src
LIBDIR := lib
BINDIR := bin
OBJDIR := obj
INCDIR := include

test: librle $(SRCDIR)/rledemo.c
	gcc -o $(BINDIR)/rledemo $(SRCDIR)/rledemo.c -I./$(INCDIR) -L./$(LIBDIR) -lrle

librle: dirs $(OBJDIR)/librle.o
	ar rcs $(LIBDIR)/librle.a $(OBJDIR)/librle.o

$(OBJDIR)/librle.o : $(SRCDIR)/librle.c $(INCDIR)/librle.h
	gcc -c -o $(OBJDIR)/librle.o $(SRCDIR)/librle.c -I./$(INCDIR)

dirs:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)

.PHONY: clean

clean:
	rm -rf $(BINDIR)/*
	rm -rf $(OBJDIR)/*
	rm -rf $(LIBDIR)/*