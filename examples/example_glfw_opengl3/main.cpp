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

// Hide the console window on Windows
#if defined(_MSC_VER)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Function to set up the custom theme
void SetupCustomTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    
    ImVec4 color_bg = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    ImVec4 color_main = ImVec4(0.71f, 0.54f, 0.25f, 1.0f); // Orange/Brown
    ImVec4 color_dark_main = ImVec4(0.51f, 0.38f, 0.18f, 1.0f);
    ImVec4 color_hover = ImVec4(0.81f, 0.62f, 0.30f, 1.0f);
    ImVec4 color_text = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 color_frame_bg = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(8, 4);
    style.ItemSpacing = ImVec2(8, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.WindowRounding = 0.0f;
    style.FrameRounding = 2.0f;
    style.GrabRounding = 2.0f;
    style.TabRounding = 2.0f;
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;


    style.Colors[ImGuiCol_Text] = color_text;
    style.Colors[ImGuiCol_WindowBg] = color_bg;
    style.Colors[ImGuiCol_ChildBg] = color_bg;
    style.Colors[ImGuiCol_PopupBg] = color_bg;

    style.Colors[ImGuiCol_Border] = ImVec4(0,0,0,0);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0,0,0,0);

    style.Colors[ImGuiCol_FrameBg] = color_frame_bg;
    style.Colors[ImGuiCol_FrameBgHovered] = color_frame_bg;
    style.Colors[ImGuiCol_FrameBgActive] = color_frame_bg;

    style.Colors[ImGuiCol_TitleBg] = color_main;
    style.Colors[ImGuiCol_TitleBgActive] = color_main;
    style.Colors[ImGuiCol_TitleBgCollapsed] = color_main;

    style.Colors[ImGuiCol_CheckMark] = color_main;
    style.Colors[ImGuiCol_SliderGrab] = color_main;
    style.Colors[ImGuiCol_SliderGrabActive] = color_hover;

    style.Colors[ImGuiCol_Button] = color_main;
    style.Colors[ImGuiCol_ButtonHovered] = color_hover;
    style.Colors[ImGuiCol_ButtonActive] = color_dark_main;
    
    style.Colors[ImGuiCol_Header] = color_dark_main;
    style.Colors[ImGuiCol_HeaderHovered] = color_hover;
    style.Colors[ImGuiCol_HeaderActive] = color_main;
    
    style.Colors[ImGuiCol_Tab] = color_dark_main;
    style.Colors[ImGuiCol_TabHovered] = color_hover;
    style.Colors[ImGuiCol_TabActive] = color_main;
    style.Colors[ImGuiCol_TabUnfocused] = color_dark_main;
    style.Colors[ImGuiCol_TabUnfocusedActive] = color_main;

    style.Colors[ImGuiCol_Separator] = color_frame_bg;
}


int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

#if defined(__APPLE__)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
    
    GLFWwindow* window = glfwCreateWindow(580, 480, "SNAKE BYPASS", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    // Viewports flag removed to fix compilation on environments where it's not supported.

    SetupCustomTheme();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    // UI state variables
    static bool bypass_successful = false;
    static int selected_emulator = 0; // 0 for Gameloop, 1 for Smartgaga
    static int selected_version = 0; // 0:Gl, 1:Kr, 2:Tw, 3:Vn

    // Misc Tab State
    static bool wide_view_enabled = false;
    static int wide_view_value = 47;
    static bool no_recoil = false;
    static bool no_tree = false;
    static bool no_grass = false;
    static bool small_crosshair = false;
    static bool night_mode = false;
    static bool instahit = false;
    static bool x_effect = false;
    static bool luffy_hand = false;
    static bool zero_head = false;
    
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // --- Main Window ---
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("SnakeBypass", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        
        // Tabs
        if (ImGui::BeginTabBar("MainTabs")) {
            // BYPASS Tab
            if (ImGui::BeginTabItem("BYPASS")) {
                ImGui::Dummy(ImVec2(0.0f, 5.0f)); // Spacer
                ImGui::TextColored(ImVec4(0.1f, 0.9f, 0.2f, 1.0f), "Welcome to SNAKE PRIVATE BYPASS");
                ImGui::Separator();

                ImGui::Dummy(ImVec2(0.0f, 10.0f));
                ImGui::Text("SELECT YOUR EMULATOR :");
                ImGui::RadioButton("GAMELOOP 7.1", &selected_emulator, 0); ImGui::SameLine(250);
                ImGui::RadioButton("SMARTGAGA", &selected_emulator, 1);
                ImGui::Dummy(ImVec2(0.0f, 10.0f));

                ImGui::Text("SELECT YOUR GAME VERSION :");
                ImGui::RadioButton("Gl", &selected_version, 0); ImGui::SameLine(150);
                ImGui::RadioButton("Kr", &selected_version, 1); ImGui::SameLine(300);
                ImGui::RadioButton("Tw", &selected_version, 2); ImGui::SameLine(450);
                ImGui::RadioButton("Vn", &selected_version, 3);
                ImGui::Separator();

                ImGui::Dummy(ImVec2(0.0f, 20.0f));

                if (ImGui::Button("BYPASS EMULATOR", ImVec2(-1, 35))) {
                    bypass_successful = true;
                }
                ImGui::Dummy(ImVec2(0.0f, 5.0f));
                ImGui::Button("SAFE EXIT", ImVec2(-1, 35));
                ImGui::Dummy(ImVec2(0.0f, 5.0f));
                ImGui::Button("REST GUEST", ImVec2(-1, 35));

                ImGui::Dummy(ImVec2(0.0f, 25.0f));
                if(bypass_successful) {
                    ImGui::TextColored(ImVec4(0.1f, 0.9f, 0.2f, 1.0f), "BYPASS DONE SUCCESSFUL");
                }

                ImGui::EndTabItem();
            }

            // MISC Tab
            if (ImGui::BeginTabItem("MISC")) {
                ImGui::Dummy(ImVec2(0.0f, 5.0f));
                ImGui::TextColored(ImVec4(0.1f, 0.9f, 0.2f, 1.0f), "SNAKE PRIVATE BYPASS");
                ImGui::Separator();
                
                ImGui::Dummy(ImVec2(0.0f, 5.0f));
                ImGui::Text("Memory Hacks");
                ImGui::Dummy(ImVec2(0.0f, 5.0f));

                ImGui::Checkbox("Wide View", &wide_view_enabled); ImGui::SameLine();
                ImGui::PushItemWidth(200);
                ImGui::SliderInt("##wideview", &wide_view_value, 0, 100, "%d");
                ImGui::PopItemWidth();
                
                ImGui::Checkbox("No Recoil", &no_recoil); ImGui::SameLine(200);
                ImGui::Checkbox("No Tree", &no_tree); ImGui::SameLine(350);
                ImGui::Checkbox("No Grass", &no_grass);

                ImGui::Checkbox("Small Crosshair", &small_crosshair); ImGui::SameLine(200);
                ImGui::Checkbox("Night Mode", &night_mode); ImGui::SameLine(350);
                ImGui::Checkbox("InstaHit", &instahit);

                ImGui::Checkbox("X-Effect", &x_effect); ImGui::SameLine(200);
                ImGui::Checkbox("LUFFY-HAND", &luffy_hand); ImGui::SameLine(350);
                ImGui::Checkbox("ZeroHead", &zero_head);
                
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End(); // End SnakeBypass


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