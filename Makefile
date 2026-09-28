SRC := src
OBJ := obj
BIN := bin
EXECUTABLE := shell

SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))

CC := gcc
INCLUDE := -Iinclude

all: $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(OBJS) | $(BIN)
	$(CC) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(INCLUDE) -c $< -o $@

$(OBJ):
	mkdir -p $(OBJ)

$(BIN):
	mkdir -p $(BIN)

run: $(BIN)/$(EXECUTABLE)
	./$(BIN)/$(EXECUTABLE)

clean:
	rm -f $(OBJ)/*.o $(BIN)/$(EXECUTABLE)

.PHONY: all run clean
