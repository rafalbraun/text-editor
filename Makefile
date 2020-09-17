CC=gcc
LIBS=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
PROGRAMS=app
SRC=app.c

all: $(PROGRAMS)

%: %.c
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm $(PROGRAMS)
