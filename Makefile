##
## https://stackoverflow.com/questions/9052792/how-to-pass-macro-definition-from-make-command-line-arguments-d-to-c-source
## https://stackoverflow.com/questions/2198186/purpose-of-ifndef-filename-endif-in-header-file
##

CC=gcc
LIBS=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
PROGRAMS=app treeview notebook2 notebook1 filechooser
SRC=app.c treeview.c notebook2.c notebook1.c filechooser.c

all: $(PROGRAMS)

%: %.c
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm $(PROGRAMS)
