#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "../src/list.h"
#include "../src/config.h"
#include "../src/notebook.h"
#include "../src/sourceview.h"

#define UI_DIR "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/ui/notebook.ui"

int main(int argc, char * argv[]) {

    GtkBuilder *builder;
    //GObject *window;
    GError *error = NULL;
    UserData* user_data;

    gtk_init(&argc, &argv);

	user_data = g_new0 (UserData, 1);
	//user_data->head = NULL;
	head = NULL;
	tab_max = 0;

    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, UI_DIR, &error) == 0) {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }
    
    user_data->window = gtk_builder_get_object (builder, "window");
    user_data->notebook = gtk_builder_get_object (builder, "notebook");	
    user_data->buffer = gtk_builder_get_object (builder, "sourcebuffer");

    g_signal_connect (G_OBJECT (user_data->window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

	create_tab (user_data, "foo.c", "foo", 3);
	create_tab (user_data, "bar.c", "bar", 3);

    gtk_main();

    return 0;
}

