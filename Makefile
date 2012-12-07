
CC = g++
CFLAGS = -Wall
LDFLAGS = -lpthread -lm
LIBDIR = ./lib
BINDIR = ./bin
SRCDIR = ./src
BINFILE = otimiza

all: $(LIBDIR) $(BINDIR) $(BINDIR)/$(BINFILE)

rebuild: clean all

clean:
	rm -rf $(LIBDIR)/*.o $(BINDIR)/$(BINFILE)

run: all
	@$(BINDIR)/$(BINFILE) 109 1010 < tests/DadosUsina1.txt

test: all
	@time $(BINDIR)/$(BINFILE) 109 1010 < tests/DadosUsina1.txt

$(LIBDIR):
	mkdir -p $(LIBDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/$(BINFILE): $(LIBDIR)/main.o $(LIBDIR)/solver.o $(LIBDIR)/state.o $(LIBDIR)/powers.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(LIBDIR)/main.o: $(SRCDIR)/main.cpp
	$(CC) -o $@ -c $(CFLAGS) $<

$(LIBDIR)/state.o: $(SRCDIR)/state.cpp $(SRCDIR)/state.h
	$(CC) -o $@ -c $(CFLAGS) $<

$(LIBDIR)/solver.o: $(SRCDIR)/solver.cpp $(SRCDIR)/solver.h
	$(CC) -o $@ -c $(CFLAGS) $<

$(LIBDIR)/powers.o: $(SRCDIR)/powers.cpp $(SRCDIR)/powers.h
	$(CC) -o $@ -c $(CFLAGS) $<

