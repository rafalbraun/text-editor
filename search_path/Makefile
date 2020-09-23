CC=gcc
PROGRAMS=search_path

all: search_path

search_path: search_path.c search_path.ui
	$(CC) `pkg-config --cflags gtk+-3.0` -o search_path search_path.c `pkg-config --libs gtk+-3.0`

clean:
	rm search_path
