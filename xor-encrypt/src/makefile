NAME=xorencrypt
OBJ_DIR=obj
INC_DIR=../../include/.
APP_INC_DIR=$(INC_DIR)/xor-encrypt/.
BIN_DIR=../bin/.
LIB_DIR=../../lib/.

DEBUG=0
CC=clang
CFLAGS=-I$(INC_DIR) -I$(APP_INC_DIR)

ifeq ($(DEBUG),0)
	DBGFLAG=
else
	DBGFLAG=-g
endif

INC_FILES=$(APP_INC_DIR)/xor-encrypt.h $(INC_DIR)/hex.h

_EXE_OBJS=xor-encrypt.o main.o
EXE_OBJS=$(patsubst %,$(OBJ_DIR)/%,$(_EXE_OBJS))

LIB_OBJS=$(OBJ_DIR)/xor-encrypt.o

$(OBJ_DIR)/%.o: %.c $(INC_FILES)
	$(CC) -c -o $@ $< $(CFLAGS) $(DBGFLAG)

exe: $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(EXE_OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(LIB_DIR)/libxor.a: $(LIB_OBJS)
	ar -vr $@ $(LIB_OBJS)

lib: $(LIB_DIR)/libxor.a

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o
