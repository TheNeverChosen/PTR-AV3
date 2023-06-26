PRJ=$(shell basename $(CURDIR))

# folder for libs .so and .a:
SRC_DIR=$(CURDIR)/src
LIB_DIR=$(CURDIR)/lib
OBJ_DIR=$(CURDIR)/obj
INC_DIR=$(CURDIR)/inc

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c) # wildcard expansion in var set
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES)) # $(patsubst pattern,replacement,$(var))

# Compiler and flags
STD=c11
CC := gcc
CFLAGS := -Wall -W -lm -pthread -std=$(STD) -D DEBUG 
CCFLAGS := -Wall -W -lm -pthread -std=$(STD) -D DEBUG -I $(INC_DIR) -L $(LIB_DIR)

# Procedures
all: dirs clean $(PRJ)

$(PRJ): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY : clean dirs

dirs:
	mkdir -p $(OBJ_DIR) $(LIB_DIR) $(INC_DIR) $(SRC_DIR)

clean:
	-@rm -f $(PRJ)  $(OBJ_DIR)/*.o *~