CC=gcc
LIBS=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
PROGRAMS=app treeview
SRC=app.c treeview.c

all: $(PROGRAMS)

%: %.c
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm $(PROGRAMS)
