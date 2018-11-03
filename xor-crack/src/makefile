NAME=xorcrack
OBJ_DIR=obj
INC_DIR=../../include/.
APP_INC_DIR=$(INC_DIR)/xor-crack/.
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

_INC_FILES=score.h xor-crack.h xor-crack-types.h
INC_FILES=$(patsubst %,$(APP_INC_DIR)/%,$(_INC_FILES)) $(INC_DIR)/hex.h

_EXE_OBJS=score.o xor-crack.o main.o
EXE_OBJS=$(patsubst %,$(OBJ_DIR)/%,$(_EXE_OBJS))

_LIB_OBJS=score.o xor-crack.o
LIB_OBJS=$(patsubst %,$(OBJ_DIR)/%,$(_LIB_OBJS))

$(OBJ_DIR)/%.o: %.c $(INC_FILES)
	$(CC) -c -o $@ $< $(CFLAGS) $(DBGFLAG)

exe: mkdirs $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(EXE_OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(LIB_DIR)/libxor.a: $(LIB_OBJS)
	ar -vr $@ $(LIB_OBJS)

lib: mkdirs $(LIB_DIR)/libxor.a

mkdirs:
	mkdir -p $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o
