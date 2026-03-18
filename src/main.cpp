#include <gtk/gtk.h>

static void activate(GtkApplication* app, gpointer user_data)
{
    GtkBuilder* builder = gtk_builder_new_from_file("../ui/main_window.ui");

    GtkWidget* window = GTK_WIDGET(
        gtk_builder_get_object(builder, "main_window")
    );

    gtk_window_set_application(GTK_WINDOW(window), app);

    gtk_widget_show(window);
}

int main(int argc, char** argv)
{
    GtkApplication* app = gtk_application_new(
        "com.iediot.performancemanager",
        G_APPLICATION_DEFAULT_FLAGS
    );

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}