//
// Created by edi on 3/24/26.
//

#ifndef GTKAPP_SYSCONTROLLER_H
#define GTKAPP_SYSCONTROLLER_H

#pragma once
#include <string>

void apply_all(const std::string& cpu,
               const std::string& gpu,
               const std::string& fan,
               int hz);

void apply_preset(const std::string& preset);

void reboot_system();

#endif //GTKAPP_SYSCONTROLLER_H