CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`
##PROGRAMS=text_editor treeview sourceview
##PROGRAMS=text_editor treeview sourceview glib_regex full_search find_files find_files_gui ## mouse
PROGRAMS=text_editor glib_regex full_search find_files find_files_gui

all: $(PROGRAMS)

text_editor: text_editor.o treeview.o notebook.o callback.o sourceview.o list.o config.o
	$(CC) $(GLIB) main.c text_editor.o treeview.o notebook.o callback.o sourceview.o list.o config.o $(GTK) -o text_editor

config.o: config.h config.c
	$(CC) -c $(GLIB) config.c $(GTK) -o config.o

list.o: list.h list.c
	$(CC) -c $(GLIB) list.c $(GTK) -o list.o

treeview.o: treeview.h treeview.c config.h
	$(CC) -c $(GLIB) treeview.c $(GTK) -o treeview.o

notebook.o: notebook.h notebook.c config.h sourceview.h
	$(CC) -c $(GLIB) notebook.c $(GTK) -o notebook.o

sourceview.o: sourceview.h sourceview.c 
	$(CC) -c $(GLIB) sourceview.c $(GTK) -o sourceview.o

callback.o: callback.h callback.c 
	$(CC) -c $(GLIB) callback.c $(GTK) -o callback.o

text_editor.o: text_editor.h text_editor.c 
	$(CC) -c $(GLIB) text_editor.c $(GTK) -o text_editor.o




#list.o: list.c
#	$(CC) -c list.c -o list.o

#treeview: test_treeview.c
#	$(CC) $(GLIB) test_treeview.c $(GTK) -o treeview

#sourceview: test_sourceview.c
#	$(CC) $(GLIB) test_sourceview.c $(GTK) -o sourceview

full_search: full_search.c glib_regex.c
	$(CC) $(GLIB) full_search.c $(GTK) -o full_search

glib_regex: glib_regex.c
	$(CC) $(GLIB) glib_regex.c -o glib_regex -lpthread

#mouse: mouse.c
#	$(CC) $(GLIB) mouse.c $(GTK) -o mouse

find_files: find_files.c
	$(CC) $(GLIB) find_files.c $(GTK) -o find_files

find_files_gui: find_files_gui.c
	$(CC) $(GLIB) find_files_gui.c $(GTK) -o find_files_gui

#%: %.c
#	$(CC) $(GLIB) -o $@ $^ $(GTK)

clean:
	rm $(PROGRAMS) *.o
