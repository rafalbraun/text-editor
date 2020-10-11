CC=gcc
##GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
##GLIB=`pkg-config --cflags --libs glib-2.0`
PROGRAMS=view_commits
SRC=view_commits.c
UI=view_commits.ui

all: view_commits

view_commits: $(SRC) $(UI)
	gcc `pkg-config --cflags gtk+-3.0 gtksourceview-4` -o view_commits view_commits.c `pkg-config --libs gtk+-3.0 gtksourceview-4` -lgit2

clean:
	rm $(PROGRAM)
