PREFIX        = gsf
ARCH          = $(shell uname -m)
VERSION       = 03.10
STATIC_LIB    = ../lib$(PREFIX)-$(ARCH)-$(VERSION).a
CC            = gcc
EXEC_DIR      = bin

$(EXEC_DIR)/test_next_json: test_next_json.c
	$(CC) -o $(EXEC_DIR)/test_next_json test_next_json.c $(STATIC_LIB)

all : $(EXECS)
	$(LINKER) $(LFLAGS) -o $(TGT) $(OBJS) $(CJSON_OBJ)
	$(STATIC_LINKER) rcs $(STATIC_TGT) $(OBJS) $(CJSON_OBJ)

clean:
	$(RM) $(EXEC_DIR)/*

