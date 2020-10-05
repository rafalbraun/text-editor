// https://stackoverflow.com/questions/1234514/very-simple-map-implemention-in-c-for-caching-purpose

// https://www.w3.org/2001/06/blindfold/api/hashmap_8c-source.html
// https://www.w3.org/2001/06/blindfold/api/hashmap_8h-source.html

// https://web.archive.org/web/20091118050932/http://www.cl.cam.ac.uk/~cwc22/hashtable

#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

#define MAPSIZE 128

char* const EMPTY = "";

static int map_max = 0;
typedef struct pair {
	gchar* key;
	gchar* val;
} t_pair;

gchar* m_at(t_pair* map_ptr, gchar* key) {
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

void m_insert(t_pair* map_ptr, gchar** key, gchar** val) {
	if (map_max == 0) {
		goto empty;
	}
	// check if map not full
	if (map_max == MAPSIZE) {
		fprintf(stderr, "Map already full: %d \n", MAPSIZE);
		exit(1);
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
		if (strcmp(map_ptr[i].key, EMPTY)==0) {
			map_ptr->key = *key;
			map_ptr->val = *val;
			return;
		}
	}
empty:
	// if all full and no key push at the end
	map_ptr[map_max].key = *key;
	map_ptr[map_max].val = *val;
	map_max++;
}

void m_delete_key(t_pair* map_ptr, gchar* key) {
	for (int i=0; i<map_max; i++) {
		if (strcmp(map_ptr[i].key, key) == 0) {
			map_ptr[i].key = EMPTY;
			map_ptr[i].val = EMPTY;
			return;
		}
	}
}

#if !MAP

gchar* k1 = "/home/user/Documents";
gchar* v1 = "Documents";
gchar* k2 = "/home/user/Downloads";
gchar* v2 = "Downloads";
gchar* k3 = "/home/user/Videos";
gchar* v3 = "Videos";
gchar* k4 = "/home/user/Music";
gchar* v4 = "Music";
gchar* k5 = "/home/user/Pictures";
gchar* v5 = "Pictures";

int main() {
	t_pair* map_ptr;
	
	map_ptr = (t_pair*)malloc(MAPSIZE * sizeof(t_pair));
	m_insert(map_ptr, &k1, &v1);
	m_insert(map_ptr, &k2, &v2);
	m_insert(map_ptr, &k3, &v3);
	m_insert(map_ptr, &k4, &v4);

	g_print("%s -> %s \n", k4, m_at(map_ptr, k4));

	m_delete_key(map_ptr, k1);
	m_delete_key(map_ptr, k3);
	g_print("%s -> %s (deleted) \n", k1, m_at(map_ptr, k1));

	g_print("map_max: %d \n", map_max);

	m_insert(map_ptr, &k4, &v4);
	g_print("map_max: %d \n", map_max);

	m_insert(map_ptr, &k5, &v5);
	g_print("map_max: %d \n", map_max);

	g_print("%s -> %s \n", k5, m_at(map_ptr, k5));

}

#endif
