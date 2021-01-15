#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

//#include "list.h"
#include "config.h"
#include "notebook.h"
#include "treeview.h"
#include "callback.h"
#include "text_editor.h"

#define COLUMN 0

//g_printerr("%s\n",
//       gdk_keyval_name (event->keyval));

gboolean is_valid_char (gchar c) {
    if( (c == ' ') || 
        (c == '\t') || 
        (c == '\n') || 
        (c == '[') || 
        (c == ']') || 
        (c == '*') || 
        (c == '(') || 
        (c == ')') || 
        (c == '=') || 
        (c == ';') || 
        (c == ':') || 
        (c == '{') || 
        (c == '}') || 
        (c == '+') || 
        (c == '-') || 
        (c == '&') || 
        (c == ',') || 
        (c == '/') || 
        (c == '.') || 
        (c == '>') || 
        (c == '<') ) 
    {
        return FALSE;
    }
    return TRUE;
}
/*
gboolean is_inside_comment (gpointer user_data, gint line, gint col) {
    GtkTextIter start, end;
    GtkTextBuffer* buffer;

    buffer = GET_TEXT_BUFFER(user_data);

    gtk_text_buffer_get_bounds (buffer, &start, &end);

    gchar* text = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);

    for (int i=0; i<strlen(text); i++) {

    }

    //return TRUE;
    //return FALSE;
}
*/
gboolean check_if_method_or_member (gchar* line, gint left) {
    gchar c1 = line[left-1];
    gchar c2 = line[left-2];
    if (c1=='.') {
        return TRUE;
    }
    if (c1=='>' && c2=='-') {
        return TRUE;
    }
    return FALSE;
}

gchar* extract_word(gchar* line, gint row, gint offset, UserData* user_data) 
{
    //g_print("%s \n", line);

    int left, right, i, j;
    //gchar buffer[1024];

    if ( !is_valid_char(line[offset-1]) || !is_valid_char(line[offset]) ) {
        return NULL;
    }

    for (i=offset-1; i>=0; i--) {
        //g_print("L %d %c\n", i, line[i]);
        if (!is_valid_char(line[i])) {
            break;
        }
        left = i;
    }
    //g_print("[%d]---", left);
    //g_print("---------\n");

    for (i=offset; i<=strlen(line); i++) {
        //g_print("R %d %c\n", i, line[i]);
        right = i;
        if (!is_valid_char(line[i])) {
            break;
        }
    }

    //g_print("[%d]---\n", right);
    //g_print("%d-%d\n", left, right);

    for (i=left; i<right; i++) {
        g_print("%c", line[i]);
    }
    g_print("\n");

    GtkTextIter start, end;
    GtkTextBuffer* buffer = GET_TEXT_BUFFER(user_data);
    GtkTextTagTable* table = gtk_text_buffer_get_tag_table (buffer);
    gtk_text_buffer_get_iter_at_line_index (buffer, &start, row, left);
    gtk_text_buffer_get_iter_at_line_index (buffer, &end, row, right);

    GtkTextTag* tag1 = gtk_text_tag_table_lookup (table, "blue");
    GtkTextTag* tag2 = gtk_text_tag_table_lookup (table, "black");
    GtkTextTag* tag3 = gtk_text_tag_table_lookup (table, "italic");
    GtkTextTag* tag4 = gtk_text_tag_table_lookup (table, "underline");

    gtk_text_buffer_apply_tag (buffer, tag1,  &start, &end);
    gtk_text_buffer_apply_tag (buffer, tag2,  &start, &end);
    gtk_text_buffer_apply_tag (buffer, tag3,  &start, &end);
    gtk_text_buffer_apply_tag (buffer, tag4,  &start, &end);


    /*
    gboolean is_member = check_if_method_or_member(line, left);
    g_print("%d\n", is_member);

    if (is_member) {
        extract_word (line, left-3);
    }
    */


    /*
    for (j=0, i=left; i<right; i++, j++) {
        buffer[j] = line[i];
    }

    //g_print("buffer: %s , left %d, tight %d\n", buffer, left, right);
    g_print("left %d, tight %d\n", left, right);
    */

    /*
    gint left, right, i, j;
    gchar buffer[1024];
    gchar* match;

    for (i = offset; i!=0; i--) {
        if ((line[i] == '\n')) {
            left = i;
            break;
        }
        if (is_valid_char(line[i])) {
            left = i+1;
            break;
        }
    }
    for (i = offset; i!=strlen(line); i++) {
        if ((line[i] == '\n')) {
            right = i;
            break;
        }
        if (is_valid_char(line[i])) {
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
    }*/
}

// http://www.bravegnu.org/gtktext/x498.html
gboolean print_word_under_mark (gpointer user_data) 
{
    GtkTextIter iter, start, end;
    gchar *msg;
    gint row, col;
    GtkTextBuffer* buffer;

    buffer = GET_TEXT_BUFFER(user_data);

    gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));

    row = gtk_text_iter_get_line(&iter);
    col = gtk_text_iter_get_line_offset(&iter);

    gtk_text_buffer_get_iter_at_line (buffer, &start, row);
    gtk_text_buffer_get_iter_at_line_index (buffer, &end, row, 9999);

    msg = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

    if (msg) {
        g_print("OK: %d : /%s/ \n", strlen(msg), msg);
        //extract_word(msg, col, &iter, buffer, scroll);
        extract_word (msg, row, col, user_data);
    } else {
        g_print("BAD\n");
    }
}

gboolean
key_pressed_window(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if (event->state & GDK_CONTROL_MASK && event->keyval == 'F') {
        g_print ("ctrl + shift + f \n");
        full_search_cb (widget, user_data);
    }
    if (event->state & GDK_CONTROL_MASK && event->keyval == 'N') {
        g_print ("ctrl + shift + n \n");
        find_files_cb (widget, user_data);
    }
    if (event->state & GDK_CONTROL_MASK && event->keyval == 's') {
        /*
        GtkTextIter start, end;
        GtkTextBuffer* buffer = GET_TEXT_BUFFER(user_data);

        gtk_text_buffer_get_bounds (buffer, &start, &end);
        gchar* text = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
        g_print("%s \n", text);
        */

        print_word_under_mark (user_data);

        //g_print ("ctrl + s \n");
        //save_file_default (user_data);
        //save_file ("/home/rafal/IdeaProjects/gtksourceview-my-ide/application/aaa.c", "Aaaaaaaaaaaaaaaaaaaaa");
    }

    return FALSE;
}
