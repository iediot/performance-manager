//
// Created by edi on 3/24/26.
//

#include "SysController.h"
#include <cstdlib>
#include <string>
#include <iostream>


void apply_all(const std::string& cpu,
               const std::string& gpu,
               const std::string& fan,
               int hz)
{
    std::string cmd = "pkexec /usr/local/bin/perf-helper";

    if (!cpu.empty()) cmd += " cpu=" + cpu;
    if (!gpu.empty()) cmd += " gpu=" + gpu;
    if (!fan.empty()) cmd += " fan=" + fan;

    system(cmd.c_str());

    if (hz > 0) {
        std::string hz_cmd =
            "/usr/bin/hyprctl keyword monitor eDP-1,1920x1200@" +
            std::to_string(hz) + ",auto,1.2";

        system(hz_cmd.c_str());
    }
}

/* PRESETS */

void apply_preset(const std::string& preset)
{
    if (preset == "powersave") {
        apply_all("powersave", "integrated", "quiet", 60);
    }
    else if (preset == "balanced") {
        apply_all("balanced", "hybrid", "balanced", 120);
    }
    else if (preset == "performance") {
        apply_all("performance", "dedicated", "performance", 120);
    }
}

void reboot_system()
{
    system("pkexec /usr/local/bin/perf-helper reboot");
}
