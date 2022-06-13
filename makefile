NAME=crypto
OBJ_DIR=obj
INC_DIR=include
BIN_DIR=bin
LIB_DIR=lib
SRC_DIR=src
SRCS=$(wildcard $(SRC_DIR)/*c)
OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

DEBUG=0
CC=clang
CFLAGS=-I$(INC_DIR)# -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls

ifeq ($(DEBUG),0)
	DBGFLAG=
else
	DBGFLAG=-g -O1
endif

LIB=$(LIB_DIR)/crypto.a

$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) -c $< $(CFLAGS) $(DBGFLAG) -o $@

lib: mkdirs $(LIB)

$(LIB): $(OBJS)
	ar -vr $@ $?

mkdirs:
	mkdir -p $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)

base64: lib
	$(CC) -o $(BIN_DIR)/$@ src/main/base64-main.c $(CFLAGS) $(LIB)

xor-crack: lib
	$(CC) -o $(BIN_DIR)/$@ src/main/xor-crack-main.c $(CFLAGS) $(LIB)

xor-detect-single: lib
	$(CC) -o $(BIN_DIR)/$@ src/main/xor-detect-single-main.c $(CFLAGS) $(LIB)

xor-encrypt: lib
	$(CC) -o $(BIN_DIR)/$@ src/main/xor-encrypt-main.c $(CFLAGS) $(LIB)

xor-encrypt-repeat: lib
	$(CC) -o $(BIN_DIR)/$@ src/main/xor-encrypt-repeat-main.c $(CFLAGS) $(LIB)

xor-crack-repeat: lib
	$(CC) -o $(BIN_DIR)/$@ src/main/xor-crack-repeat-main.c $(CFLAGS) $(DBGFLAG) $(LIB)

read-data: lib
	$(CC) -o $(BIN_DIR)/$@ src/main/io-main.c $(CFLAGS) $(DBGFLAG) $(LIB)

all: lib base64 xor-crack xor-detect-single xor-encrypt xor-encrypt-repeat xor-crack-repeat read-data

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o
