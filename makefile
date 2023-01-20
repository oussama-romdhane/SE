CC=gcc
SRCDIR=src
BUILDDIR=build
OBJDIR=obj

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

EXECUTABLE=shell

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	mkdir -p build
	$(CC) -o $(BUILDDIR)/$@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p obj
	$(CC) -c -o $@ $<

permissions:
	chmod u+rx $(SOURCES)
	chmod u+rx $(OBJECTS)

help:
	@echo "src: $(SOURCES)"
	@echo "object: $(wildcard $(OBJECTS)/*)"
	@echo "Build: $(wildcard $(BUILDDIR)/*)"

clean:
	rm -f $(OBJDIR)/*.o $(BUILDDIR)/$(EXECUTABLE)