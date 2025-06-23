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

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Apply the custom "Snake Bypass" theme
static void ApplySnakeTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    // Overall window background
    style.Colors[ImGuiCol_WindowBg]            = ImVec4(0.12f, 0.10f, 0.08f, 0.95f);
    // Tabs
    style.Colors[ImGuiCol_Tab]                 = ImVec4(0.45f, 0.33f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TabHovered]          = ImVec4(0.55f, 0.40f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_TabActive]           = ImVec4(0.65f, 0.50f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused]        = style.Colors[ImGuiCol_Tab];
    style.Colors[ImGuiCol_TabUnfocusedActive]  = style.Colors[ImGuiCol_TabActive];
    // Buttons
    style.Colors[ImGuiCol_Button]              = ImVec4(0.45f, 0.33f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]       = ImVec4(0.55f, 0.40f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]        = ImVec4(0.65f, 0.50f, 0.20f, 1.00f);
    // Text
    style.Colors[ImGuiCol_Text]                = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
    // Green accent text
    style.Colors[ImGuiCol_TextSelectedBg]      = ImVec4(0.00f, 0.80f, 0.00f, 0.35f);
}

int main(int, char**)
{
    // Setup GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create GLFW window (800×640)
    int res_w = 800, res_h = 640;
    GLFWwindow* window = glfwCreateWindow(res_w, res_h, "Snake Bypass", NULL, NULL);
    if (!window)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Apply custom style
    ImGui::StyleColorsDark();
    ApplySnakeTheme();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Desired ImGui window size from reference (380×320)
    const ImVec2 gui_size = ImVec2(380, 320);
    const ImVec2 gui_pos  = ImVec2((res_w - gui_size.x) * 0.5f,
                                  (res_h - gui_size.y) * 0.5f);

    // State variables (unique IDs via labels)
    int emu = 0, ver = 0;
    bool done = false;
    float wide = 47.0f;
    bool r_no = false, t_no = false, g_no = false, cross_small = false;
    bool night = false, insta = false, xfx = false, luffy = false, zero = false;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(gui_size, ImGuiCond_Once);
        ImGui::SetNextWindowPos(gui_pos, ImGuiCond_Once);
        if (ImGui::Begin("SNAKE BYPASS", NULL, ImGuiWindowFlags_NoCollapse))
        {
            if (ImGui::BeginTabBar("Tabs##Snake"))
            {
                if (ImGui::BeginTabItem("BYPASS"))
                {
                    ImGui::TextColored(ImVec4(0,1,0,1), "Welcome to SNAKE PRIVATE BYPASS");
                    ImGui::Spacing();

                    ImGui::Text("SELECT YOUR EMULATOR :");
                    ImGui::RadioButton("GAMLOOP 7.1##emu", &emu, 0); ImGui::SameLine();
                    ImGui::RadioButton("SMARTGAGA##emu", &emu, 1);

                    ImGui::Spacing();
                    ImGui::Text("SELECT YOUR GAME VERSION :");
                    ImGui::RadioButton("Gl##ver", &ver, 0); ImGui::SameLine();
                    ImGui::RadioButton("Kr##ver", &ver, 1); ImGui::SameLine();
                    ImGui::RadioButton("Tw##ver", &ver, 2); ImGui::SameLine();
                    ImGui::RadioButton("Vn##ver", &ver, 3);

                    ImGui::Spacing();
                    if (ImGui::Button("BYPASS EMULATOR")) { done = true; }
                    if (ImGui::Button("SAFE EXIT"))     { glfwSetWindowShouldClose(window, GLFW_TRUE); }
                    if (ImGui::Button("REST GUEST"))    { done = false; }

                    if (done)
                        ImGui::TextColored(ImVec4(0,1,0,1), "BYPASS DONE SUCCESSFUL");

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("MISC"))
                {
                    ImGui::TextColored(ImVec4(0,1,0,1), "SNAKE PRIVATE BYPASS");
                    ImGui::Spacing();
                    ImGui::Text("Memory Hacks");
                    ImGui::Spacing();

                    ImGui::SliderFloat("Wide View##wide", &wide, 0.0f, 100.0f, "%.0f");
                    ImGui::Spacing();

                    ImGui::Checkbox("No Recoil##no", &r_no); ImGui::SameLine();
                    ImGui::Checkbox("No Tree##no", &t_no);  ImGui::SameLine();
                    ImGui::Checkbox("No Grass##no", &g_no); ImGui::SameLine();
                    ImGui::Checkbox("Small Crosshair##cross", &cross_small);

                    ImGui::Spacing();
                    ImGui::Checkbox("Night Mode##nm", &night);   ImGui::SameLine();
                    ImGui::Checkbox("InstaHit##ih", &insta);    ImGui::SameLine();
                    ImGui::Checkbox("X-Effect##xf", &xfx);      ImGui::SameLine();
                    ImGui::Checkbox("LUFFY-HAND##lf", &luffy);
                    ImGui::Checkbox("ZeroHead##zh", &zero);

                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::End();

        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
