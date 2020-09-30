CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0`
GLIB=`pkg-config --cflags --libs glib-2.0`
PROGRAMS=settings
SRC=settings.c

all: $(PROGRAMS)

%: %.c
	$(CC) -o $@ $^ $(GTK)

clean:
	rm $(PROGRAMS)
