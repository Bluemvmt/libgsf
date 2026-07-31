PREFIX  = gsf
ARCH    = $(shell uname -m)
VERSION = 03.11
DIST    = dist
TGT     = $(DIST)/lib$(PREFIX)-$(ARCH)-$(VERSION).so

CC      = gcc
RM      = rm -f
CFLAGS  = -fPIC -Wall -O2 -D_STRICT_ANSI -D_LARGEFILE64_SOURCE \
          -I cjson -I GSF_03-11
LDFLAGS = -shared -lm

SRC_DIR = GSF_03-11
SRCS    = \
	$(SRC_DIR)/gsf.c \
	$(SRC_DIR)/gsf_enc.c \
	$(SRC_DIR)/gsf_dec.c \
	$(SRC_DIR)/gsf_indx.c \
	$(SRC_DIR)/gsf_info.c \
	$(SRC_DIR)/gsf_geo.c \
	$(SRC_DIR)/gsf_json.c \
	$(SRC_DIR)/gsf_compress.c \
	cjson/cJSON.c

OBJS    = $(SRCS:.c=.o)

.PHONY: all clean smoke

all: $(TGT)

$(DIST):
	mkdir -p $(DIST)

$(TGT): $(OBJS) | $(DIST)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

smoke: $(TGT)
	./scripts/smoke-check.sh $(TGT)

clean:
	$(RM) $(OBJS) $(TGT)
	rmdir $(DIST) 2>/dev/null || true
