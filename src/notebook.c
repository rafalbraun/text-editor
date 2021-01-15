#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

//#include "list.h"
#include "config.h"
#include "notebook.h"
#include "sourceview.h"

void list_tabs() {
    g_print("tabs: \n");
    //l_print(&head);
}

gboolean
notebook_tab_clicked(GtkWidget *widget, GdkEventButton *event, gpointer userdata) {
    gchar* title = get_text_from_eventbox(widget);

    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1) {
        // empty
        return FALSE;
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 2) {
        gint index = index_tab (head, title);
        t_tab* data = get_nth (head, index);
        close_tab (userdata, data, index);
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

    // TODO - check if exists in list!!!
    index = index_tab(head, title);

    //filepath = absolute_path[index];
    //return filepath;

    return title;
}

gchar*
get_text_from_eventbox(GtkWidget* widget)
{
    GtkLabel* label;
    GList* list;
    gchar* title;

    list = gtk_container_get_children(GTK_CONTAINER(widget));
    label = ((GtkLabel*) list->data);
    title = (gchar *)gtk_label_get_text(GTK_LABEL(label));

    return title;
}

void
close_tab (gpointer user_data, t_tab* data, int index) {
    //g_print("closing %s \n", index);
    delete_tab(&head, data); // check if delete correct - namely if page exists
    //g_print("closing index %d \n", index);
    
    gtk_notebook_remove_page (GET_NOTEBOOK(user_data), index);

    tab_max--;
}

void create_empty_tab (GtkMenuItem *menuitem, gpointer user_data) {
    gchar title[128];

    //snprintf(title, sizeof(title), "Untitled (%d)", get_untitled_files(user_data));
    //create_tab (user_data, title, "", 0);
    //incr_untitled_files (user_data);
}


// https://stackoverflow.com/questions/5802191/use-gnu-versions-of-basename-and-dirname-in-c-source
// https://people.gnome.org/~ryanl/glib-docs/glib-Miscellaneous-Utility-Functions.html
GtkSourceBuffer*
create_tab (gpointer user_data, gchar* title) {
    GtkWidget       *eventbox;
    GtkWidget       *textview;
    GtkWidget       *label;
    GtkWidget       *notebook;
    GError          *err = NULL;

    // TODO - sprawdzać zarówno relative_path jak i absolute_path
    t_tab* tab;

    gint index = index_tab (head, title);
    if (index != -1) {
        gtk_notebook_set_current_page (GET_NOTEBOOK(user_data), index);
        tab = get_nth (head, index);
        return GTK_SOURCE_BUFFER(tab->buffer);
    }

    //t_tab* tab = new_tab(title);
    //append_tab(&head, tab);
    tab = append_tab (&head, title);
    /*
    if (index != -1) {
        gtk_notebook_set_current_page (GET_NOTEBOOK(user_data), index);
        return NULL;
    }*/
    tab_max++;

    label = gtk_label_new(title);
    eventbox = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(eventbox), label);

    textview = sourceview_new(GTK_SOURCE_BUFFER(tab->buffer));

    int pagenum = gtk_notebook_append_page (GET_NOTEBOOK(user_data), textview, eventbox);
    //gtk_text_buffer_set_text(GTK_TEXT_BUFFER(tab->buffer), text, len);
    //g_free(text);

    gtk_widget_show_all (GTK_WIDGET(GET_NOTEBOOK(user_data)));
    gtk_widget_show (label);

    g_signal_connect (eventbox, "button-press-event", G_CALLBACK (notebook_tab_clicked), user_data);

    gtk_notebook_set_current_page (GET_NOTEBOOK(user_data), pagenum);

    return GTK_SOURCE_BUFFER(tab->buffer);
}

// https://developer.gnome.org/gtksourceview/stable/GtkSourceFileLoader.html
/*
void
load_file(gpointer userdata, guint pagenum) {
    gchar* filepath = get_filename_from_page_number (userdata, pagenum);

    gchar *text;
    gsize len;
    GError *err = NULL;
    if (g_file_get_contents(filepath, &text, &len, &err) == FALSE) {
        g_error("error reading %s: %s", filepath, err->message);
    }

    //gtk_text_buffer_set_text(GTK_TEXT_BUFFER(get_buffer(userdata)), text, len);
    //guess_language(GTK_SOURCE_BUFFER(get_buffer(userdata)), filepath);

    g_free(text);
}
*/
void
switch_page (GtkNotebook *notebook, GtkWidget *page, guint page_dst, gpointer user_data) {
    GList* children;
    GtkTextView* text_view;
    GtkTextBuffer* buffer;
    gint page_src;

    //page_src = gtk_notebook_get_current_page (notebook);

    //page = gtk_notebook_get_nth_page (notebook, page_src);

    children = gtk_container_get_children (GTK_CONTAINER(page));
    text_view = ((GtkTextView*) children->data);
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    SET_TEXT_BUFFER (user_data, buffer);

    /*
    if (page_src<0) {
        return;
    }
    //g_print("%d -> %d \n", page_src, page_dst);

    buffer = GET_TEXT_BUFFER(user_data);

    buffer_src = l_at(&head, page_src)->data->tab_buffer;
    buffer_dst = l_at(&head, page_dst)->data->tab_buffer;

    gtk_text_buffer_get_bounds (buffer, &start, &end);
    text = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
    strcpy (buffer_src, text);
    
    gtk_text_buffer_set_text(buffer, buffer_dst, strlen(buffer_dst));
    */

    //load_file (userdata, page_dst);
    /*
    if (absolute_path[pagedst]) {
        load_file(userdata, pagedst);
    } else {
        g_print("[INFO] trying to open %s on page %d but is null \n", absolute_path[pagedst], pagedst);
    }*/
}
