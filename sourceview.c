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

GtkWidget*
sourceview_new(GtkSourceBuffer* buffer) {
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

    return scroll;
}

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