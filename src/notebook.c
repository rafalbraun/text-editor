#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "list.h"
#include "config.h"
#include "notebook.h"
#include "sourceview.h"

int get_max() {
    int max = 0;
    while (absolute_path[max]) {max++;}
    return max;
}

void list_tabs() {
    int i=0;
    while (absolute_path[i]) {
        g_print("%d :: %s -> %s \n", i, relative_path[i], absolute_path[i]);
        i++;
    }
    g_print("[INFO] max: %d \n", get_max());
}

int get_index(gchar* basename) {
    for (int i=0; absolute_path[i]; i++) {
        if (strcmp(relative_path[i], basename) == 0) {
            return i;
        }
    }
    return -1;
}

gchar* open_files() {
    int max = get_max();
    absolute_path[max+1] = NULL;
    return g_strjoinv(separator, (gchar**)&absolute_path);
}

void
close_file(gpointer userdata, gchar* title) {
    //t_node** head = get_list(userdata);
    gchar* filepath;
    int index;

    index = get_index(title);
    filepath = absolute_path[index];
    //absolute_path[index] = "";
    //relative_path[index] = "";

    index = l_delete_value(&head, filepath);
    gtk_notebook_remove_page (get_notebook(userdata), index);

    // przepisać na niższe indexy relative_path i absolute_path
    int max = get_max();
    for (int i=index; i<max; i++) {
        absolute_path[i] = absolute_path[i+1];
        relative_path[i] = relative_path[i+1];
    }
    absolute_path[max] = NULL;
    relative_path[max] = NULL;
}

gboolean
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

gchar* 
get_filename_from_page_number (gpointer userdata, int pagenum) {
    GtkWidget *eventbox, *child;
    gchar *title, *filepath;
    int index;

    child = gtk_notebook_get_nth_page(get_notebook(userdata), pagenum);
    eventbox = gtk_notebook_get_tab_label(get_notebook(userdata), child);
    title = get_text_from_eventbox(eventbox);
    index = get_index(title);

    filepath = absolute_path[index];

    return filepath;
}

// https://developer.gnome.org/gtksourceview/stable/GtkSourceFileLoader.html
void
load_file(gpointer userdata, guint pagenum) {
    gchar* filepath = get_filename_from_page_number (userdata, pagenum);

    gchar *text;
    gsize len;
    GError *err = NULL;
    if (g_file_get_contents(filepath, &text, &len, &err) == FALSE) {
        g_error("error reading %s: %s", filepath, err->message);
    }

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(get_buffer(userdata)), text, len);
    //guess_language(GTK_SOURCE_BUFFER(get_buffer(userdata)), filepath);

    g_free(text);
}


void
save_file (const gchar* filename, const gchar* contents) {
    int status, mode;
    gssize length;
    GError *err = NULL;

    mode = 0666;
    length = strlen(contents);
    GFileSetContentsFlags flags = G_FILE_SET_CONTENTS_CONSISTENT;

    status = g_file_set_contents_full (filename,
                                       contents,
                                       length,
                                       flags,
                                       mode,
                                       &err);

    if (status == FALSE) {
        g_error("[ERROR] Error saving %s: %s", filename, err->message);
    }
}

void save_file_default (gpointer userdata) {
    gint             page_num;
    gchar           *filepath;
    gchar           *contents;
    GList           *children;
    GtkWidget       *scroll;
    GtkTextView     *text_view;
    GtkTextBuffer   *buffer;
    GtkTextIter      start, end;

    page_num = gtk_notebook_get_current_page (get_notebook (userdata));
    scroll = gtk_notebook_get_nth_page (get_notebook (userdata), page_num);
    children = gtk_container_get_children (GTK_CONTAINER(scroll));
    text_view = (GtkTextView*) g_list_first (children)->data;
    buffer = gtk_text_view_get_buffer (text_view);
    gtk_text_buffer_get_bounds (buffer, &start, &end);

    contents = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
    filepath = get_filename_from_page_number (userdata, page_num);
    
    save_file (filepath, contents);
}

// https://stackoverflow.com/questions/5802191/use-gnu-versions-of-basename-and-dirname-in-c-source
// https://people.gnome.org/~ryanl/glib-docs/glib-Miscellaneous-Utility-Functions.html
void
open_file (gpointer userdata, gchar* filepath) {
    GtkWidget       *eventbox;
    GtkWidget       *textview;
    GtkWidget       *label;
    gchar           *title;
    GtkNotebook     *notebook = get_notebook(userdata);

    //g_print("[INFO] opening file %s \n", filepath);

    // sprawdzanie czy nie jest binarny musi być tutaj ponieważ jeśli byłoby po l_append to wrzucany byłby w listę pod indexem NULL (bo nie zapisywalibyśmy nazwy pliku w liście - wywalałoby się)
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

    //t_node** head = get_list(userdata);
    int index = l_append(&head, filepath);
    if (index != -1) {
        gtk_notebook_set_current_page (notebook, index);
        //g_print("[INFO] switching to file %s under index %d \n", filepath, index);
        return;
    }

    index = l_index_of(&head, filepath);
    gchar* basename = g_path_get_basename(filepath);

    title = (get_index(basename) == -1) ? basename : filepath;
    relative_path[index] = title;
    absolute_path[index] = filepath;

    label = gtk_label_new(title);
    eventbox = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(eventbox), label);

    textview = sourceview_new(GTK_SOURCE_BUFFER(get_buffer(userdata)));
    //guess_language(GTK_SOURCE_BUFFER(get_buffer(userdata)), filepath);

    int pagenum = gtk_notebook_append_page (notebook, textview, eventbox);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(get_buffer(userdata)), text, len);
    g_free(text);

    //g_print("[INFO] opening file %s under index %d \n", filepath, index);

    gtk_widget_show_all (GTK_WIDGET(notebook));
    gtk_widget_show (label);

    g_signal_connect (eventbox, "button-press-event", G_CALLBACK (notebook_tab_clicked), userdata);

    gtk_notebook_set_current_page (notebook, pagenum);
}

void
switch_page (GtkNotebook *notebook, GtkWidget *page, guint pagedst, gpointer userdata) {
    gint page_src;

    page_src = gtk_notebook_get_current_page (get_notebook(userdata));
    load_file (userdata, pagedst);
    /*
    if (absolute_path[pagedst]) {
        load_file(userdata, pagedst);
    } else {
        g_print("[INFO] trying to open %s on page %d but is null \n", absolute_path[pagedst], pagedst);
    }*/
}