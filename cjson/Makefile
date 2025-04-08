LFLAGS        = -shared $(LINK_FLAGS)
CC            = gcc
CPP           = gcc
LINKER        = gcc
RM            = rm -f

OBJS = cJSON.o

cJSON.o: cJSON.c
	$(CC) -c $(CFLAGS) $*.c -o$@

clean:
	$(RM) $(OBJS)
