#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <stdio.h>

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

/*
#if !NOTEBOOK
#include "config.h"
#endif
*/

GList* filenames;
t_node *head;
t_pair *map_ptr;

//guint tabnum;

static void
close_file(gpointer userdata, gchar* filepath) {
    guint tabmax = get_tabnum(userdata);

    /////////////////////////////////////////////////////////////////////////////////
    //delete_at();
    l_delete_value(&head, filepath);
    /////////////////////////////////////////////////////////////////////////////////

    if (tabmax > 0) {
        for (int i=0; i < tabmax; i++) {
            GList *t = g_list_nth (filenames, i);
            if (strcmp(((gchar*) t->data), filepath) == 0) {

                GList *t = g_list_nth (filenames, i);
                filenames = g_list_remove(filenames, (t->data));
                gtk_notebook_remove_page (get_notebook(userdata), i);
                tabnum_decr(userdata);
                
                return;
            }
        }
    }
}

static gboolean
notebook_tab_clicked(GtkWidget *widget, GdkEventButton *event, gpointer userdata) {
    gchar* title = get_text_from_eventbox(widget);

    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1)
    {
        // empty
        l_print(&head);
        return FALSE;
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 2)
    {
        //g_print("tabnum : %d \n", get_tabnum(userdata));
        close_file (userdata, title);
        return TRUE;
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
    {
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
    GList* list;
    gchar *text;
    gsize len;
    GError *err = NULL;
    gchar* tabname;

    guint tabnum = get_tabnum(userdata);

    if (tabnum > 0) {
        child = gtk_notebook_get_nth_page(get_notebook(userdata), pagenum);
        eventbox = gtk_notebook_get_tab_label(get_notebook(userdata), child);
        tabname = get_text_from_eventbox(eventbox);

        if (g_file_get_contents(tabname, &text, &len, &err) == FALSE) {
            g_error("error reading %s: %s", tabname, err->message);
        }

        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(get_buffer(userdata)), text, len);
        g_free(text);
    }
}

static void
save_file(gchar* path, gchar* contents) {

}

static void
open_file (gpointer userdata, gchar* filepath, GtkWidget* content) {
    GtkWidget* label = gtk_label_new(filepath);
    GtkWidget *eventbox = gtk_event_box_new();
    GtkNotebook* notebook = get_notebook(userdata);
    GtkTextBuffer* buffer = GTK_TEXT_BUFFER(get_buffer(userdata));
    int pagenum;

    guint tabnum = get_tabnum(userdata);

/////////////////////////////////////////////////////////////////////////////////
    int index = l_append(&head, g_strdup(filepath));
    //m_insert(map_ptr, g_strdup(filepath), g_strdup(filename));
    g_print("index %d \n", index);
/////////////////////////////////////////////////////////////////////////////////

    if (tabnum > 0) {
        for (int i=0; i < tabnum; i++) {
            GList *t = g_list_nth (filenames, i);
            if (strcmp(((gchar*) t->data), filepath) == 0) {
                gtk_notebook_set_current_page (notebook, i);
                return;
            }
        }
    }

    gtk_container_add(GTK_CONTAINER(eventbox), label);
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

    pagenum = gtk_notebook_append_page (notebook, content, eventbox);
    gtk_text_buffer_set_text(buffer, text, len);
    g_free(text);
    
    tabnum_incr(userdata);
    filenames = g_list_append (filenames, g_strdup(filepath));

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
    
    gint page_dst = gtk_notebook_get_current_page(get_notebook(userdata));
    load_file(userdata, page_src);
}






/*

    if (content == NULL) {
        GtkWidget* frame = gtk_label_new("file contents");
        pagenum = gtk_notebook_append_page (notebook, frame, event_box);
    } 


void
append_book(GtkNotebook *notebook, gchar* tabname) {
    GtkWidget *frame;
    GtkWidget *label;
    GtkWidget *cLabel;
    
    frame = gtk_frame_new (tabname);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
    gtk_widget_show (frame);
    
    cLabel = gtk_label_new ("contents");
    gtk_container_add (GTK_CONTAINER (frame), cLabel);

    label = gtk_label_new (tabname);
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
}

static void remove_book( GtkWidget   *button,
                         GtkNotebook *notebook )
{
    GtkWidget *child, *label;
    gint pagenum;
    const gchar* tabname;
    
    pagenum = gtk_notebook_get_current_page (notebook);
    child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), pagenum);
    label = gtk_notebook_get_tab_label(GTK_NOTEBOOK(notebook), child);
    tabname = gtk_label_get_text(GTK_LABEL(label));

    if (tabnum > 0) {
        for (int i=0; i < tabnum; i++) {
            GList *t = g_list_nth (filenames, i);
            if (strcmp(((gchar*) t->data), tabname) == 0) {
                filenames = g_list_remove(filenames, (t->data));
                break;
            }
        }
    }
    tabnum--;

    gtk_notebook_remove_page (notebook, pagenum);

    // Need to refresh the widget -- 
    // This forces the widget to redraw itself.
    gtk_widget_queue_draw (GTK_WIDGET (notebook));
}

static gboolean delete( GtkWidget *widget,
                        GtkWidget *event,
                        gpointer   data )
{
    gtk_main_quit ();
    return FALSE;
}

static gboolean print_tabs( GtkWidget *widget,
                        GtkWidget *event,
                        gpointer   data )
{
    if (tabnum > 0) {
        for (int i=0; i < tabnum; i++) {
            GList *t = g_list_nth (filenames, i);
            g_print("%d :: %s\n", i, ((gchar*) t->data));
        }
        g_print("-----\n");
    }
}

static gboolean
key_pressed(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) 
{
  if (event->keyval == GDK_KEY_q && (event->state & GDK_CONTROL_MASK)) {
    remove_book(NULL, GTK_NOTEBOOK(notebook));
  }
  if (event->keyval == GDK_KEY_n && (event->state == GDK_CONTROL_MASK)) {
    append_book(GTK_NOTEBOOK(notebook), "foo");
  }

  return FALSE;
}


#if !NOTEBOOK

GtkWidget* window;

static void
open_file_dialog (GtkWidget *widget, GtkWidget* notebook) {
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Open File",
                                        GTK_WINDOW(window),
                                        action,
                                        "_Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "_Open",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        gchar *filepath;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        GtkWidget *label1, *label2;

        filepath = gtk_file_chooser_get_filename (chooser);
        g_print("GTK_RESPONSE_ACCEPT: %s \n", filepath);
        
        //open_file (GTK_NOTEBOOK(notebook), filepath, NULL, NULL);

    } else if (res == GTK_RESPONSE_CANCEL) {
      g_print("GTK_RESPONSE_CANCEL \n");
    }
  gtk_widget_destroy (dialog);
}

int main( int argc,
          char *argv[] )
{
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *vbox, *hbox;
    GtkWidget *notebook;
    GtkWidget *checkbutton;
    
    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    g_signal_connect (window, "delete-event",
                  G_CALLBACK (delete), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    // Create vertical box for notebook at top and buttons at bottom
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);

    // Create a new notebook, place the position of the tabs 
    notebook = gtk_notebook_new ();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
    gtk_box_pack_start (GTK_BOX (vbox), notebook, TRUE, TRUE, 0);

    g_signal_connect (G_OBJECT (notebook), "key-press-event",
                      G_CALLBACK (key_pressed),
                      NULL);
    g_signal_connect (G_OBJECT (notebook), "switch-page",
                      G_CALLBACK (switch_page),
                      NULL);

    // Create horizontal box for buttons
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

    //open_file (GTK_NOTEBOOK(notebook), "./notebook.c", NULL, NULL);

    // Set what page to start at (page 4) 
    gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), 3);

    // Create a bunch of buttons
    button = gtk_button_new_with_label ("close");
    g_signal_connect_swapped (button, "clicked",
			      G_CALLBACK (delete), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

    button = gtk_button_new_with_label ("print tabs");
    g_signal_connect_swapped (button, "clicked",
                  G_CALLBACK (print_tabs), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

    button = gtk_button_new_with_label ("next page");
    g_signal_connect_swapped (button, "clicked",
			      G_CALLBACK (gtk_notebook_next_page),
			      notebook);
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
    
    button = gtk_button_new_with_label ("prev page");
    g_signal_connect_swapped (button, "clicked",
			      G_CALLBACK (gtk_notebook_prev_page),
			      notebook);
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

    button = gtk_button_new_with_label ("remove page");
    g_signal_connect (button, "clicked",
                      G_CALLBACK (remove_book),
                      notebook);
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

    button = gtk_button_new_with_label ("open file");
    g_signal_connect (button, "clicked",
                      G_CALLBACK (open_file_dialog),
                      notebook);
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

    gtk_widget_show_all (window);
    
    gtk_main ();
    
    return 0;
}

#endif

*/