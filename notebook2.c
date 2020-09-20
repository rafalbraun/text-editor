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


static void
open_file (GtkNotebook* notebook, gchar* filepath) {
    GtkWidget* label = gtk_label_new(filepath);
    GtkWidget* content = gtk_label_new("file contents");
    int pagenum;

    // check if this filepath is already in list
    if (tabnum > 0) {
        for (int i=0; i < tabnum; i++) {
            GList *t = g_list_nth (filenames, i);
            if (strcmp(((gchar*) t->data), filepath) == 0) {
                g_print("[INFO] %s already found\n", filepath);
                gtk_notebook_set_current_page (GTK_NOTEBOOK(notebook), i);
                return;
            }
        }
    }
    tabnum++;
    filenames = g_list_append (filenames, g_strdup(filepath));

    pagenum = gtk_notebook_append_page (GTK_NOTEBOOK (notebook), content, label);
    //gtk_widget_show (label);
    //gtk_widget_show (content);
    gtk_widget_show_all (GTK_WIDGET(notebook));

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
        //open_file (filename);
        g_print("GTK_RESPONSE_ACCEPT: %s \n", filepath);

        //label1 = gtk_label_new(filepath);
        //label2 = gtk_label_new(filepath);
        
        open_file (GTK_NOTEBOOK(notebook), filepath);
        //gtk_notebook_append_page (GTK_NOTEBOOK (notebook), label1, label2);
        //g_free (filename);

    //gtk_widget_show (label1);
    //gtk_widget_show (label2);

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

    //sprintf(bufferf, "aaaaaaaaa");
    //sprintf(bufferl, "bbbbb");
    
    frame = gtk_frame_new (tabname);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
    gtk_widget_show (frame);
    
    cLabel = gtk_label_new ("contents");
    gtk_container_add (GTK_CONTAINER (frame), cLabel);

    label = gtk_label_new (tabname);
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
}

static void append_page( GtkWidget   *button,
                         GtkNotebook *notebook )
{

  append_book(GTK_NOTEBOOK(notebook), "ccccccc");
}

/* Remove a page from the notebook */
static void remove_book( GtkWidget   *button,
                         GtkNotebook *notebook )
{
    gint pagenum;
    
    pagenum = gtk_notebook_get_current_page (notebook);
    gtk_notebook_remove_page (notebook, pagenum);

    /* Need to refresh the widget -- 
     This forces the widget to redraw itself. */
    gtk_widget_queue_draw (GTK_WIDGET (notebook));

    // remove element from list
    //GList *t = g_list_nth (data->filenames, pagenum);
    //data->filenames = g_list_remove (data->filenames, ((gchar*) t->data));
    //data->tabnum--;
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

    button = gtk_button_new_with_label ("open file");
    g_signal_connect (button, "clicked",
                      G_CALLBACK (open_file_dialog),
                      notebook);
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

    gtk_widget_show_all (window);
    
    gtk_main ();
    
    return 0;
}
