INCLUDES = ../include
OBJECTS	=vsprintf.o string.o  printf.o
MAKEFILE=Makefile


#lib.o : $(MAKEFILE) $(OBJECTS)
all: lib.a
	$(LD) -r -o lib.o $(OBJECTS)
	sync

lib.a : $(MAKEFILE) $(OBJECTS)
	$(AR) rcs lib.a $(OBJECTS)
	sync


vsprintf.o : vsprintf.c
	$(CC) $(CFLAGS) -I$(INCLUDES) -c vsprintf.c

string.o : string.c
	$(CC) $(CFLAGS) -I$(INCLUDES) -c string.c

printf.o : printf.c
	$(CC) $(CFLAGS) -lc -I$(INCLUDES) -c printf.c

clean:
	rm -f *.o
	rm -f *.a


