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

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Snake Bypass", NULL, NULL);
    if (!window)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // State variables
    int current_emulator = 0; // 0=GamLoop, 1=SmartGaGa
    int current_version = 0;  // 0=Gl,1=Kr,2=Tw,3=Vn
    bool bypass_done = false;
    bool show_misc = false;
    float wide_view = 47.0f;
    bool no_recoil = false, no_tree = false, no_grass = false, small_cross = false;
    bool night_mode = false, instahit = false, x_effect = false, luffy_hand = false, zero_head = false;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main "Snake Bypass" window
        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("SNAKE BYPASS", NULL, ImGuiWindowFlags_NoCollapse))
        {
            if (ImGui::BeginTabBar("MainTabs"))
            {
                if (ImGui::BeginTabItem("BYPASS"))
                {
                    ImGui::TextColored(ImVec4(0,1,0,1), "Welcome to SNAKE PRIVATE BYPASS");
                    ImGui::Spacing();
                    ImGui::Text("Select Your Emulator :");
                    ImGui::RadioButton("GAMLOOP 7.1", &current_emulator, 0); ImGui::SameLine();
                    ImGui::RadioButton("SMARTGAGA", &current_emulator, 1);

                    ImGui::Spacing();
                    ImGui::Text("Select Your Game Version :");
                    ImGui::RadioButton("Gl", &current_version, 0); ImGui::SameLine();
                    ImGui::RadioButton("Kr", &current_version, 1); ImGui::SameLine();
                    ImGui::RadioButton("Tw", &current_version, 2); ImGui::SameLine();
                    ImGui::RadioButton("Vn", &current_version, 3);

                    ImGui::Spacing();
                    if (ImGui::Button("BYPASS EMULATOR", ImVec2(-1, 0)))
                    {
                        // TODO: trigger bypass logic
                        bypass_done = true;
                    }
                    if (ImGui::Button("SAFE EXIT", ImVec2(-1, 0)))
                    {
                        glfwSetWindowShouldClose(window, GLFW_TRUE);
                    }
                    if (ImGui::Button("REST GUEST", ImVec2(-1, 0)))
                    {
                        // TODO: reset guest logic
                        bypass_done = false;
                    }

                    if (bypass_done)
                    {
                        ImGui::TextColored(ImVec4(0,1,0,1), "BYPASS DONE SUCCESSFUL");
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("MISC"))
                {
                    ImGui::TextColored(ImVec4(0,1,0,1), "SNAKE PRIVATE BYPASS");
                    ImGui::Spacing();
                    ImGui::Text("Memory Hacks");
                    ImGui::Spacing();

                    ImGui::Checkbox("Wide View", &show_misc); ImGui::SameLine();
                    ImGui::SliderFloat("", &wide_view, 0.0f, 100.0f, "%.0f"); ImGui::SameLine();
                    ImGui::Text("%.0f", wide_view);

                    ImGui::Checkbox("No Recoil", &no_recoil); ImGui::SameLine();
                    ImGui::Checkbox("No Tree", &no_tree); ImGui::SameLine();
                    ImGui::Checkbox("No Grass", &no_grass); ImGui::SameLine();
                    ImGui::Checkbox("Small Crosshair", &small_cross);

                    ImGui::Checkbox("Night Mode", &night_mode); ImGui::SameLine();
                    ImGui::Checkbox("InstaHit", &instahit); ImGui::SameLine();
                    ImGui::Checkbox("X-Effect", &x_effect); ImGui::SameLine();
                    ImGui::Checkbox("LUFFY-HAND", &luffy_hand);
                    ImGui::Checkbox("ZeroHead", &zero_head);

                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
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
