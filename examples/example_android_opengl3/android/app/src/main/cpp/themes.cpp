#include "themes.h"

namespace ImGui {

    // Adobe White theme implementation (light theme).
    Options& Options::adobeWhite() {
        ImGuiStyle* style = &ImGui::GetStyle();
        style->GrabRounding = 4.0f;

        ImVec4* colors = style->Colors;

        colors[ImGuiCol_Text]                  = ImColor(0x212121FF); // Text color
        colors[ImGuiCol_TextDisabled]          = ImColor(0x999999FF); // Disabled text color
        colors[ImGuiCol_WindowBg]              = ImColor(0xFAFAFAFF); // Window background color
        colors[ImGuiCol_ChildBg]               = ImColor(0x00000000); // Child background transparent
        colors[ImGuiCol_PopupBg]               = ImColor(0xFAFAFAFF); // Popup background color
        colors[ImGuiCol_Border]                = ImColor(0xCCCCCCFF); // Border color
        colors[ImGuiCol_BorderShadow]          = ImColor(0x00000000); // Border shadow off
        colors[ImGuiCol_FrameBg]               = ImColor(0xB0B0B0FF); // Frame background
        colors[ImGuiCol_FrameBgHovered]        = ImColor(0xE0E0E0FF); // Frame hover color
        colors[ImGuiCol_FrameBgActive]         = ImColor(0xD0D0D0FF); // Frame active color
        colors[ImGuiCol_TitleBg]               = ImColor(0xCCCCCCFF); // Title background color
        colors[ImGuiCol_TitleBgActive]         = ImColor(0xD0D0D0FF); // Active title color
        colors[ImGuiCol_TitleBgCollapsed]      = ImColor(0xB0B0B0FF); // Collapsed title color
        colors[ImGuiCol_MenuBarBg]             = ImColor(0xFAFAFAFF); // Menu bar background
        colors[ImGuiCol_ScrollbarBg]           = ImColor(0xFAFAFAFF); // Scrollbar background
        colors[ImGuiCol_ScrollbarGrab]         = ImColor(0xB0B0B0FF); // Scrollbar grab color
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImColor(0xA0A0A0FF); // Scrollbar hover color
        colors[ImGuiCol_ScrollbarGrabActive]   = ImColor(0x909090FF); // Scrollbar active color
        colors[ImGuiCol_CheckMark]             = ImColor(0x00A9F0FF); // Check mark color
        colors[ImGuiCol_SliderGrab]            = ImColor(0x909090FF); // Slider grab color
        colors[ImGuiCol_SliderGrabActive]      = ImColor(0x707070FF); // Slider grab active
        colors[ImGuiCol_Button]                = ImColor(0xB0B0B0FF); // Button color
        colors[ImGuiCol_ButtonHovered]         = ImColor(0xE0E0E0FF); // Button hover color
        colors[ImGuiCol_ButtonActive]          = ImColor(0xD0D0D0FF); // Button active color
        colors[ImGuiCol_Header]                = ImColor(0x0069B0FF); // Header color
        colors[ImGuiCol_HeaderHovered]         = ImColor(0x0080D0FF); // Header hover color
        colors[ImGuiCol_HeaderActive]          = ImColor(0x0069B0FF); // Header active color
        colors[ImGuiCol_Separator]             = ImColor(0xD0D0D0FF); // Separator color
        colors[ImGuiCol_SeparatorHovered]      = ImColor(0xA0A0A0FF); // Separator hover color
        colors[ImGuiCol_SeparatorActive]       = ImColor(0x909090FF); // Separator active color
        colors[ImGuiCol_ResizeGrip]            = ImColor(0xB0B0B0FF); // Resize grip color
        colors[ImGuiCol_ResizeGripHovered]     = ImColor(0xA0A0A0FF); // Resize grip hover color
        colors[ImGuiCol_ResizeGripActive]      = ImColor(0x909090FF); // Resize grip active color
        colors[ImGuiCol_PlotLines]             = ImColor(0x0069B0FF); // Plot lines color
        colors[ImGuiCol_PlotLinesHovered]      = ImColor(0x0080D0FF); // Plot lines hover color
        colors[ImGuiCol_PlotHistogram]         = ImColor(0x0069B0FF); // Plot histogram color
        colors[ImGuiCol_PlotHistogramHovered]  = ImColor(0x0080D0FF); // Plot histogram hover color
        colors[ImGuiCol_TextSelectedBg]        = ImColor(0x0069B033); // Text selected background
        colors[ImGuiCol_DragDropTarget]        = ImColor(0xFFFF00E6); // Drag & drop target
        colors[ImGuiCol_NavCursor]             = ImColor(0x21212166); // Nav cursor color
        colors[ImGuiCol_NavWindowingHighlight] = ImColor(0xFFFFFFB3); // Nav window highlight
        colors[ImGuiCol_NavWindowingDimBg]     = ImColor(0xCCCCCC33); // Nav window dim background
        colors[ImGuiCol_ModalWindowDimBg]      = ImColor(0x33333359); // Modal window dim background
        colors[ImGuiCol_Tab]                   = ImColor(0xCCCCCCFF); // Tab color
        colors[ImGuiCol_TabSelected]           = ImColor(0x00A9F0FF); // Tab selected color
        colors[ImGuiCol_TabHovered]            = ImColor(0x0080D0FF); // Tab hover color
        colors[ImGuiCol_TabDimmed]             = ImColor(0xB0B0B0FF); // Dimmed tab color
        colors[ImGuiCol_TabDimmedSelected]     = ImColor(0x0080D0FF); // Dimmed selected tab color

        ImGui::GetStyle() = *style;

        return *this;
    }

    // Cinder Dark theme implementation (dark theme).
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
