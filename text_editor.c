#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "list.h"
#include "config.h"
#include "sourceview.h"
#include "treeview.h"
#include "notebook.h"
#include "callback.h"
#include "text_editor.h"

// on quit save clipboard to xclip: https://wiki.ubuntu.com/ClipboardPersistence
void
on_main_quit (GtkWidget * widget, gpointer userdata)
{
    GError *err = NULL;
    gchar *contents = open_files ();

    g_file_set_contents (cast_to_ud(userdata)->session_info, contents, strlen (contents), &err);
    g_free (contents);

    gtk_main_quit ();
}

void
new_file_cb (GtkButton * widget, gpointer userdata)
{
    gchar buffer[64];
    gchar *fname;
    g_print ("%d \n", cast_to_ud (userdata)->untitled_files_in_buffer_max);
    sprintf (buffer, "/tmp/Untitled %d",
       cast_to_ud (userdata)->untitled_files_in_buffer_max++);
    fname = g_strdup (buffer);
    g_creat (fname, S_IREAD | S_IWRITE);
    open_file (userdata, fname);
}
/*
gboolean
_check_timeout_since_last_keypress (gpointer userdata)
{
  gchar *fname;
  GDateTime *time_val;
  gchar *iso8601_string;

  gdouble time_elapsed = g_timer_elapsed (timer, NULL);

  //g_print ("[INFO] Timer: %lf , is active %d \n", 
  //    time_elapsed, 
  //    g_timer_is_active (timer) );

  if (time_elapsed > 3.0 && is_saved == FALSE) {

    time_val = g_date_time_new_now_local ();
    iso8601_string = g_date_time_format_iso8601 (time_val);

    //g_print ("saving file ...\n");
    is_saved = TRUE;

    fname = g_strdup_printf ("./tmp/filename_%s", iso8601_string);
    g_creat (fname, S_IREAD | S_IWRITE);

    g_print ("\e[34m[INFO] File created.\e[0m \n");
  }


        //co 400 ms sprawdzaj timer i jeśli ma nabite 5 sekund (przy każdym keypress/przełączeniu karty jest reset timera) to zapisujemy plik
        //
        //przy przełączeniu karty powinniśmy sprawdzać czy plik różni się od tego co mamy w ostatniej wersji i jeśli tak to pomijamy a jeśli zmieniło się to zapisujemy


  return TRUE;
}
*/

void
open_files_from_last_session (gpointer userdata)
{
  gchar *text, *filepath = "~session-info";
  gchar **filepaths;
  gsize len;
  GError *err = NULL;
  guint i = 0;

  if (g_file_get_contents (filepath, &text, &len, &err) == FALSE) {
    g_error ("error reading %s: %s", filepath, err->message);
    return;
  }
  filepaths = g_strsplit (text, "\n", 0);

  while (filepaths[i]) {
    if (strcmp (filepaths[i], "") != 0) {
      open_file (userdata, filepaths[i]);
    }
    i++;
  }
}

