
$(info $(_InputFile) running under $(_System).)



PREFIX        = gsf
ARCH          = $(shell uname -m)
VERSION       = 03.10
TGT           = lib$(PREFIX)-$(ARCH)-$(VERSION).so
STATIC_TGT    = lib$(PREFIX)-$(ARCH)-$(VERSION).a
CFLAGS        = $(MACROS) -fPIC -Wall -D_STRICT_ANSI -D_LARGEFILE64_SOURCE
CPPFLAGS      = $(CFLAGS)
LFLAGS        = -shared $(LINK_FLAGS)
CC            = gcc
CPP           = gcc
LINKER        = gcc
STATIC_LINKER = ar
AR            = ar
RM            = rm -f
LIBS          = -lwsock32
CJSON_OBJ     = ../cjson/cJSON.o

.cpp.o:
	$(CPP) -c $(CPPFLAGS) $*.cpp -o$@
.c.o:
	$(CC) -c $(CFLAGS) $*.c -o$@

OBJS = gsf.o  gsf_enc.o gsf_dec.o gsf_indx.o gsf_info.o gsf_geo.o gsf_json.o gsf_compress.o 

all : $(OBJS)
	$(LINKER) $(LFLAGS) -o $(TGT) $(OBJS) $(CJSON_OBJ)
	$(STATIC_LINKER) rcs $(STATIC_TGT) $(OBJS) $(CJSON_OBJ)

clean:
	$(RM) $(TGT) $(STATIC_TGT) $(OBJS) 

