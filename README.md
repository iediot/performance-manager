<h1 align="center">Performance Manager</h1>

<p align="center">
A lightweight GTK-based tool for managing CPU, GPU, fan profiles, and display refresh rate on Linux systems designed for ASUS laptops and Hyprland users.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Linux-grey?logo=linux&logoColor=white">
  <img src="https://img.shields.io/badge/C++-blue?logo=c%2B%2B&logoColor=white">
  <img src="https://img.shields.io/badge/GTK-4-green?logo=gnome&logoColor=white">
  <img src="https://img.shields.io/badge/Hyprland-purple">
</p>

---

### How it works

1. Select a **preset** or configure CPU, GPU, and fan manually  
2. Choose a **refresh rate**  
3. Click **Apply** to apply changes instantly  
4. Optionally click **Restart** if required  

---

### Features

- CPU governor control (`cpupower`)
- GPU mode switching (`supergfxctl`)
- Fan profiles (`asusctl`)
- Refresh rate switching (`hyprctl`)
- Preset system (Power Saving / Balanced / Performance)
- Polkit integration (no sudo needed in UI)

---

### Requirements

- Linux (tested on Arch Linux)
- GTK4
- Hyprland (for refresh rate control)
- `cpupower`
- `supergfxctl`
- `asusctl`
- `polkit`

---

### Build from source

    git clone https://github.com/iediot/performance-manager.git
    cd performance-manager
    mkdir build
    cd build
    cmake ..
    cmake --build .

---

### Install

    # Install app locally
    cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local ..
    cmake --build .
    cmake --install .

    # Install UI file
    mkdir -p ~/.local/share/performance-manager
    cp ui/main_window.ui ~/.local/share/performance-manager/

    # Install helper (requires root)
    sudo cp perf-helper /usr/local/bin/perf-helper
    sudo chmod +x /usr/local/bin/perf-helper

---

### Polkit setup (required)

    sudo nano /etc/polkit-1/rules.d/10-performance-manager.rules

Paste:

    polkit.addRule(function(action, subject) {
        if (action.id == "org.freedesktop.policykit.exec" &&
            subject.user == "<your-username>") {
            return polkit.Result.YES;
        }
    });

---

### Usage

| Action | What happens |
|------|-------------|
| Select preset | Applies predefined CPU/GPU/FAN settings |
| Manual config | Overrides presets |
| Apply | Applies settings instantly |
| Restart | Reboots system |

---

### Useful commands

    # Run manually
    performance-manager

    # Test helper
    pkexec /usr/local/bin/perf-helper cpu=performance

    # Check logs (if needed)
    journalctl -xe

---

### Notes

- Refresh rate changes are **temporary** (reset on Hyprland reload)
- Designed primarily for ASUS laptops
- Requires proper permissions via polkit

---

### Future improvements

- Auto-detect monitor resolution and refresh rates  
- Remove UI file dependency (embed resources)  
- AUR package  

---
