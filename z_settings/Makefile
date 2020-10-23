CC=gcc
PROGRAMS=settings
SRC=settings.c
UI=settings.ui

all: settings

settings: $(SRC) $(UI)
	gcc `pkg-config --cflags gtk+-3.0 gtksourceview-4` -o settings settings.c `pkg-config --libs gtk+-3.0 gtksourceview-4` -lgit2

clean:
	rm $(PROGRAM)
