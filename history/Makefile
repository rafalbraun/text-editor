CC=gcc
PROGRAMS=history
SRC=history.c
UI=history.ui

all: history

history: $(SRC) $(UI)
	gcc `pkg-config --cflags gtk+-3.0 gtksourceview-4` -o history history.c `pkg-config --libs gtk+-3.0 gtksourceview-4`

clean:
	rm $(PROGRAM)
