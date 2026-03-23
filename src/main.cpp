#include <gtk/gtk.h>
#include <vector>
#include <tuple>

/* = HELPERS = */

void handle_toggle_group(GtkToggleButton* active, std::vector<GtkToggleButton*>* group)
{
    if (!gtk_toggle_button_get_active(active))
        return;

    for (auto btn : *group) {
        if (btn != active) {
            gtk_toggle_button_set_active(btn, FALSE);
        }
    }
}

void clear_group(std::vector<GtkToggleButton*>* group)
{
    for (auto btn : *group) {
        gtk_toggle_button_set_active(btn, FALSE);
    }
}

/* = CALLBACKS = */

static void on_group_toggled(GtkToggleButton* btn, gpointer data)
{
    handle_toggle_group(btn, static_cast<std::vector<GtkToggleButton*>*>(data));
}

static void on_preset_clears_manual(GtkToggleButton* btn, gpointer data)
{
    if (!gtk_toggle_button_get_active(btn))
        return;

    auto groups = static_cast<std::tuple<
        std::vector<GtkToggleButton*>*,
        std::vector<GtkToggleButton*>*,
        std::vector<GtkToggleButton*>*
    >*>(data);

    clear_group(std::get<0>(*groups)); // CPU
    clear_group(std::get<1>(*groups)); // GPU
    clear_group(std::get<2>(*groups)); // FAN
}

static void on_manual_clears_presets(GtkToggleButton* btn, gpointer data)
{
    if (!gtk_toggle_button_get_active(btn))
        return;

    clear_group(static_cast<std::vector<GtkToggleButton*>*>(data));
}

/* ACTIVATE */

static void activate(GtkApplication* app, gpointer user_data)
{
    GtkBuilder* builder = gtk_builder_new_from_file("../ui/main_window.ui");

    GtkWidget* window = GTK_WIDGET(
        gtk_builder_get_object(builder, "main_window")
    );

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));

    /* DROPDOWN */

    GtkComboBoxText* dropdown = GTK_COMBO_BOX_TEXT(
        gtk_builder_get_object(builder, "refresh_rate_dropdown")
    );

    gtk_combo_box_text_append_text(dropdown, "60 Hz");
    gtk_combo_box_text_append_text(dropdown, "120 Hz");
    gtk_combo_box_text_append_text(dropdown, "144 Hz");
    gtk_combo_box_text_append_text(dropdown, "240 Hz");

    gtk_combo_box_set_active(GTK_COMBO_BOX(dropdown), 0);

    /* CPU */

    auto cpu_power = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "power_save_cpu_button"));
    auto cpu_bal = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "balanced_cpu_button"));
    auto cpu_perf = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "performance_cpu_button"));

    auto cpu_group = new std::vector<GtkToggleButton*>{cpu_power, cpu_bal, cpu_perf};

    for (auto btn : *cpu_group)
        g_signal_connect(btn, "toggled", G_CALLBACK(on_group_toggled), cpu_group);

    /* GPU */

    auto gpu_i = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "integrated_button"));
    auto gpu_h = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "both_button"));
    auto gpu_d = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "dedicated_button"));

    auto gpu_group = new std::vector<GtkToggleButton*>{gpu_i, gpu_h, gpu_d};

    for (auto btn : *gpu_group)
        g_signal_connect(btn, "toggled", G_CALLBACK(on_group_toggled), gpu_group);

    /* FAN */

    auto fan_q = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "quiet_fan_button"));
    auto fan_b = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "balanced_fan_button"));
    auto fan_p = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "performance_fan_button"));

    auto fan_group = new std::vector<GtkToggleButton*>{fan_q, fan_b, fan_p};

    for (auto btn : *fan_group)
        g_signal_connect(btn, "toggled", G_CALLBACK(on_group_toggled), fan_group);

    /* PRESETS */

    auto p_save = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "power_saving_button"));
    auto p_bal = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "balanced_button"));
    auto p_perf = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "performance_button"));

    auto preset_group = new std::vector<GtkToggleButton*>{p_save, p_bal, p_perf};

    for (auto btn : *preset_group)
        g_signal_connect(btn, "toggled", G_CALLBACK(on_group_toggled), preset_group);

    /* CLEAR */

    auto preset_data = new std::tuple(cpu_group, gpu_group, fan_group);

    for (auto btn : *preset_group)
        g_signal_connect(btn, "toggled", G_CALLBACK(on_preset_clears_manual), preset_data);

    for (auto btn : *cpu_group)
        g_signal_connect(btn, "toggled", G_CALLBACK(on_manual_clears_presets), preset_group);

    for (auto btn : *gpu_group)
        g_signal_connect(btn, "toggled", G_CALLBACK(on_manual_clears_presets), preset_group);

    for (auto btn : *fan_group)
        g_signal_connect(btn, "toggled", G_CALLBACK(on_manual_clears_presets), preset_group);
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