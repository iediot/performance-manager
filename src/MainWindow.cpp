//
// Created by edi on 3/18/26.
//

#include <gtk/gtk.h>
#include <vector>
#include <tuple>
#include "SysController.h"
#include <string>

/* GETTERS */

std::string get_preset(GtkBuilder* builder)
{
    auto p = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "power_saving_button"));
    auto b = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "balanced_button"));
    auto f = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "performance_button"));

    if (gtk_toggle_button_get_active(p)) return "powersave";
    if (gtk_toggle_button_get_active(b)) return "balanced";
    if (gtk_toggle_button_get_active(f)) return "performance";

    return "";
}

std::string get_cpu_mode(GtkBuilder* builder)
{
    auto p = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "power_save_cpu_button"));
    auto b = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "balanced_cpu_button"));
    auto f = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "performance_cpu_button"));

    if (gtk_toggle_button_get_active(p)) return "powersave";
    if (gtk_toggle_button_get_active(b)) return "balanced";
    if (gtk_toggle_button_get_active(f)) return "performance";

    return "";
}

std::string get_gpu_mode(GtkBuilder* builder)
{
    auto i = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "integrated_button"));
    auto h = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "both_button"));
    auto d = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "dedicated_button"));

    if (gtk_toggle_button_get_active(i)) return "integrated";
    if (gtk_toggle_button_get_active(h)) return "hybrid";
    if (gtk_toggle_button_get_active(d)) return "dedicated";

    return "";
}

std::string get_fan_mode(GtkBuilder* builder)
{
    auto q = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "quiet_fan_button"));
    auto b = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "balanced_fan_button"));
    auto p = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "performance_fan_button"));

    if (gtk_toggle_button_get_active(q)) return "quiet";
    if (gtk_toggle_button_get_active(b)) return "balanced";
    if (gtk_toggle_button_get_active(p)) return "performance";

    return "";
}

int get_refresh_rate(GtkBuilder* builder)
{
    GtkComboBoxText* dropdown = GTK_COMBO_BOX_TEXT(
        gtk_builder_get_object(builder, "refresh_rate_dropdown")
    );

    const char* text = gtk_combo_box_text_get_active_text(dropdown);

    if (!text) return 0;

    std::string str = text;

    if (str == "60 Hz") return 60;
    if (str == "120 Hz") return 120;
    if (str == "144 Hz") return 144;
    if (str == "240 Hz") return 240;

    return 0;
}

/* HELPERS */

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

/* CALLBACKS */

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

void create_main_window(GtkApplication* app)
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

    auto apply_btn = GTK_BUTTON(
    gtk_builder_get_object(builder, "apply_button")
    );

    g_signal_connect(apply_btn, "clicked",
    G_CALLBACK(+[](GtkButton*, gpointer data)
{
        GtkBuilder* builder = static_cast<GtkBuilder*>(data);

    auto preset = get_preset(builder);
    auto hz = get_refresh_rate(builder);

    if (!preset.empty()) {
        apply_preset(preset);
    }
    else {
        auto cpu = get_cpu_mode(builder);
        auto gpu = get_gpu_mode(builder);
        auto fan = get_fan_mode(builder);

        apply_all(cpu, gpu, fan, hz);;
    }
    }), builder);

    GtkButton* restart_btn = GTK_BUTTON(
    gtk_builder_get_object(builder, "restart_button")
);

    g_signal_connect(restart_btn, "clicked",
        G_CALLBACK(+[](GtkButton*, gpointer) {
            reboot_system();
        }),
        NULL);
}
