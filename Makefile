CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`
##PROGRAMS=text_editor treeview sourceview
PROGRAMS=text_editor treeview

all: $(PROGRAMS)

text_editor: text_editor.c treeview.c notebook.c callback.c sourceview.c list.c 
	$(CC) $(GLIB) text_editor.c $(GTK) -o text_editor

treeview: test_treeview.c
	$(CC) $(GLIB) test_treeview.c $(GTK) -o treeview


#%: %.c
#	$(CC) $(GLIB) -o $@ $^ $(GTK)

clean:
	rm $(PROGRAMS) 
