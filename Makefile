CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`
##PROGRAMS=text_editor treeview sourceview
##PROGRAMS=text_editor treeview sourceview glib_regex full_search find_files find_files_gui ## mouse
##PROGRAMS=text_editor glib_regex full_search find_files find_files_gui
SRC=./src

##all: test_notebook
#all: text_editor 
#all: test_treeview 
all: test_config

#test_sourceview
##all: $(PROGRAMS)
##all: test_stack
##all: test_list
##test_list test_sourceview 
##test: test_sourceview
##test: stack.o

text_editor: text_editor.o treeview.o notebook.o callback.o sourceview.o config.o
	$(CC) $(GLIB) src/main.c obj/text_editor.o obj/treeview.o obj/notebook.o obj/callback.o obj/sourceview.o obj/config.o $(GTK) -o bin/text_editor

config.o: src/config.h src/config.c
	$(CC) -c $(GLIB) src/config.c $(GTK) -o obj/config.o

#list.o: src/list.h src/list.c
#	$(CC) -c $(GLIB) src/list.c $(GTK) -o obj/list.o

treeview.o: src/treeview.h src/treeview.c src/config.h
	$(CC) -c $(GLIB) src/treeview.c $(GTK) -o obj/treeview.o

notebook.o: src/notebook.h src/notebook.c src/config.h src/sourceview.h
	$(CC) -c $(GLIB) src/notebook.c $(GTK) -o obj/notebook.o

sourceview.o: src/sourceview.h src/sourceview.c 
	$(CC) -c $(GLIB) src/sourceview.c $(GTK) -o obj/sourceview.o

callback.o: src/callback.h src/callback.c 
	$(CC) -c $(GLIB) src/callback.c $(GTK) -o obj/callback.o

text_editor.o: src/text_editor.h src/text_editor.c 
	$(CC) -c $(GLIB) src/text_editor.c $(GTK) -o obj/text_editor.o


### TEST

###################################################################
#test_list.o: test/test_list.c src/list.h
#	$(CC) -c $(GLIB) test/test_list.c $(GTK) -o obj/test_list.o

test_config.o: test/test_config.c src/config.h
	$(CC) -c $(GLIB) test/test_config.c $(GTK) -o obj/test_config.o

test_config: test_config.o config.o
	$(CC) $(GLIB) test/test_config.c obj/config.o $(GTK) -o bin/test_config

###################################################################
test_stack.o: test/test_stack.c src/stack.h
	$(CC) -c $(GLIB) test/test_stack.c $(GTK) -o obj/test_stack.o

test_stack: test_stack.o stack.o
	$(CC) $(GLIB) test/test_stack.c obj/stack.o $(GTK) -o bin/test_stack

###################################################################
test_sourceview.o: test/test_sourceview.c src/sourceview.h src/config.h
	$(CC) -c $(GLIB) test/test_sourceview.c $(GTK) -o obj/test_sourceview.o

test_sourceview: test_sourceview.o sourceview.o config.o
	$(CC) $(GLIB) test/test_sourceview.c obj/sourceview.o obj/config.o $(GTK) -o bin/test_sourceview

###################################################################
test_treeview.o: test/test_treeview.c src/treeview.h
	$(CC) -c $(GLIB) test/test_treeview.c $(GTK) -o obj/test_treeview.o

test_treeview: test_treeview.o treeview.o config.o
	$(CC) $(GLIB) test/test_treeview.c obj/treeview.o obj/config.o $(GTK) -o bin/test_treeview

###################################################################
test_notebook.o: test/test_notebook.c src/notebook.h 
	$(CC) -c $(GLIB) test/test_notebook.c $(GTK) -o obj/test_notebook.o

test_notebook: test_notebook.o notebook.o list.o config.o sourceview.o
	$(CC) $(GLIB) test/test_notebook.c obj/notebook.o obj/list.o obj/config.o obj/sourceview.o $(GTK) -o bin/test_notebook

###################################################################
history:
	$(CC) `pkg-config --cflags gtk+-3.0 gtksourceview-4` -o bin/history modules/history.c `pkg-config --libs gtk+-3.0 gtksourceview-4`

settings:
	$(CC) `pkg-config --cflags gtk+-3.0 gtksourceview-4` -o bin/settings modules/settings.c `pkg-config --libs gtk+-3.0 gtksourceview-4`






#full_search: full_search.c glib_regex.c
#	$(CC) $(GLIB) full_search.c $(GTK) -o full_search

#glib_regex: glib_regex.c
#	$(CC) $(GLIB) glib_regex.c -o glib_regex -lpthread

#####

#find_files: find_files.c
#	$(CC) $(GLIB) find_files.c $(GTK) -o find_files

#find_files_gui: find_files_gui.c
#	$(CC) $(GLIB) find_files_gui.c $(GTK) -o find_files_gui

#%: %.c
#	$(CC) $(GLIB) -o $@ $^ $(GTK)

clean:
	rm $(PROGRAMS) obj/*.o bin/*

