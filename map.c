// https://stackoverflow.com/questions/1234514/very-simple-map-implemention-in-c-for-caching-purpose

// https://www.w3.org/2001/06/blindfold/api/hashmap_8c-source.html
// https://www.w3.org/2001/06/blindfold/api/hashmap_8h-source.html

#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

#define MAPSIZE 128

const char* EMPTY = "";

typedef struct pair {
	gchar* key;
	gchar* val;
} t_pair;

int map_max = 0;
t_pair* map_ptr;

gchar* k = "kkk";
gchar* v = "aaaaaaaaeefefaa";

gchar* at(t_pair* map_ptr, gchar* key) {
	if (map_max == 0) {
		return NULL;
	}
	if (strcmp(key, EMPTY)==0) {
		return NULL;
	}
	for (int i=0; i<map_max; i++) {
		if (strcmp(map_ptr[i].key, key) == 0) {
			//g_print("%s \n", map_ptr[i].val);
			return map_ptr[i].val;
		}
	}
	return NULL;
}

void insert(t_pair* map_ptr, gchar** key, gchar** val) {
	if (map_max == 0) {
		goto empty;
	}
	// check if map not full
	if (map_max == MAPSIZE) {
		fprintf(stderr, "Map already full: %d \n", MAPSIZE);
		return;
	}
	// check if key already in map
	for (int i=0; i<map_max; i++) {
		if (strcmp(map_ptr[i].key, *key)==0) {
			map_ptr->val = *val;
			return;
		}
	}
	// check if there any empty places
	for (int i=0; i<map_max; i++) {
		if (strcmp(map_ptr[i].key, "")==0) {
			map_ptr->key = *key;
			map_ptr->val = *val;
			return;
		}
	}
empty:
	// if all full and no key push at the end
	g_print("dupa\n");
	map_ptr->key = *key;
	map_ptr->val = *val;
	map_max++;
}

void delete_key(gchar* key) {
	for (int i=0; i<map_max; i++) {
		if (strcmp(map_ptr[i].key, key) == 0) {
			map_ptr[i].key = "";
			map_ptr[i].val = "";
			return;
		}
	}
}

int main() {
	map_ptr = (t_pair*)malloc(MAPSIZE * sizeof(t_pair)); 
	insert(map_ptr, &k, &v);
	gchar* found = at(map_ptr, k);

	g_print("%s -> %s \n", k, found);


}
