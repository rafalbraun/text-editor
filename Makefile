CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`
PROGRAMS=text_editor treeview sourceview listview find_files

all: $(PROGRAMS) glib_regex full_search

text_editor: text_editor.c treeview.c notebook.c callback.c sourceview.c list.c 
	$(CC) $(GLIB) text_editor.c $(GTK) -o text_editor

%: %.c
	$(CC) $(GLIB) -o $@ $^ $(GTK)

full_search: full_search.c glib_regex.c
	$(CC) $(GLIB) full_search.c $(GTK) -o full_search

glib_regex: glib_regex.c
	$(CC) $(GLIB) glib_regex.c -o glib_regex -lpthread

clean:
	rm $(PROGRAMS) glib_regex
