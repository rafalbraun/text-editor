CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`
PROGRAMS=find_files_gui find_files

all: $(PROGRAMS)

%: %.c
	$(CC) $(GLIB) -o $@ $^ $(GTK)

clean:
	rm $(PROGRAMS) 
