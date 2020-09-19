
void 
append_page( gchar* filename, gchar* filepath, struct DispatchData *data) {

  GtkWidget   *scroll, *label;
  gint        pagenum;

  // check if this filepath is already in list
  if (data->filenames_size > 0) {
    for (int i=0; i<data->filenames_size; i++) {
      GList *t = g_list_nth (data->filenames_list, i);
      if (strcmp(((gchar*) t->data), filepath) == 0) {
        gtk_notebook_set_current_page ( GTK_NOTEBOOK( data->notebook ), i );
        return;
      }
    }
  }

  data->filenames_size++;
  data->filenames_list = g_list_append (data->filenames_list, g_strdup(filepath));

  scroll  = create_gtksourceview ( GTK_SOURCE_BUFFER( data->buffer ) );
  label   = gtk_label_new ( filename );
  pagenum = gtk_notebook_append_page ( GTK_NOTEBOOK( data->notebook ), scroll, label );

  //clear_buffer( data->buffer );
  //load_file ( data->buffer, filepath );
  //guess_language ( filepath, data );

  gtk_widget_show_all ( data->notebook );

  // MUST be called AFTER page made visible !!!
  gtk_notebook_set_current_page ( GTK_NOTEBOOK( data->notebook ), pagenum );

}

void
switch_page_cb (GtkNotebook        *notebook,
            gpointer            arg1,
            guint               pagesrc,
            struct DispatchData *data) 
{
  
  //gint pagedst = gtk_notebook_get_current_page(notebook);

  //GtkWidget* page = gtk_notebook_get_nth_page (notebook, pagesrc);
  //GtkWidget* label = gtk_notebook_get_tab_label (notebook, page);
  //const gchar* tabname = gtk_label_get_text (GTK_LABEL(label));

  GList *t = g_list_nth (data->filenames_list, pagesrc);
  gchar* filepath = ((gchar*) t->data);
  //g_print("[INFO](%d -> %d) :: %d / %d :: %s \n", pagedst, pagesrc, pagesrc, data->filenames_size, filepath);

  clear_buffer( data->buffer );
  load_file ( data->buffer, filepath );
  guess_language ( filepath, data );

}

GtkWidget* 
create_notebook(struct DispatchData *data) 
{
  GtkWidget* notebook = gtk_notebook_new ();
  
  g_signal_connect (G_OBJECT (notebook), "switch-page",
                    G_CALLBACK (switch_page_cb),
                    data);

  return notebook;
}

