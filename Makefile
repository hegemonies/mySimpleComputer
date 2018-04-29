BIN_NAME = msc

SRC_PATH = src
BUILD_PATH = build
BIN_PATH = bin
LIB_PATH = lib
INCLUDE_PATH_FLAGS = -I src/include
LIB_FLAG = -L

SRC_EXT = c

CC = gcc

COMPILE_FLAGS = -std=c99 -Wall -Werror -O0 -g

SOURCES = $(shell find $(SRC_PATH)/ -name '*.$(SRC_EXT)')

OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)

LIB_SOURCES = $(filter-out $(SRC_PATH)/main.c, $(SOURCES))

LIBS = $(LIB_SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(LIB_PATH)/lib%.a)

LLIBS = $(filter-out -lhelper, $(LIBS:$(LIB_PATH)/lib%.a=-l%))

all: makedirs main
	
main: $(BUILD_PATH)/main.o $(LIBS)
	$(CC) $(COMPILE_FLAGS) $(LIB_FLAG)$(LIB_PATH) $(BUILD_PATH)/main.o -o $(BIN_PATH)/$(BIN_NAME) -lhelper $(LLIBS)

$(BUILD_PATH)/main.o: $(SRC_PATH)/main.$(SRC_EXT)
	$(CC) $(COMPILE_FLAGS) $(INCLUDE_PATH_FLAGS) $< -c -o $@

$(LIB_PATH)/lib%.a : $(BUILD_PATH)/%.o
	ar rcs $@ $^

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	$(CC) $(COMPILE_FLAGS) $(INCLUDE_PATH_FLAGS) $< -c -o $@

$(BIN_PATH)/asm: $(BUILD_PATH)/asm.o $(LIBS)
	$(CC) $(COMPILE_FLAGS) $(LIB_FLAG)$(LIB_PATH) $(INCLUDE_PATH_FLAGS) $< -o $(BIN_PATH)/sat -lhelper $(LLIBS)

$(BUILD_PATH)/asm.o: $(SRC_PATH)/asm/asm.$(SRC_EXT)
	$(CC) $(COMPILE_FLAGS) $(INCLUDE_PATH_FLAGS) $< -c -o $@

makedirs:
	@mkdir $(BIN_PATH) -p
	@mkdir $(BUILD_PATH) -p
	@mkdir $(BUILD_PATH)/asm -p
	@mkdir $(LIB_PATH) -p
	@mkdir $(LIB_PATH)/libasm -p

.PHONY:

clean:
	$(shell printf '\043 Cleaned \043')
	@rm -rf $(BIN_PATH)
	@rm -rf $(BUILD_PATH)
	@rm -rf $(LIB_PATH)

run:
	@clear
	@bin/$(BIN_NAME)

tog:
	@make clean
	@make
	@make run

asm: makedirs $(BIN_PATH)/asm
