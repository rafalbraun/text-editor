CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`
PROGRAMS=text_editor treeview notebook full_search sourceview

all: $(PROGRAMS) glib_regex

text_editor: text_editor.c treeview.c notebook.c callback.c sourceview.c
	$(CC) text_editor.c $(GTK) -o text_editor

%: %.c
	$(CC) -o $@ $^ $(GTK)

glib_regex: glib_regex.c
	$(CC) $(GLIB) glib_regex.c -o glib_regex -lpthread

clean:
	rm $(PROGRAMS) glib_regex
