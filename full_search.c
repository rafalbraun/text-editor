#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 50000
#define SEARCH_PATH 1

#include "glib_regex.c"

int counter = 0;

// needed by spawn
int stdout_fd = -1;
char buffer [BUFFER_SIZE];
guint gLinenum;
guint gCount;

guint gLimit = 3;

gchar* 
extract_filename(gchar* filepath) {
  gchar*  filename;
  gchar** tokens;
  int     tokennum;

  tokens = g_strsplit(filepath, "/", -1);
  tokennum = g_strv_length(tokens);
  filename = g_strdup(tokens[tokennum-1]);
  g_strfreev(tokens);

  return filename;
}

// utf8 problem
// https://mail.gnome.org/archives/gtk-list/2007-February/msg00142.html
gchar* 
read_file(gchar* filename) {
  gchar *contents;
  gsize len;
  GError *err = NULL;

  if (g_file_get_contents(filename, &contents, &len, &err) == FALSE) {
    g_error("error reading %s: %s", filename, err->message);
  }
  //bytes = g_bytes_new_take(contents, len);

  return contents;
}

void add_to_list(GObject* widget, 
    gchar *str0, 
    gchar *str1, 
    gchar* str2, 
    gchar* str3, 
    gchar* str4,
    gchar* str5) 
{
  GtkTreeView *treeview = NULL;
  GtkTreeModel *model = NULL;
  GtkListStore *liststore = NULL;
  GtkTreeIter iter;

  treeview = GTK_TREE_VIEW(widget);
  model = gtk_tree_view_get_model(treeview);
  liststore = GTK_LIST_STORE(model);

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter, 0, str0, 1, str1, 2, str2, 3, str3, 4, str4, -1, str5, -1);
}

void clear_list(GObject* widget) {
  GtkTreeView *treeview = NULL;
  GtkTreeModel *model = NULL;
  GtkListStore *liststore = NULL;

  treeview = GTK_TREE_VIEW(widget);
  model = gtk_tree_view_get_model(treeview);
  liststore = GTK_LIST_STORE(model);

  gtk_list_store_clear(liststore);
}

gboolean 
_stop_main_loop (gpointer treeview)
{
  char** lines;
  char** line;
  char*  contents;
  int    bytes;
  gchar* filename;
  gchar* occurrence;
  gchar* linenum;
  gchar* start;
  gchar* end;
  gchar* filepath;

  //g_print("time %d \n", counter);
  //counter++;

  bytes = read (stdout_fd, buffer, BUFFER_SIZE);
  buffer[bytes-1] = '\0';

  if (bytes > 0) {
    lines = g_strsplit(buffer, "\n", -1);
    for (int i=0 ; i<g_strv_length(lines); i++) {
      line = g_strsplit(lines[i], "\x1C", 0);
      //add_to_list(treeview, line[4], extract_filename(line[0]), line[1]);
      filepath = line[0];
      linenum = line[1];
      start = line[2];
      end = line[3];
      occurrence = line[4];
      filename = extract_filename(filepath);
      add_to_list(treeview, occurrence, filename, linenum, start, end, filepath);
      g_strfreev(line);
    }
    g_strfreev(lines);
  }
  memset(buffer, 0, sizeof(buffer));

  return TRUE;
}

int spawn(gchar* needle) {
  char *argv [15];
  GPid child_pid;
  int status;

  g_print("searching %s\n", needle);

  memset (argv, 0, 15 * sizeof (char *));
  argv[0] = 
    "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/glib_regex";
  argv[1] = 
    "/home/rafal/IdeaProjects/gtksourceview-my-ide/application";
  argv[2] = needle; //"int";
  status = g_spawn_async_with_pipes (NULL, argv, NULL, 
      G_SPAWN_SEARCH_PATH, NULL, NULL, &child_pid, NULL, &stdout_fd, NULL, NULL);
  if (!status) {
    g_print("[FAILED] 1 Failed to run %s: %d \n", argv [0], status);
    return 1;
  }
  if (child_pid == 0) {
    g_print("[FAILED] 2 child pid not returned \n");
    return 1;
  }
  if (stdout_fd == -1) {
    g_print("[FAILED] 3 out fd is -1 \n");
    return 1;
  }

  return 0;
}

void
preedit_changed (GtkEntry *widget, gpointer treeview)  {
  //g_print("preedit_changed %s \n", gtk_entry_get_text(GTK_ENTRY(widget)));
  //add_to_list(userdata, "string000000000", "string11111111111");
  clear_list(treeview);

  gchar* needle = (gchar*)gtk_entry_get_text(GTK_ENTRY(widget));

  // check if string is more than gLimit
  if (strlen(needle) > gLimit) {
    spawn(needle);
  }
}

int count_lines(gchar* contents, int len) {
  int count = 0;
  for (int i=0; i<len; i++) {
    if (contents[i]==10) { // here checking for newline, could be also '\n'
      count++;
    }
  }
  return count;
}

void on_changed(GtkTreeSelection *widget, gpointer textbufferscroll) {
    GObject *textbuffer, *textview;
    GtkTextIter start_iter, end_iter;
    GtkTreeIter iter;
    GtkTextTagTable *tagtable;
    GtkTextTag *btag, *wtag;
    GtkTreeModel *model;
    gchar *filename, *contents, *linenum, *start, *end;
    gsize len;
    GError *err = NULL;
    GList *children, *child;
    gsize bytes_read, bytes_written;
    gchar* str;

    children = gtk_container_get_children(GTK_CONTAINER(textbufferscroll));
    child = g_list_nth (children, 0);
    textview = ((GObject*) child->data);
    textbuffer = ((GObject*) gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)));

    if (gtk_tree_selection_get_selected(
        GTK_TREE_SELECTION(widget), &model, &iter)) {

        gtk_tree_model_get(model, &iter, 1, &filename,  -1);
        gtk_tree_model_get(model, &iter, 2, &linenum,  -1);
        gtk_tree_model_get(model, &iter, 3, &start,  -1);
        gtk_tree_model_get(model, &iter, 4, &end,  -1); //filename
        g_print("filename %s, linenum %s, start %s, end %s \n", filename, linenum, start, end);

        if (g_file_get_contents(filename, &contents, &len, &err) == FALSE) {
          g_error("error reading %s: %s", filename, err->message);
        }

        // convert to utf8
        /*
        // g_utf8_validate
        str = g_convert (contents,
                   len,
                   "UTF-8",
                   "ANSI_X3.4-1968",
                   &bytes_read,
                   &bytes_written,
                   &err);
        if (str == NULL) {
          g_error("error converting %s: %s", filename, err->message);
        }
        */

        //g_print("bytes_read: %d \n", bytes_read);
        //g_print("bytes_written: %d \n", bytes_written);

        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), contents, len);
        gCount = count_lines(contents, len);
        gLinenum = atoi(linenum);

        tagtable = gtk_text_buffer_get_tag_table(GTK_TEXT_BUFFER(textbuffer));
        btag = gtk_text_tag_table_lookup(tagtable, "black_bg");
        wtag = gtk_text_tag_table_lookup(tagtable, "white_fg");
        gtk_text_buffer_get_iter_at_line_index(GTK_TEXT_BUFFER(textbuffer), 
            &start_iter, gLinenum, atoi(start));
        gtk_text_buffer_get_iter_at_line_index(GTK_TEXT_BUFFER(textbuffer), 
            &end_iter, gLinenum, atoi(end));
        gtk_text_buffer_apply_tag(GTK_TEXT_BUFFER(textbuffer), btag, 
            &start_iter, &end_iter);
        gtk_text_buffer_apply_tag(GTK_TEXT_BUFFER(textbuffer), wtag, 
            &start_iter, &end_iter);

        g_free(filename);
  }
}

// needed: linenum, count
void
adjustment_changed (GtkAdjustment *adjustment,
                    gpointer userdata) {
    gdouble upper, pagesize, factor;

    upper = gtk_adjustment_get_upper(GTK_ADJUSTMENT(adjustment));
    pagesize = gtk_adjustment_get_page_size (GTK_ADJUSTMENT(adjustment));
    factor = ( ((gdouble)gLinenum) * upper) / (gdouble)gCount - pagesize/2;

    gtk_adjustment_set_value (GTK_ADJUSTMENT(adjustment), factor);
}

void
row_activated (GtkTreeView       *tree_view,
               GtkTreePath       *path,
               GtkTreeViewColumn *column,
               gpointer           user_data) {
  g_print("row activated \n");
}
/*
gboolean
key_pressed_window(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) 
{
  g_print("key_pressed: GtkWindow \n");
  return FALSE;
}
*/

gchar* escape_ampersands(gchar* str) {
    gchar** strstr;
    gchar* joined;
    const gchar* separator = "&amp;";

    strstr = g_strsplit(str, "&", 0);
    joined = g_strjoinv(separator, strstr);
    return joined;
}

void
cell_data_func (GtkTreeViewColumn *col,
                    GtkCellRenderer   *renderer,
                    GtkTreeModel      *model,
                    GtkTreeIter       *iter,
                    gpointer           user_data)
{

    //gchar *markuptxt;
    gchar *buff;
    //gchar *escaped;

    gtk_tree_model_get(model, iter, 0, &buff, -1);

    //g_print("[%s] -> ", buff);
    //escaped = escape_ampersands(buff);
    //g_print("[%s] \n", buff);

    //markuptxt = g_markup_printf_escaped( g_strconcat("<tt>", buff, "</tt>", NULL) );

    //g_object_set( renderer, "markup", markuptxt, NULL );

    g_object_set( renderer, "text", buff, NULL );

    //g_free(markuptxt);
    g_free(buff);
    //g_free(escaped);
}


void combo_selected(GtkWidget *widget, gpointer user_data) {
    GtkTreeModel* model;
    GtkTreeIter iter;
    GValue value = {0,};
    gchar *lang = NULL;

    if (gtk_combo_box_get_active_iter (GTK_COMBO_BOX (widget), &iter)) {
        model = gtk_combo_box_get_model (GTK_COMBO_BOX(widget));
        gtk_tree_model_get_value (model, &iter, 0, &value);
        lang = g_value_dup_string (&value);
        g_print("%s \n", lang);
        g_free (lang);
        g_value_unset (&value);
    }
}

int
main (int   argc, char *argv[])
{
  GtkBuilder *builder;
  GObject *window, *entry, *treeview, *textbufferscroll, *buffer, *combobox;
  GtkTreeSelection *selection;
  GError *error = NULL;
  GObject* renderer;
  GObject* column;
  GObject *adjustment;

  const gchar *charset;
  g_get_charset (&charset);
  g_print("%s \n", charset);


  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "full_search.ui", &error) == 0)
  {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
  }

  // Connect signal handlers to the constructed widgets. 
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  //g_signal_connect (window, "key-press-event", G_CALLBACK (key_pressed_window), NULL);

  treeview = gtk_builder_get_object(builder, "treeview1");
  g_signal_connect(treeview, "row-activated", G_CALLBACK(row_activated), NULL);

  textbufferscroll = gtk_builder_get_object (builder, "textbufferscroll");
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  g_signal_connect(selection, "changed", G_CALLBACK(on_changed), textbufferscroll);

  adjustment = (GObject*)gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(textbufferscroll));
  g_signal_connect(adjustment, "changed", G_CALLBACK(adjustment_changed), NULL);

  entry = gtk_builder_get_object (builder, "entry");
  g_signal_connect (entry, "changed", G_CALLBACK (preedit_changed), treeview);

  renderer = gtk_builder_get_object (builder, "cellrenderertext1");
  column   = gtk_builder_get_object (builder, "treeviewcolumn1");
  gtk_tree_view_column_set_cell_data_func(GTK_TREE_VIEW_COLUMN(column), GTK_CELL_RENDERER(renderer), cell_data_func, NULL, NULL);

  buffer = gtk_builder_get_object (builder, "textbuffer");
  gtk_text_buffer_create_tag(GTK_TEXT_BUFFER(buffer), "black_bg", "background", "black", NULL); 
  gtk_text_buffer_create_tag(GTK_TEXT_BUFFER(buffer), "white_fg", "foreground", "white", NULL); 

  //gtk_list_store_append(liststore, &iter);
  //gtk_list_store_set(liststore, &iter, 0, str0, 1, str1, 2, str2, 3, str3, 4, str4, -1, str5, -1);



  combobox = gtk_builder_get_object (builder, "pp2_pin2_in");
  //g_signal_connect (combobox, "changed", G_CALLBACK (combo_box_changed), NULL);
  g_signal_connect(G_OBJECT(combobox), "changed", G_CALLBACK(combo_selected), NULL);

  g_timeout_add (100,
           (GSourceFunc) _stop_main_loop,
           treeview);

  gtk_main ();

  return 0;
}
