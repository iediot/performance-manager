//
// Created by edi on 3/24/26.
//

#include <cstdlib>
#include <string>
#include <iostream>

int main(int argc, char** argv)
{
    std::string cpu, gpu, fan;
    int hz = 0;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg.find("cpu=") == 0)
            cpu = arg.substr(4);
        else if (arg.find("gpu=") == 0)
            gpu = arg.substr(4);
        else if (arg.find("fan=") == 0)
            fan = arg.substr(4);
    }

    /* CPU */
    if (cpu == "powersave")
        system("/usr/bin/cpupower frequency-set -g powersave");
    else if (cpu == "balanced")
        system("/usr/bin/cpupower frequency-set -g schedutil");
    else if (cpu == "performance")
        system("/usr/bin/cpupower frequency-set -g performance");

    /* GPU */
    if (gpu == "integrated")
        system("/usr/bin/supergfxctl -m Integrated");
    else if (gpu == "hybrid")
        system("/usr/bin/supergfxctl -m Hybrid");
    else if (gpu == "dedicated")
        system("/usr/bin/supergfxctl -m Dedicated");

    /* FAN */
    if (fan == "quiet")
    {
        system("/usr/bin/asusctl profile -P quiet");
        system("/usr/bin/asusctl profile -a quiet");
        system("/usr/bin/asusctl profile -b quiet");
    }
    else if (fan == "balanced")
    {
        system("/usr/bin/asusctl profile -P balanced");
        system("/usr/bin/asusctl profile -a balanced");
        system("/usr/bin/asusctl profile -b balanced");
    }
    else if (fan == "performance")
    {
        system("/usr/bin/asusctl profile -P performance");
        system("/usr/bin/asusctl profile -a performance");
        system("/usr/bin/asusctl profile -b performance");
    }

    return 0;
}