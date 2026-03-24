#include <gtk/gtk.h>
#include "MainWindow.h"

static void activate(GtkApplication* app, gpointer)
{
    create_main_window(app);
}

int run_app(int argc, char** argv)
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