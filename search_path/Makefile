CC=gcc
PROGRAMS=search_path treestore
CFLAGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0  gtksourceview-4`

all: search_path treestore

search_path: search_path.c search_path.ui
	$(CC) $(CFLAGS) -o search_path search_path.c $(LIBS)

treestore: treestore.c treestore.ui
	$(CC) $(CFLAGS) -o treestore treestore.c $(LIBS)

clean:
	rm search_path treestore


