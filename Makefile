##
## https://stackoverflow.com/questions/9052792/how-to-pass-macro-definition-from-make-command-line-arguments-d-to-c-source
## https://stackoverflow.com/questions/2198186/purpose-of-ifndef-filename-endif-in-header-file
##

CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`
PROGRAMS=app treeview notebook2 notebook1 filechooser search_path
SRC=app.c treeview.c notebook2.c notebook1.c filechooser.c search_path.c

all: $(PROGRAMS) glib_regex

app: app.c treeview.c notebook2.c
	gcc app.c $(GTK) -o app

%: %.c
	$(CC) -o $@ $^ $(GTK)

glib_regex: glib_regex.c
	$(CC) $(GLIB) glib_regex.c -o glib_regex

clean:
	rm $(PROGRAMS) glib_regex
