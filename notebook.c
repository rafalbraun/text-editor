#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <stdio.h>

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

char bufferf[32];
char bufferl[32];
GList* filenames;
guint tabnum;

static void
load_file(GtkNotebook *notebook, GObject* buffer, guint pagenum) {
    const gchar* tabname;
    GtkWidget *eventbox, *child;
    GtkSourceView *textview;
    GtkScrolledWindow *scroll;
    GtkLabel  *label;
    GList* list;

    if (tabnum > 0) {
        child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), pagenum);
        eventbox = gtk_notebook_get_tab_label(GTK_NOTEBOOK(notebook), child);

        list = gtk_container_get_children(GTK_CONTAINER(eventbox));
        label = ((GtkLabel*) list->data);
        tabname = gtk_label_get_text(GTK_LABEL(label));

        gchar *text;
        gsize len;
        GError *err = NULL;

        if (g_file_get_contents(tabname, &text, &len, &err) == FALSE) {
            g_error("error reading %s: %s", tabname, err->message);
        }

        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), text, len);
        g_free(text);
    }
}

static void
close_file(GtkNotebook* notebook, gchar* filepath) {
    if (tabnum > 0) {
        for (int i=0; i < tabnum; i++) {
            GList *t = g_list_nth (filenames, i);
            if (strcmp(((gchar*) t->data), filepath) == 0) {

                GList *t = g_list_nth (filenames, i);
                filenames = g_list_remove(filenames, (t->data));
                gtk_notebook_remove_page (notebook, i);
                tabnum--;
                
                return;
            }
        }
    }
}

static gboolean
notebook_tab_clicked(GtkWidget *widget, GdkEventButton *event, gpointer notebook) {
    GList* list = gtk_container_get_children(GTK_CONTAINER(widget));
    GtkLabel* label = ((GtkLabel*) list->data);
    gchar* text = (gchar *)gtk_label_get_text(GTK_LABEL(label));

    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1)
    {
        return FALSE;
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 2)
    {
        close_file (notebook, text);
        return TRUE;
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
    {
        return TRUE;
    }
}

static void
open_file (GtkNotebook* notebook, gchar* filepath, GtkWidget* content, GObject* buffer) {
    GtkWidget* label = gtk_label_new(filepath);
    GtkWidget *event_box = gtk_event_box_new();
    int pagenum;

    if (tabnum > 0) {
        for (int i=0; i < tabnum; i++) {
            GList *t = g_list_nth (filenames, i);
            if (strcmp(((gchar*) t->data), filepath) == 0) {
                gtk_notebook_set_current_page (GTK_NOTEBOOK(notebook), i);
                return;
            }
        }
    }

    gtk_container_add(GTK_CONTAINER(event_box), label);
    if (content == NULL) {
        GtkWidget* frame = gtk_label_new("file contents");
        pagenum = gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, event_box);
    } else {
        gchar *text;
        gsize len;
        GError *err = NULL;

        if (g_file_get_contents(filepath, &text, &len, &err) == FALSE) {
            g_error("error reading %s: %s", filepath, err->message);
            return;
        }

#if NOTEBOOK
        if (!g_utf8_validate (text, len, NULL)) {
            show_error(window, "the file is binary");
            return;
        }
#endif

        pagenum = gtk_notebook_append_page (GTK_NOTEBOOK (notebook), content, event_box);
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), text, len);

        g_free(text);
    }

    tabnum++;
    filenames = g_list_append (filenames, g_strdup(filepath));

    gtk_widget_show_all (GTK_WIDGET(notebook));
    gtk_widget_show (label);

    g_signal_connect (event_box, "button-press-event", G_CALLBACK (notebook_tab_clicked), notebook);

    gtk_notebook_set_current_page (GTK_NOTEBOOK(notebook), pagenum);
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

    /* Need to refresh the widget -- 
     This forces the widget to redraw itself. */
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

void
switch_page (GtkNotebook *notebook,
             GtkWidget   *page,
             guint        page_src,
             gpointer     buffer) {
    
    gint page_dst = gtk_notebook_get_current_page(notebook);
    load_file(notebook, buffer, page_src);
}


void
page_removed (GtkNotebook *notebook,
               GtkWidget   *child,
               guint        page_num,
               gpointer     user_data) {
    g_print("page removed \n");
}

void
page_reordered (GtkNotebook *notebook,
               GtkWidget   *child,
               guint        page_num,
               gpointer     user_data) {
    g_print("page reordered\n");
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
        
        open_file (GTK_NOTEBOOK(notebook), filepath, NULL, NULL);

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
    g_signal_connect (G_OBJECT (notebook), "page-removed",
                      G_CALLBACK (page_removed),
                      NULL);
    g_signal_connect (G_OBJECT (notebook), "page-reordered",
                      G_CALLBACK (page_reordered),
                      NULL);

    // Create horizontal box for buttons
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

    open_file (GTK_NOTEBOOK(notebook), "/tmp/Makefile", NULL, NULL);
    open_file (GTK_NOTEBOOK(notebook), "/tmp/main.c", NULL, NULL);
    open_file (GTK_NOTEBOOK(notebook), "/tmp/main.h", NULL, NULL);

    // Set what page to start at (page 4) 
    gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), 3);

    /* Create a bunch of buttons */
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