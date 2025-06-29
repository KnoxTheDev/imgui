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

#if defined(_MSC_VER)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static void ApplySnakeTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.95f); // VS Code black high contrast

    ImVec4 color1 = ImVec4(232.0f/255.0f, 163.0f/255.0f, 33.0f/255.0f, 1.00f); // #e8a321
    ImVec4 color2 = ImVec4(130.0f/255.0f, 92.0f/255.0f, 38.0f/255.0f, 1.00f);  // #825c26
    ImVec4 tab_focused   = ImVec4(199.0f/255.0f, 140.0f/255.0f, 54.0f/255.0f, 1.00f); // #c78c36
    ImVec4 tab_unfocused = color2;

    style.Colors[ImGuiCol_Header]        = color2;
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(140.0f/255.0f, 100.0f/255.0f, 45.0f/255.0f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]  = ImVec4(150.0f/255.0f, 110.0f/255.0f, 50.0f/255.0f, 1.00f);

    style.Colors[ImGuiCol_Tab]                 = tab_unfocused;
    style.Colors[ImGuiCol_TabUnfocused]        = tab_unfocused;
    style.Colors[ImGuiCol_TabUnfocusedActive]  = tab_unfocused;
    style.Colors[ImGuiCol_TabActive]           = tab_focused;
    style.Colors[ImGuiCol_TabHovered]          = tab_focused;

    style.Colors[ImGuiCol_TitleBg]          = color1;
    style.Colors[ImGuiCol_TitleBgActive]    = color1;
    style.Colors[ImGuiCol_TitleBgCollapsed] = color1;

    style.Colors[ImGuiCol_Button]        = color2;
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(140.0f/255.0f, 100.0f/255.0f, 45.0f/255.0f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]  = ImVec4(150.0f/255.0f, 110.0f/255.0f, 50.0f/255.0f, 1.00f);

    style.Colors[ImGuiCol_Text]           = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]   = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.80f, 0.00f, 0.50f);

    style.Colors[ImGuiCol_FrameBg]        = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive]  = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

    style.Colors[ImGuiCol_Separator]      = ImVec4(0.45f, 0.40f, 0.40f, 0.50f);

    style.Colors[ImGuiCol_CheckMark] = tab_focused;
    style.Colors[ImGuiCol_SliderGrab]        = color1;
    style.Colors[ImGuiCol_SliderGrabActive]  = color1;

    style.WindowBorderSize = 0.0f;    // No border around main window
    style.FrameBorderSize  = 0.0f;    // No border around widgets
    style.TabBorderSize    = 0.0f;    // No tab border
    style.GrabMinSize      = 10.0f;   // Optional: smaller grab handle

    // Hide resizer corner
    style.Colors[ImGuiCol_ResizeGrip]         = ImVec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_ResizeGripHovered]  = ImVec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_ResizeGripActive]   = ImVec4(0, 0, 0, 0);

    // Optional (removes border visuals on sliders, checkboxes, buttons etc.)
    style.Colors[ImGuiCol_Border]        = ImVec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_BorderShadow]  = ImVec4(0, 0, 0, 0);

    style.FrameRounding      = 3.0f;
    style.GrabRounding       = 3.0f;
    style.TabRounding        = 2.0f;
    style.WindowBorderSize   = 1.0f;
    style.FrameBorderSize    = 1.0f;
    style.TabBorderSize      = 1.0f;
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    int res_w = 800, res_h = 640;
    GLFWwindow* window = glfwCreateWindow(res_w, res_h, "Snake Bypass", NULL, NULL);
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImFont* rudaFont = io.Fonts->AddFontFromFileTTF("Ruda-Bold.ttf", 18.0f);
    if (!rudaFont)
       fprintf(stderr, "Failed to load font: Ruda-Bold.ttf\n");
    else
    io.FontDefault = rudaFont;

    ImGui::StyleColorsDark();
    ApplySnakeTheme();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    const ImVec2 gui_size = ImVec2(445, 432);
    const ImVec2 gui_pos  = ImVec2((res_w - gui_size.x) * 0.5f,
                                  (res_h - gui_size.y) * 0.5f);

    int emu = 0, ver = 0;
    bool done = false;
    bool wide_enabled = false;
    float wide = 47.0f;
    bool r_no = false, t_no = false, g_no = false, cross_small = false;
    bool night = false, insta = false, xfx = false, luffy = false, zero = false;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(gui_size, ImGuiCond_Once);
        ImGui::SetNextWindowPos(gui_pos, ImGuiCond_Once);
        static bool show_main_window = true;
        if (ImGui::Begin("SNAKE BYPASS", &show_main_window, ImGuiWindowFlags_NoResize))
        {
            if (ImGui::BeginTabBar("Tabs##Snake"))
            {
                if (ImGui::BeginTabItem("BYPASS"))
                {
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::TextColored(ImVec4(0,1,0,1), "Welcome to SNAKE PRIVATE BYPASS");
                    ImGui::Separator();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Text("SELECT YOUR EMULATOR :");
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::RadioButton("GAMELOOP 7.1##emu", &emu, 0); ImGui::SameLine();
                    ImGui::RadioButton("SMARTGAGA##emu", &emu, 1);
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Text("SELECT YOUR GAME VERSION :");
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();                    
                    ImGui::RadioButton("Global##ver", &ver, 0); ImGui::SameLine();
                    ImGui::RadioButton("Korea##ver", &ver, 1); ImGui::SameLine();
                    ImGui::RadioButton("Taiwan##ver", &ver, 2); ImGui::SameLine();
                    ImGui::RadioButton("Vietnam##ver", &ver, 3);
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    if (ImGui::Button("BYPASS EMULATOR", ImVec2(-1,0))) { done = true; }
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    if (ImGui::Button("SAFE EXIT", ImVec2(-1,0)))     { glfwSetWindowShouldClose(window, GLFW_TRUE); }
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing(); 
                    if (ImGui::Button("REST GUEST", ImVec2(-1,0)))    { done = false; }

                    if (done)
                    {
                        ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::TextColored(ImVec4(0,1,0,1), "BYPASS DONE SUCCESSFUL");
                        ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::Spacing();
                    }
                    
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("MISC"))
                {
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();                    
                    ImGui::TextColored(ImVec4(0,1,0,1), "SNAKE PRIVATE BYPASS");
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Text("Memory Hacks");
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Checkbox("Wide View##widechk", &wide_enabled);
                    ImGui::SameLine();
                    ImGui::SliderFloat("##wide", &wide, 0.0f, 100.0f, "%.0f");

                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();

                    ImGui::Checkbox("No Recoil##no", &r_no); ImGui::SameLine();
                    ImGui::Checkbox("No Tree##no", &t_no);  ImGui::SameLine();
                    ImGui::Checkbox("No Grass##no", &g_no); ImGui::SameLine();
                    ImGui::Checkbox("Small Crosshair##cross", &cross_small);

                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();

                    ImGui::Checkbox("Night Mode##nm", &night);   ImGui::SameLine();
                    ImGui::Checkbox("InstaHit##ih", &insta);    ImGui::SameLine();
                    ImGui::Checkbox("X-Effect##xf", &xfx);      ImGui::SameLine();
                    ImGui::Checkbox("LUFFY-HAND##lf", &luffy);
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Checkbox("ZeroHead##zh", &zero);
                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Spacing();

                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::End();
        if (!show_main_window)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        ImGui::Render();
        int w, h; glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.10f, 0.10f, 0.10f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
