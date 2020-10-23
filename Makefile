CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`
##PROGRAMS=text_editor treeview sourceview find_files_gui find_files
PROGRAMS=text_editor

all: $(PROGRAMS)

text_editor: text_editor.c treeview.c notebook.c callback.c sourceview.c list.c 
	$(CC) $(GLIB) text_editor.c $(GTK) -o text_editor

%: %.c
	$(CC) $(GLIB) -o $@ $^ $(GTK)

clean:
	rm $(PROGRAMS) 
