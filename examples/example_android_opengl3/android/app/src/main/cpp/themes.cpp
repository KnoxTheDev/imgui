#include "themes.h"

namespace ImGui {

    // Adobe White theme implementation (light theme)
    Options& Options::adobeWhite() {
        // Set the style values for Adobe White theme (light theme)
        mStyle.WindowMinSize        = ImVec2(160, 20);
        mStyle.FramePadding         = ImVec2(4, 2);
        mStyle.ItemSpacing          = ImVec2(6, 2);
        mStyle.ItemInnerSpacing     = ImVec2(6, 4);
        mStyle.Alpha                = 1.00f; // Full opacity for light theme
        mStyle.WindowRounding       = 4.0f;
        mStyle.FrameRounding        = 2.0f;
        mStyle.IndentSpacing        = 6.0f;
        mStyle.ColumnsMinSpacing    = 50.0f;
        mStyle.GrabMinSize          = 14.0f;
        mStyle.GrabRounding         = 16.0f;
        mStyle.ScrollbarSize        = 12.0f;
        mStyle.ScrollbarRounding    = 16.0f;

        // Set the colors for Adobe White theme
        mStyle.Colors[ImGuiCol_Text]                  = ImVec4(0.00f, 0.00f, 0.00f, 1.00f); // Dark text for light theme
        mStyle.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f); // Disabled text
        mStyle.Colors[ImGuiCol_WindowBg]              = ImVec4(0.95f, 0.95f, 0.95f, 1.00f); // Light background
        mStyle.Colors[ImGuiCol_Border]                = ImVec4(0.80f, 0.80f, 0.80f, 1.00f); // Border color
        mStyle.Colors[ImGuiCol_FrameBg]               = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Frame background
        mStyle.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.80f, 0.80f, 0.80f, 0.78f); // Frame hover
        mStyle.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.70f, 0.70f, 0.70f, 1.00f); // Frame active
        mStyle.Colors[ImGuiCol_Button]                = ImVec4(0.24f, 0.70f, 1.00f, 1.00f); // Button color
        mStyle.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.18f, 0.64f, 0.90f, 1.00f); // Button hover color
        mStyle.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.14f, 0.56f, 0.76f, 1.00f); // Button active color
        mStyle.Colors[ImGuiCol_Header]                = ImVec4(0.24f, 0.70f, 1.00f, 0.76f); // Header color
        mStyle.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.18f, 0.64f, 0.90f, 0.86f); // Header hover
        mStyle.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.14f, 0.56f, 0.76f, 1.00f); // Header active
        mStyle.Colors[ImGuiCol_Separator]             = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Separator
        mStyle.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.80f, 0.80f, 0.80f, 0.78f); // Separator hover
        mStyle.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.70f, 0.70f, 0.70f, 1.00f); // Separator active
        mStyle.Colors[ImGuiCol_PopupBg]               = ImVec4(1.00f, 1.00f, 1.00f, 0.9f); // Popup background

        // Apply the custom Adobe White theme styles
        ImGui::GetStyle() = mStyle; // Apply the custom style to ImGui
        
        return *this;
    }

    // Cinder Dark theme implementation (dark theme)
    Options& Options::cinderDark() {
        // Set the style values for Cinder Dark theme (dark theme)
        mStyle.WindowMinSize        = ImVec2(160, 20);
        mStyle.FramePadding         = ImVec2(4, 2);
        mStyle.ItemSpacing          = ImVec2(6, 2);
        mStyle.ItemInnerSpacing     = ImVec2(6, 4);
        mStyle.Alpha                = 0.95f;
        mStyle.WindowRounding       = 4.0f;
        mStyle.FrameRounding        = 2.0f;
        mStyle.IndentSpacing        = 6.0f;
        mStyle.ColumnsMinSpacing    = 50.0f;
        mStyle.GrabMinSize          = 14.0f;
        mStyle.GrabRounding         = 16.0f;
        mStyle.ScrollbarSize        = 12.0f;
        mStyle.ScrollbarRounding    = 16.0f;

        // Set the colors for Cinder Dark theme
        mStyle.Colors[ImGuiCol_Text]                  = ImVec4(0.86f, 0.93f, 0.89f, 0.78f); // Light text on dark background
        mStyle.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.86f, 0.93f, 0.89f, 0.28f); // Disabled text
        mStyle.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 1.00f); // Dark background
        mStyle.Colors[ImGuiCol_Border]                = ImVec4(0.31f, 0.31f, 1.00f, 0.00f); // Border color
        mStyle.Colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f); // Frame background
        mStyle.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.92f, 0.18f, 0.29f, 0.78f); // Frame hover
        mStyle.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.92f, 0.18f, 0.29f, 1.00f); // Frame active
        mStyle.Colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f); // Button color
        mStyle.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.86f); // Button hover color
        mStyle.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f); // Button active color
        mStyle.Colors[ImGuiCol_Header]                = ImVec4(0.92f, 0.18f, 0.29f, 0.76f); // Header color
        mStyle.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.86f); // Header hover
        mStyle.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f); // Header active
        mStyle.Colors[ImGuiCol_Separator]             = ImVec4(0.14f, 0.16f, 0.19f, 1.00f); // Separator
        mStyle.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.92f, 0.18f, 0.29f, 0.78f); // Separator hover
        mStyle.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.92f, 0.18f, 0.29f, 1.00f); // Separator active

        // Apply the custom Cinder Dark theme styles
        ImGui::GetStyle() = mStyle; // Apply the custom style to ImGui
        
        return *this;
    }

}  // namespace ImGui
