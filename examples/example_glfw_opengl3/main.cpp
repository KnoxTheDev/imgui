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

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(800, 600, "SNAKE BYPASS", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool show_misc_tab = false;
    int selected_emulator = 0; // 0 = Gameloop 7.1, 1 = SmartGaga
    int selected_game = 0; // 0 = Gl, 1 = Kr, 2 = Tw, 3 = Vn
    float wide_view = 47.0f;

    bool hacks[12] = { false }; // 0:NoRecoil, 1:NoTree, ..., 11:ZeroHead

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("SNAKE BYPASS", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

        if (ImGui::BeginTabBar("MainTabs"))
        {
            if (ImGui::BeginTabItem("BYPASS"))
            {
                ImGui::TextColored(ImVec4(0,1,0,1), "Welcome to SNAKE PRIVATE BYPASS");
                ImGui::Separator();
                ImGui::Text("SELECT YOUR EMULATOR:");
                ImGui::RadioButton("GAMLOOP 7.1", &selected_emulator, 0); ImGui::SameLine();
                ImGui::RadioButton("SMARTGAGA", &selected_emulator, 1);

                ImGui::Text("SELECT YOUR GAME VERSION:");
                ImGui::RadioButton("Gl", &selected_game, 0); ImGui::SameLine();
                ImGui::RadioButton("Kr", &selected_game, 1); ImGui::SameLine();
                ImGui::RadioButton("Tw", &selected_game, 2); ImGui::SameLine();
                ImGui::RadioButton("Vn", &selected_game, 3);

                ImGui::Spacing();
                ImGui::Spacing();
                if (ImGui::Button("BYPASS EMULATOR", ImVec2(-1, 0))) {}
                if (ImGui::Button("SAFE EXIT", ImVec2(-1, 0))) {}
                if (ImGui::Button("REST GUEST", ImVec2(-1, 0))) {}

                ImGui::TextColored(ImVec4(0,1,0,1), "BYPASS DONE SUCCESSFUL");

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("MISC"))
            {
                ImGui::TextColored(ImVec4(0,1,0,1), "SNAKE PRIVATE BYPASS");
                ImGui::Text("Memory Hacks");

                ImGui::SliderFloat("Wide View", &wide_view, 0.0f, 100.0f);

                ImGui::Checkbox("No Recoil", &hacks[0]); ImGui::SameLine();
                ImGui::Checkbox("No Tree", &hacks[1]); ImGui::SameLine();
                ImGui::Checkbox("No Grass", &hacks[2]); ImGui::SameLine();
                ImGui::Checkbox("Small Crosshair", &hacks[3]);

                ImGui::Checkbox("Night Mode", &hacks[4]); ImGui::SameLine();
                ImGui::Checkbox("InstaHit", &hacks[5]); ImGui::SameLine();
                ImGui::Checkbox("X-Effect", &hacks[6]); ImGui::SameLine();
                ImGui::Checkbox("LUFFY-HAND", &hacks[7]);

                ImGui::Checkbox("ZeroHead", &hacks[8]);

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End(); // SNAKE BYPASS

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
