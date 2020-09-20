#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

// gcc -o notebook notebook.c `pkg-config --cflags --libs gtk+-3.0`

#include <stdio.h>
#include <gtk/gtk.h>

//
// https://developer.gnome.org/gtk-tutorial/stable/x1450.html
//

int i;
char bufferf[32];
char bufferl[32];

void
append_book(GtkNotebook *notebook, int i) {
    GtkWidget *frame;
    GtkWidget *label;

    sprintf(bufferf, "Append Frame %d", i + 1);
    sprintf(bufferl, "Page %d", i + 1);
    
    frame = gtk_frame_new (bufferf);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
    //gtk_widget_set_size_request (frame, 100, 75);
    gtk_widget_show (frame);
    
    label = gtk_label_new (bufferf);
    gtk_container_add (GTK_CONTAINER (frame), label);
    gtk_widget_show (label);
    
    label = gtk_label_new (bufferl);
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
}

void
prepend_book(GtkNotebook* notebook, int i) {
    GtkWidget *frame;
    GtkWidget *label;

    sprintf (bufferf, "Prepend Frame %d", i + 1);
    sprintf (bufferl, "PPage %d", i + 1);
    
    frame = gtk_frame_new (bufferf);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
    //gtk_widget_set_size_request (frame, 100, 75);
    gtk_widget_show (frame);
    
    label = gtk_label_new (bufferf);
    gtk_container_add (GTK_CONTAINER (frame), label);
    gtk_widget_show (label);
    
    label = gtk_label_new (bufferl);
    gtk_notebook_prepend_page (GTK_NOTEBOOK (notebook), frame, label);
}

static void append_page( GtkWidget   *button,
                         GtkNotebook *notebook )
{
    append_book(GTK_NOTEBOOK(notebook), i++);
}

static void prepend_page( GtkWidget   *button,
                         GtkNotebook *notebook )
{
    prepend_book(GTK_NOTEBOOK(notebook), i++);
}

/* Remove a page from the notebook */
static void remove_book( GtkWidget   *button,
                         GtkNotebook *notebook )
{
    gint page;
    
    page = gtk_notebook_get_current_page (notebook);
    gtk_notebook_remove_page (notebook, page);
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

gboolean
key_pressed(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) 
{
  if (event->keyval == GDK_KEY_q && (event->state & GDK_CONTROL_MASK)) {
    printf("key pressed: %s\n", "ctrl + Q");
    remove_book(NULL, GTK_NOTEBOOK(notebook));
  }
  if (event->keyval == GDK_KEY_n && (event->state == GDK_CONTROL_MASK)) {
    printf("key pressed: %s\n", "ctrl + N");
    append_book(GTK_NOTEBOOK(notebook), i++);
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


int main( int argc,
          char *argv[] )
{
    GtkWidget *window;
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

    // Create horizontal box for buttons
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

    // Let's append a bunch of pages to the notebook 
    for (i = 0; i < 5; i++) {
        append_book(GTK_NOTEBOOK(notebook), i);
    }
      
    // Now let's add a page to a specific spot 
    checkbutton = gtk_check_button_new_with_label ("Check me please!");
    gtk_widget_set_size_request (checkbutton, 100, 75);
    gtk_widget_show (checkbutton);
   
    label = gtk_label_new ("Add page");
    gtk_notebook_insert_page (GTK_NOTEBOOK (notebook), checkbutton, label, 2);
    
    // Now finally let's prepend pages to the notebook 
    for (i = 0; i < 5; i++) {
        prepend_book(GTK_NOTEBOOK(notebook), i);
    }
    
    // Set what page to start at (page 4) 
    gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), 3);

    /* Create a bunch of buttons */
    button = gtk_button_new_with_label ("close");
    g_signal_connect_swapped (button, "clicked",
			      G_CALLBACK (delete), NULL);
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

    button = gtk_button_new_with_label ("append page");
    g_signal_connect (button, "clicked",
                      G_CALLBACK (append_page),
                      notebook);
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

    button = gtk_button_new_with_label ("prepend page");
    g_signal_connect (button, "clicked",
                      G_CALLBACK (prepend_page),
                      notebook);
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

    gtk_widget_show_all (window);
    
    gtk_main ();
    
    return 0;
}
