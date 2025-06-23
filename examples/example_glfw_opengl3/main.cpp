#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// Hide console window on Windows
#if defined(_MSC_VER)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

#ifdef EMSCRIPTEN
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Custom styling function to replicate the UI from images
void ApplySnakeStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Set a consistent, sharp-edged style
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;

    // Define colors
    colors[ImGuiCol_WindowBg]           = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_ChildBg]            = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_PopupBg]            = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_FrameBg]            = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]     = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgActive]      = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

    colors[ImGuiCol_TitleBg]            = ImVec4(0.82f, 0.51f, 0.0f, 1.0f);
    colors[ImGuiCol_TitleBgActive]      = ImVec4(0.82f, 0.51f, 0.0f, 1.0f);
    colors[ImGuiCol_TitleBgCollapsed]   = ImVec4(0.82f, 0.51f, 0.0f, 1.0f);

    colors[ImGuiCol_Button]             = ImVec4(0.69f, 0.42f, 0.0f, 1.0f);
    colors[ImGuiCol_ButtonHovered]      = ImVec4(0.75f, 0.47f, 0.05f, 1.0f);
    colors[ImGuiCol_ButtonActive]       = ImVec4(0.80f, 0.50f, 0.10f, 1.0f);

    colors[ImGuiCol_Header]             = ImVec4(0.69f, 0.42f, 0.0f, 1.0f);
    colors[ImGuiCol_HeaderHovered]      = ImVec4(0.75f, 0.47f, 0.05f, 1.0f);
    colors[ImGuiCol_HeaderActive]       = ImVec4(0.80f, 0.50f, 0.10f, 1.0f);

    colors[ImGuiCol_Tab]                = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    colors[ImGuiCol_TabHovered]         = ImVec4(0.75f, 0.47f, 0.05f, 1.0f);
    colors[ImGuiCol_TabActive]          = ImVec4(0.69f, 0.42f, 0.0f, 1.0f);
    colors[ImGuiCol_TabUnfocused]       = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
    
    colors[ImGuiCol_CheckMark]          = ImVec4(0.82f, 0.51f, 0.0f, 1.0f);
    colors[ImGuiCol_SliderGrab]         = ImVec4(0.82f, 0.51f, 0.0f, 1.0f);
    colors[ImGuiCol_SliderGrabActive]   = ImVec4(0.9f, 0.6f, 0.1f, 1.0f);

    colors[ImGuiCol_Text]               = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_Separator]          = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_Border]             = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with a fixed size
    GLFWwindow* window = glfwCreateWindow(520, 480, "Snake Bypass", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Apply the custom style
    ApplySnakeStyle();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- SNAKE BYPASS Window ---
        {
            ImGui::SetNextWindowSize(ImVec2(520, 480), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            
            // Begin the window. Using NoTitleBar and NoMove flags.
            ImGui::Begin("SnakeBypassWindow", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
            
            // --- Custom Title Bar ---
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1,0,0,0.4f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1,0,0,0.7f));
            
            // Draw title bar rect
            ImGui::GetWindowDrawList()->AddRectFilled(
                ImGui::GetWindowPos(),
                ImVec2(ImGui::GetWindowPos().x + 520, ImGui::GetWindowPos().y + 30),
                ImGui::GetColorU32(ImGuiCol_TitleBgActive)
            );
            
            // Title text
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
            ImGui::Text("SNAKE BYPASS");

            // Close button
            ImGui::SameLine(ImGui::GetWindowWidth() - 35);
            if(ImGui::Button("X", ImVec2(25, 25))) {
                 glfwSetWindowShouldClose(window, true);
            }
            ImGui::PopStyleColor(3);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);


            // --- Tabs ---
            if (ImGui::BeginTabBar("MainTabBar"))
            {
                // -- BYPASS Tab --
                if (ImGui::BeginTabItem("BYPASS"))
                {
                    ImGui::Spacing();
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Welcome to SNAKE PRIVATE BYPASS");
                    ImGui::Separator();

                    ImGui::Text("SELECT YOUR EMULATOR :");
                    static int selected_emulator = 0;
                    ImGui::RadioButton("GAMPLOOP 7.1", &selected_emulator, 0); ImGui::SameLine();
                    ImGui::RadioButton("SMARTGAGA", &selected_emulator, 1);
                    ImGui::Spacing();

                    ImGui::Text("SELECT YOUR GAME VERSION :");
                    static int game_version = 0;
                    ImGui::RadioButton("Gl", &game_version, 0); ImGui::SameLine();
                    ImGui::RadioButton("Kr", &game_version, 1); ImGui::SameLine();
                    ImGui::RadioButton("Tw", &game_version, 2); ImGui::SameLine();
                    ImGui::RadioButton("Vn", &game_version, 3);
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();

                    float button_width = ImGui::GetContentRegionAvail().x;
                    ImGui::Button("BYPASS EMULATOR", ImVec2(button_width, 35));
                    ImGui::Button("SAFE EXIT", ImVec2(button_width, 35));
                    ImGui::Button("REST GUEST", ImVec2(button_width, 35));
                    ImGui::Spacing();
                    ImGui::Separator();

                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "BYPASS DONE SUCCESSFUL");
                    
                    ImGui::EndTabItem();
                }

                // -- MISC Tab --
                if (ImGui::BeginTabItem("MISC"))
                {
                    ImGui::Spacing();
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "SNAKE PRIVATE BYPASS");
                    ImGui::Separator();
                    ImGui::Text("Memory Hacks");
                    
                    static bool wide_view_cb = false;
                    static int wide_view_slider = 47;
                    ImGui::Checkbox("Wide View", &wide_view_cb); ImGui::SameLine(120);
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 10);
                    ImGui::SliderInt("##WideView", &wide_view_slider, 0, 100, "%d");

                    static bool no_recoil = false, no_tree = false, no_grass = false, small_crosshair = false;
                    ImGui::Checkbox("No Recoil", &no_recoil); ImGui::SameLine();
                    ImGui::Checkbox("No Tree", &no_tree); ImGui::SameLine();
                    ImGui::Checkbox("No Grass", &no_grass); ImGui::SameLine();
                    ImGui::Checkbox("Small Crosshair", &small_crosshair);

                    static bool night_mode = false, instahit = false, x_effect = false, luffy_hand = false;
                    ImGui::Checkbox("Night Mode", &night_mode); ImGui::SameLine();
                    ImGui::Checkbox("InstaHit", &instahit); ImGui::SameLine();
                    ImGui::Checkbox("X-Effect", &x_effect); ImGui::SameLine();
                    ImGui::Checkbox("LUFFY-HAND", &luffy_hand);

                    static bool zero_head = false;
                    ImGui::Checkbox("ZeroHead", &zero_head);

                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}