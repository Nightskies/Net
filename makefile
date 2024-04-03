CFLAGS=-Wall
EXE=net

SRC_DIRS := .

SOURCES := $(shell find $(SRC_DIRS) -name '*.c')
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXE) $(OBJECTS)