#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

static void
guess_language(GtkSourceBuffer* buffer, gchar* filepath) {
    GtkSourceLanguageManager *manager;
    GtkSourceLanguage *lang = NULL;
    gboolean result_uncertain;
    gchar *content_type;

    manager = gtk_source_language_manager_get_default ();
    content_type = g_content_type_guess (filepath, NULL, 0, &result_uncertain);
    if (result_uncertain) {
        //g_print("no lang recognized \n");
        gtk_source_buffer_set_highlight_syntax (buffer, FALSE);
        g_free (content_type);
        return;
    }

    lang = gtk_source_language_manager_guess_language (manager, filepath, content_type);
    if (lang != NULL) {
        g_print("lang %s recognized in file %s \n", gtk_source_language_get_name(lang), filepath);
        gtk_source_buffer_set_language (buffer, lang);
        gtk_source_buffer_set_highlight_syntax (buffer, TRUE);
        g_free (content_type);
    }
}

static void show_langs() {
    GtkSourceLanguageManager *manager;
    const gchar * const * lang_ids;

    manager = gtk_source_language_manager_get_default ();
    lang_ids = gtk_source_language_manager_get_language_ids (manager);

    for (int i=0; *(lang_ids+i); i++) {
        g_print("%d -> %s \n", i, *(lang_ids+i));
    }

}

void clear_buffer(GtkSourceBuffer* buffer) {
    GtkTextIter iter_start, iter_end;

    gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (buffer), &iter_start);
    gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (buffer), &iter_end);
    gtk_text_buffer_delete ( GTK_TEXT_BUFFER( buffer ), &iter_start, &iter_end );
}

gboolean is_valid_string(gchar* line) {
    for (int i=0; i!=strlen(line); i++) {
        gunichar c = line[i];
        if (c == '_') {
            continue;
        }
        if (c == '.') {
            continue;
        }
        if (g_unichar_isalpha(c) == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

GtkTextIter start, end;
int active = 0;

gchar* extract_word(gchar* line, gint offset, GtkTextIter* iter, GtkTextBuffer* sourcebuff, GtkWidget* scroll) {
    gint left, right, i, j;
    gchar buffer[1024];
    gchar* match;

    for (i = offset; i!=0; i--) {
        if ((line[i] == '\n')) {
            left = i;
            break;
        }
        if ( (line[i] == ' ') || (line[i] == '[') || (line[i] == ']') || (line[i] == '*') || (line[i] == '(') || (line[i] == ')') || (line[i] == '=') || (line[i] == ';') || (line[i] == '{') || (line[i] == '}') || (line[i] == '+') || (line[i] == '-') || (line[i] == '&') || (line[i] == ',') || (line[i] == '/') || (line[i] == '>') || (line[i] == '<') ) {
            left = i+1;
            break;
        }
    }
    for (i = offset; i!=strlen(line); i++) {
        if ((line[i] == '\n')) {
            right = i;
            break;
        }
        if ( (line[i] == ' ') || (line[i] == '[') || (line[i] == ']') || (line[i] == '*') || (line[i] == '(') || (line[i] == ')') || (line[i] == '=') || (line[i] == ';') || (line[i] == '{') || (line[i] == '}') || (line[i] == '+') || (line[i] == '-') || (line[i] == '&') || (line[i] == ',') || (line[i] == '/') || (line[i] == '>') || (line[i] == '<') ) {
            right = i;
            break;
        }
    }
    for (j=0, i = left; i < right; i++, j++) {
       buffer[j] = line[i];
    }
    buffer[j] = '\0';
    g_print("buffer (%d:%d) [%s] \n", left, right, buffer);

    if (active) {
        gtk_text_buffer_remove_tag_by_name (GTK_TEXT_BUFFER(sourcebuff), "blue", &start, &end);
        gtk_text_buffer_remove_tag_by_name (GTK_TEXT_BUFFER(sourcebuff), "black", &start, &end);
        gtk_text_buffer_remove_tag_by_name (GTK_TEXT_BUFFER(sourcebuff), "italic", &start, &end);
        gtk_text_buffer_remove_tag_by_name (GTK_TEXT_BUFFER(sourcebuff), "underline", &start, &end);
    }

    start = *iter;
    end = *iter;
    gtk_text_iter_set_line_index (&start, left);
    gtk_text_iter_set_line_index (&end, right);
    match = gtk_text_iter_get_text (&start, &end);
    g_print("match [%d]: %s \n", strlen(match), match);

    /*
    NIE DZIAŁA

    if (strlen(match) != 0) {
        GdkCursor *cursor;
        GdkDisplay *display;

        display = gdk_display_get_default ();
        cursor = gdk_cursor_new_from_name(display, "grab");
        g_assert (cursor != NULL);

        GtkWidget* win = gtk_widget_get_ancestor(scroll, GTK_TYPE_WINDOW);
        gdk_window_set_cursor (gtk_widget_get_window(win), cursor);
    }
    */

    /*
    gtk_text_iter_backward_chars (&start, i-left);
    gtk_text_iter_forward_chars (&end, right-i);
    match = gtk_text_iter_get_text (&start, &end);
    g_print("match: %s \n", match);
    */

    if (is_valid_string(match)) {
        GtkTextTagTable* table = gtk_text_buffer_get_tag_table (GTK_TEXT_BUFFER(sourcebuff));
        GtkTextTag* tag1 = gtk_text_tag_table_lookup (table, "blue");
        GtkTextTag* tag2 = gtk_text_tag_table_lookup (table, "black");
        GtkTextTag* tag3 = gtk_text_tag_table_lookup (table, "italic");
        GtkTextTag* tag4 = gtk_text_tag_table_lookup (table, "underline");
        gtk_text_buffer_apply_tag (GTK_TEXT_BUFFER(sourcebuff), tag1,  &start, &end);
        gtk_text_buffer_apply_tag (GTK_TEXT_BUFFER(sourcebuff), tag2,  &start, &end);
        gtk_text_buffer_apply_tag (GTK_TEXT_BUFFER(sourcebuff), tag3,  &start, &end);
        gtk_text_buffer_apply_tag (GTK_TEXT_BUFFER(sourcebuff), tag4,  &start, &end);
        active = 1;
    }
}

// http://www.bravegnu.org/gtktext/x498.html
static gboolean mouse_moved(GtkWidget *widget, GdkEvent *event, gpointer scroll) {
    gint window_x, window_y;
    gint buffer_x, buffer_y;
    GtkTextIter iter, start, end;
    gchar *msg;
    gint row, col;

    if (event->type==GDK_MOTION_NOTIFY) {
        GdkEventMotion* e=(GdkEventMotion*)event;

        window_x = (guint)e->x;
        window_y = (guint)e->y;

        //printf("Coordinates: (%u,%u)\n", window_x, window_y);
        
        gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW(widget), GTK_TEXT_WINDOW_TEXT, window_x, window_y, &buffer_x, &buffer_y);

        //printf("Coordinates: (%u,%u) -> (%u,%u)  ", window_x, window_y, buffer_x, buffer_y);

        gtk_text_view_get_iter_at_location (GTK_TEXT_VIEW(widget), &iter, buffer_x, buffer_y);

        row = gtk_text_iter_get_line(&iter);
        col = gtk_text_iter_get_line_offset(&iter);

        msg = g_strdup_printf("Col: %d Ln: %d \n", col+1, row+1);
        g_print(msg);
        //g_print("\n");


        GtkTextBuffer* buffer = GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget)));
        gtk_text_buffer_get_iter_at_line (buffer, &start, row);
        gtk_text_buffer_get_iter_at_line (buffer, &end, row+1);
        msg = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

        if (msg) {
            g_print("OK: %d : %s \n", strlen(msg), msg);
            extract_word(msg, col, &iter, buffer, scroll);
        } else {
            g_print("BAD\n");
        }


        /*
        g_print(g_strescape(msg, NULL));
        */

        g_free(msg);
    }
}

GtkWidget*
sourceview_new(GtkSourceBuffer* buffer) 
{
    GtkWidget *scroll, *sourceview;

    scroll = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    sourceview = gtk_source_view_new_with_buffer(buffer);
    gtk_source_view_set_show_right_margin(GTK_SOURCE_VIEW(sourceview), TRUE);
    gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(sourceview), TRUE);
    gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(sourceview), TRUE);
    gtk_container_add (GTK_CONTAINER (scroll), GTK_WIDGET (sourceview));

    /* change font */
    GtkCssProvider *provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_data (provider,
                                     "textview { font-family: Monospace; font-size: 11pt; }",
                                     -1,
                                     NULL);
    gtk_style_context_add_provider (gtk_widget_get_style_context (sourceview),
                                    GTK_STYLE_PROVIDER (provider),
                                    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref (provider);

    gtk_source_view_set_tab_width (GTK_SOURCE_VIEW(sourceview), 4);

    // DO ODBLOKOWANIA
    //g_signal_connect (G_OBJECT (sourceview), "motion-notify-event",G_CALLBACK (mouse_moved), scroll);

    return scroll;
}
/*
#if !SOURCEVIEW

int main( int argc, char *argv[] ) {

    GtkWidget *window;
    GtkWidget *scroll;
    GtkWidget *srcview;
    GtkSourceBuffer *buffer;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show(window);

    scroll = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    buffer = GTK_SOURCE_BUFFER (gtk_source_buffer_new (NULL));
    srcview = sourceview_new(buffer);

    gtk_container_add (GTK_CONTAINER (scroll), GTK_WIDGET (srcview));
    gtk_container_add (GTK_CONTAINER (window), scroll);

    g_signal_connect(window, "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all (window);
    gtk_main();

    return 0;
}

#endif
*/