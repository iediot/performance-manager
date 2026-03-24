//
// Created by edi on 3/24/26.
//

#include "SysController.h"
#include <cstdlib>
#include <string>
#include <iostream>


void apply_all(const std::string& cpu,
               const std::string& gpu,
               const std::string& fan)
{
    std::string cmd = "pkexec /usr/local/bin/perf-helper";

    if (!cpu.empty()) cmd += " cpu=" + cpu;
    if (!gpu.empty()) cmd += " gpu=" + gpu;
    if (!fan.empty()) cmd += " fan=" + fan;

    system(cmd.c_str());
}

/* PRESETS */

void apply_preset(const std::string& preset)
{
    if (preset == "powersave") {
        apply_all("powersave", "integrated", "quiet");
    }
    else if (preset == "balanced") {
        apply_all("balanced", "hybrid", "balanced");
    }
    else if (preset == "performance") {
        apply_all("performance", "dedicated", "performance");
    }
}