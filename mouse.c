#include<gtk/gtk.h>
#include<stdio.h>

static void destroy(GtkWidget*, gpointer);
static gboolean mouse_moved(GtkWidget *widget,GdkEvent *event,gpointer user_data);

int main(int argc, char* argv[]) {

    GtkWidget *main_window;

    // initializing
    gtk_init(&argc, &argv);

    main_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window),"Test");
    gtk_widget_set_size_request (main_window, 500, 300);

    // connect the window with signals
    g_signal_connect (G_OBJECT (main_window), "destroy",G_CALLBACK (destroy), NULL);
    g_signal_connect (G_OBJECT (main_window), "motion-notify-event",G_CALLBACK (mouse_moved), NULL);

    gtk_widget_set_events(main_window, GDK_POINTER_MOTION_MASK);

    // show window
    gtk_widget_show_all (main_window);

    gtk_main();
    return 0;
}


static void destroy(GtkWidget *window,gpointer data) {
    gtk_main_quit ();
}


static gboolean mouse_moved(GtkWidget *widget,GdkEvent *event, gpointer user_data) {

    if (event->type==GDK_MOTION_NOTIFY) {
        GdkEventMotion* e=(GdkEventMotion*)event;
        printf("Coordinates: (%u,%u)\n", (guint)e->x,(guint)e->y);
    }
}
