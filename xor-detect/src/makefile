NAME=xordetect
OBJ_DIR=obj
INC_DIR=../../include/.
APP_INC_DIR=$(INC_DIR)/xor-detect/.
BIN_DIR=../bin/.
LIB_DIR=../../lib/.

DEBUG=0
CC=clang
CFLAGS=-I$(INC_DIR) -I$(APP_INC_DIR) -I$(INC_DIR)/xor-crack/.

ifeq ($(DEBUG),0)
	DBGFLAG=
else
	DBGFLAG=-g
endif

_XOR_CRACK_INC_FILES=score.h xor-crack.h xor-crack-types.h
_INC_FILES=xor-detect.h
INC_FILES=$(APP_INC_DIR)/xor-detect.h $(INC_DIR)/hex.h $(patsubst %,$(INC_DIR)/xor-crack/%,$(_XOR_CRACK_INC_FILES))

_EXE_OBJS=xor-detect.o main.o
EXE_OBJS=$(patsubst %,$(OBJ_DIR)/%,$(_EXE_OBJS))

_LIB_OBJS=xor-detect.o
LIB_OBJS=$(patsubst %,$(OBJ_DIR)/%,$(_LIB_OBJS))

EXT_LIB=$(LIB_DIR)/libxor.a

$(OBJ_DIR)/%.o: %.c $(INC_FILES)
	$(CC) -c -o $@ $< $(CFLAGS) $(DBGFLAG)

exe: $(BIN_DIR)/$(NAME) copyfile

$(BIN_DIR)/$(NAME): $(EXE_OBJS)
	$(CC) -o $@ $^ $(EXT_LIB) $(CFLAGS)

lib: $(EXT_LIB)

$(EXT_LIB): $(LIB_OBJS)
	ar -vr $@ $<

copyfile:
	cp xor-encrypted-hex.txt $(BIN_DIR)

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o
