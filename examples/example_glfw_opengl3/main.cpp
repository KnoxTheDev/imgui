#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#if defined(_MSC_VER)
#pragma comment(lib, "legacy_stdio_definitions")
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void ApplyYellowTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_WindowBg]           = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBg]            = ImVec4(0.95f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive]      = ImVec4(0.95f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_Tab]                = ImVec4(0.60f, 0.35f, 0.15f, 1.00f);
    colors[ImGuiCol_TabActive]          = ImVec4(0.95f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TabHovered]         = ImVec4(1.00f, 0.65f, 0.15f, 1.00f);
    colors[ImGuiCol_Button]             = ImVec4(0.60f, 0.35f, 0.15f, 1.00f);
    colors[ImGuiCol_ButtonHovered]      = ImVec4(0.95f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_ButtonActive]       = ImVec4(0.95f, 0.50f, 0.00f, 1.00f);
    colors[ImGuiCol_FrameBg]            = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_CheckMark]          = ImVec4(0.95f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_SliderGrab]         = ImVec4(0.95f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]   = ImVec4(1.00f, 0.70f, 0.10f, 1.00f);
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(602, 748, "SNAKE BYPASS", NULL, NULL);
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ApplyYellowTheme();

    int selected_emulator = 0;
    int selected_game = 0;
    float wide_view = 47.0f;
    bool hacks[12] = { false };
    enum Tab { TAB_BYPASS, TAB_MISC };
    Tab active_tab = TAB_BYPASS;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Fixed window size
        ImGui::SetNextWindowSize(ImVec2(602, 748));
        ImGui::Begin("SNAKE BYPASS", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

        // Header
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
        ImGui::Text("SNAKE BYPASS");
        ImGui::PopStyleColor();

        // Tabs
        if (ImGui::BeginTabBar("Tabs", ImGuiTabBarFlags_NoTooltip))
        {
            if (ImGui::TabItemButton("BYPASS")) active_tab = TAB_BYPASS;
            ImGui::SameLine();
            if (ImGui::TabItemButton("MISC")) active_tab = TAB_MISC;
            ImGui::EndTabBar();
        }

        ImGui::Spacing();

        if (active_tab == TAB_BYPASS)
        {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Welcome to SNAKE PRIVATE BYPASS");
            ImGui::Spacing();

            ImGui::Text("SELECT YOUR EMULATOR:");
            ImGui::RadioButton("GAMLOOP 7.1", &selected_emulator, 0); ImGui::SameLine();
            ImGui::RadioButton("SMARTGAGA", &selected_emulator, 1);

            ImGui::Spacing();
            ImGui::Text("SELECT YOUR GAME VERSION:");
            ImGui::RadioButton("Gl", &selected_game, 0); ImGui::SameLine();
            ImGui::RadioButton("Kr", &selected_game, 1); ImGui::SameLine();
            ImGui::RadioButton("Tw", &selected_game, 2); ImGui::SameLine();
            ImGui::RadioButton("Vn", &selected_game, 3);

            ImGui::Spacing(); ImGui::Spacing();
            if (ImGui::Button("BYPASS EMULATOR", ImVec2(-1, 0))) {}
            if (ImGui::Button("SAFE EXIT", ImVec2(-1, 0))) {}
            if (ImGui::Button("REST GUEST", ImVec2(-1, 0))) {}

            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "BYPASS DONE SUCCESSFUL");
        }

        if (active_tab == TAB_MISC)
        {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "SNAKE PRIVATE BYPASS");
            ImGui::Text("Memory Hacks");
            ImGui::Spacing();

            ImGui::Checkbox("Wide View", &hacks[0]); ImGui::SameLine();
            ImGui::SliderFloat("", &wide_view, 0.0f, 100.0f, "%.0f");

            ImGui::Checkbox("No Recoil", &hacks[1]); ImGui::SameLine();
            ImGui::Checkbox("No Tree", &hacks[2]); ImGui::SameLine();
            ImGui::Checkbox("No Grass", &hacks[3]); ImGui::SameLine();
            ImGui::Checkbox("Small Crosshair", &hacks[4]);

            ImGui::Checkbox("Night Mode", &hacks[5]); ImGui::SameLine();
            ImGui::Checkbox("InstaHit", &hacks[6]); ImGui::SameLine();
            ImGui::Checkbox("X-Effect", &hacks[7]); ImGui::SameLine();
            ImGui::Checkbox("LUFFY-HAND", &hacks[8]);

            ImGui::Checkbox("ZeroHead", &hacks[9]);
        }

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.09f, 0.09f, 0.09f, 1.0f);
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
