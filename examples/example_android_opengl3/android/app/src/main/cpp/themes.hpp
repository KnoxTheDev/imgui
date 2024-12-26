// themes.h
#ifndef THEMES_H
#define THEMES_H

#include "imgui.h"

namespace ImGui {
    class Options {
    public:
        // Apply the themes
        Options& cinderDark();
        Options& monochromeBlue();
    private:
        ImGuiStyle mStyle;
    };
}

#endif // THEMES_H
