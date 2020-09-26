CC=gcc
PROGRAMS=search_path treestore

all: search_path treestore

search_path: search_path.c search_path.ui
	$(CC) `pkg-config --cflags gtk+-3.0` -o search_path search_path.c `pkg-config --libs gtk+-3.0` -lpthread

treestore: treestore.c treestore.ui
	$(CC) `pkg-config --cflags gtk+-3.0` -o treestore treestore.c `pkg-config --libs gtk+-3.0`


clean:
	rm search_path
