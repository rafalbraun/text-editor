#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <stdio.h>

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

#include <glib/gstdio.h>

//t_node *head;
//t_pair *map_ptr;

const gchar* separator = "\n";
gchar* relative_path[128];
gchar* absolute_path[128];

gchar* open_files() {
    int max=0;
    while (absolute_path[max]) {max++;}
    absolute_path[max+1] = NULL;
    return g_strjoinv(separator, (gchar**)&absolute_path);
}

int index_of(gchar* basename) {
    int i=0;
    while (absolute_path[i]) {
        if (strcmp(relative_path[i], basename) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

static void
close_file(gpointer userdata, gchar* title) {
    t_node** head = get_list(userdata);
    gchar* filepath;
    int index;

    index = index_of(title);
    filepath = absolute_path[index];
    absolute_path[index] = "";
    relative_path[index] = "";

    index = l_delete_value(head, filepath);
    gtk_notebook_remove_page (get_notebook(userdata), index);
}

static gboolean
notebook_tab_clicked(GtkWidget *widget, GdkEventButton *event, gpointer userdata) {
    gchar* title = get_text_from_eventbox(widget);

    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1) {
        // empty
        return FALSE;
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 2) {
        close_file (userdata, title);
        return TRUE;
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) {
        // context menu
        return TRUE;
    }
}

static void
load_file(gpointer userdata, guint pagenum) {
    GtkWidget *eventbox, *child;
    GtkSourceView *textview;
    GtkScrolledWindow *scroll;
    GtkLabel  *label;
    gchar *title, *filepath;
    int index;

    child = gtk_notebook_get_nth_page(get_notebook(userdata), pagenum);
    eventbox = gtk_notebook_get_tab_label(get_notebook(userdata), child);
    title = get_text_from_eventbox(eventbox);
    index = index_of(title);
    filepath = absolute_path[index];

    gchar *text;
    gsize len;
    GError *err = NULL;
    if (g_file_get_contents(filepath, &text, &len, &err) == FALSE) {
        g_error("error reading %s: %s", title, err->message);
    }

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(get_buffer(userdata)), text, len);
    g_free(text);
}

static void
save_file(gchar* path, gchar* contents) {

}

// https://stackoverflow.com/questions/5802191/use-gnu-versions-of-basename-and-dirname-in-c-source
// https://people.gnome.org/~ryanl/glib-docs/glib-Miscellaneous-Utility-Functions.html
static void
open_file (gpointer userdata, gchar* filepath) {
    GtkWidget       *eventbox;
    GtkWidget       *textview;
    GtkWidget       *label;
    gchar           *title;
    GtkNotebook* notebook = get_notebook(userdata);

    t_node** head = get_list(userdata);
    int index = l_append(head, filepath);
    if (index != -1) {
        gtk_notebook_set_current_page (notebook, index);
        g_print("[INFO] switching to file %s under index %d \n", filepath, index);
        return;
    }

    gchar *text;
    gsize len;
    GError *err = NULL;
    if (g_file_get_contents(filepath, &text, &len, &err) == FALSE) {
        g_error("error reading %s: %s", filepath, err->message);
        return;
    }
    if (!g_utf8_validate (text, len, NULL)) {
        show_error(get_window(userdata), "the file is binary");
        return;
    }

    index = l_index_of(head, filepath);
    gchar* basename = g_path_get_basename(filepath);

    title = (index_of(basename) == -1) ? basename : filepath;
    relative_path[index] = title;
    absolute_path[index] = filepath;

    label = gtk_label_new(title);
    eventbox = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(eventbox), label);

    textview = sourceview_new(GTK_SOURCE_BUFFER(get_buffer(userdata)));
    guess_language(GTK_SOURCE_BUFFER(get_buffer(userdata)), filepath);

    int pagenum = gtk_notebook_append_page (notebook, textview, eventbox);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(get_buffer(userdata)), text, len);
    g_free(text);

    g_print("[INFO] opening file %s under index %d \n", filepath, index);

    gtk_widget_show_all (GTK_WIDGET(notebook));
    gtk_widget_show (label);

    g_signal_connect (eventbox, "button-press-event", G_CALLBACK (notebook_tab_clicked), userdata);

    gtk_notebook_set_current_page (notebook, pagenum);
}

void
switch_page (GtkNotebook *notebook,
             GtkWidget   *page,
             guint        page_src,
             gpointer     userdata) {
    
    gtk_notebook_get_current_page(get_notebook(userdata));
    load_file(userdata, page_src);
}
