#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "../src/list.h"
#include "../src/config.h"
#include "../src/notebook.h"

#define UI_DIR "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/ui/notebook.ui"

int main(int argc, char * argv[]) {

    GtkBuilder *builder;
    GObject *window;
    GError *error = NULL;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, UI_DIR, &error) == 0) {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }
    
    window = gtk_builder_get_object (builder, "window");
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_main();

    return 0;
}

