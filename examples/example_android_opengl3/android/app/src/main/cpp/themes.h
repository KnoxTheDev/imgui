// themes.h
#ifndef THEMES_H
#define THEMES_H

#include "imgui.h"

namespace ImGui {
    class Options {
    public:
        // Apply the Adobe White theme (light theme)
        Options& adobeWhite();

        // Apply the Cinder Dark theme (dark theme)
        Options& cinderDark();

    private:
        ImGuiStyle mStyle;
    };
}

#endif // THEMES_H
