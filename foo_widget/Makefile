##
## https://stackoverflow.com/questions/9052792/how-to-pass-macro-definition-from-make-command-line-arguments-d-to-c-source
## https://stackoverflow.com/questions/2198186/purpose-of-ifndef-filename-endif-in-header-file
##

CC=gcc
LIBS=`pkg-config --cflags --libs gtk+-3.0`
##PROGRAMS=foo_widget main
##SRC=foo_widget.c main.c
PROGRAMS=main
SRC=main.c

all: $(PROGRAMS)

%: %.c
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm $(PROGRAMS)
