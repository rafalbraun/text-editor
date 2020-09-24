#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

// gcc -o notebook notebook.c `pkg-config --cflags --libs gtk+-3.0`

#include <stdio.h>
#include <gtk/gtk.h>

//
// https://developer.gnome.org/gtk-tutorial/stable/x1450.html
//

char bufferf[32];
char bufferl[32];
GList* filenames;
guint tabnum;
GtkWidget* window;

//static void
//print_hello (GtkWidget *widget,
//             gpointer   data)
static gboolean print_hello(GtkWidget *widget, GdkEventButton *event, gpointer userdata)
{
  GList* list = gtk_container_get_children(GTK_CONTAINER(widget));
  GtkLabel* label = ((GtkLabel*) list->data);
  const gchar* text = gtk_label_get_text(GTK_LABEL(label));
  g_print("label: %s \n", text);

    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1)
    {//1 is left mouse btn
        g_print("1\n");
        return FALSE;
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 2)
    {//2 is left mouse btn
        g_print("2\n");
        return TRUE;
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
    {   //3 is right mouse btn
        g_print("3\n");
        return TRUE;
    }
}

static void
open_file (GtkNotebook* notebook, gchar* filepath) {
    GtkWidget* label = gtk_label_new(filepath);
    GtkWidget* frame = gtk_label_new("file contents");
    GtkWidget *event_box = gtk_event_box_new();
    int pagenum;

    // check if this filepath is already in list
    if (tabnum > 0) {
        for (int i=0; i < tabnum; i++) {
            GList *t = g_list_nth (filenames, i);
            if (strcmp(((gchar*) t->data), filepath) == 0) {
                g_print("[INFO] %s already found: %s \n", filepath, ((gchar*) t->data) );
                gtk_notebook_set_current_page (GTK_NOTEBOOK(notebook), i);
                return;
            }
        }
    }
    tabnum++;
    filenames = g_list_append (filenames, g_strdup(filepath));

    gtk_container_add(GTK_CONTAINER(event_box), label);
    pagenum = gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, event_box);
    gtk_widget_show_all (GTK_WIDGET(notebook));
    gtk_widget_show (label);

    g_signal_connect (event_box, "button-press-event", G_CALLBACK (print_hello), NULL);

    gtk_notebook_set_current_page (GTK_NOTEBOOK(notebook), pagenum);
}

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
        
        open_file (GTK_NOTEBOOK(notebook), filepath);

    } else if (res == GTK_RESPONSE_CANCEL) {
      g_print("GTK_RESPONSE_CANCEL \n");
    }
  gtk_widget_destroy (dialog);
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

/* Remove a page from the notebook */
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

gboolean
key_pressed(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) 
{
  if (event->keyval == GDK_KEY_q && (event->state & GDK_CONTROL_MASK)) {
    printf("key pressed: %s\n", "ctrl + Q");
    remove_book(NULL, GTK_NOTEBOOK(notebook));
  }
  if (event->keyval == GDK_KEY_n && (event->state == GDK_CONTROL_MASK)) {
    printf("key pressed: %s\n", "ctrl + N");
    append_book(GTK_NOTEBOOK(notebook), "aaaaa");
  }

  return FALSE;
}

void
switch_page (GtkNotebook *notebook,
             GtkWidget   *page,
             guint        page_src,
             gpointer     user_data) {
    
    gint page_dst = gtk_notebook_get_current_page(notebook);
    g_print("page: %d -> %d\n", page_dst, page_src);
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

int main( int argc,
          char *argv[] )
{
    GtkWidget *button;
    GtkWidget *label;
    //GtkWidget *table;
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

    // Let's append a bunch of pages to the notebook 
    //for (int i = 0; i < 5; i++) {
    //    append_book(GTK_NOTEBOOK(notebook), "bbb");
    //}
    open_file (GTK_NOTEBOOK(notebook), "/tmp/Makefile");
    open_file (GTK_NOTEBOOK(notebook), "/tmp/main.c");
    open_file (GTK_NOTEBOOK(notebook), "/tmp/main.h");

      
    // Now let's add a page to a specific spot 
    // checkbutton = gtk_check_button_new_with_label ("Check me please!");
    // gtk_widget_set_size_request (checkbutton, 100, 75);
    // gtk_widget_show (checkbutton);
   
    // label = gtk_label_new ("Add page");
    // gtk_notebook_insert_page (GTK_NOTEBOOK (notebook), checkbutton, label, 2);


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

    // button = gtk_button_new_with_label ("append page");
    // g_signal_connect (button, "clicked",
    //                   G_CALLBACK (append_page),
    //                   notebook);
    // gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

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