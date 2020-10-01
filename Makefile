CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`
PROGRAMS=app treeview notebook full_search

all: $(PROGRAMS) glib_regex

app: app.c treeview.c notebook.c
	$(CC) app.c $(GTK) -o app

%: %.c
	$(CC) -o $@ $^ $(GTK)

glib_regex: glib_regex.c
	$(CC) $(GLIB) glib_regex.c -o glib_regex -lpthread

clean:
	rm $(PROGRAMS) glib_regex
