CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`
##PROGRAMS=text_editor treeview sourceview
PROGRAMS=text_editor treeview sourceview glib_regex full_search find_files find_files_gui ## mouse

all: $(PROGRAMS)

text_editor: text_editor.c treeview.c notebook.c callback.c sourceview.c list.c 
	$(CC) $(GLIB) text_editor.c $(GTK) -o text_editor

treeview: test_treeview.c
	$(CC) $(GLIB) test_treeview.c $(GTK) -o treeview

sourceview: test_sourceview.c
	$(CC) $(GLIB) test_sourceview.c $(GTK) -o sourceview

full_search: full_search.c glib_regex.c
	$(CC) $(GLIB) full_search.c $(GTK) -o full_search

glib_regex: glib_regex.c
	$(CC) $(GLIB) glib_regex.c -o glib_regex -lpthread

mouse: mouse.c
	$(CC) $(GLIB) mouse.c $(GTK) -o mouse

find_files: find_files.c
	$(CC) $(GLIB) find_files.c $(GTK) -o find_files

find_files_gui: find_files_gui.c
	$(CC) $(GLIB) find_files_gui.c $(GTK) -o find_files_gui

#%: %.c
#	$(CC) $(GLIB) -o $@ $^ $(GTK)

clean:
	rm $(PROGRAMS) 
