#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include <android/log.h>
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>

// Global data
static EGLDisplay g_EglDisplay = EGL_NO_DISPLAY;
static EGLSurface g_EglSurface = EGL_NO_SURFACE;
static EGLContext g_EglContext = EGL_NO_CONTEXT;
static struct android_app* g_App = nullptr;
static bool g_Initialized = false;

// Helper function to log messages
void LogMessage(const char* message)
{
    __android_log_print(ANDROID_LOG_INFO, "ThunderMod", "%s", message);
}

// Forward declarations
static void Init(struct android_app* app);
static void Shutdown();
static void MainLoopStep();
static void HandleAppCmd(struct android_app* app, int32_t appCmd);
static int32_t HandleInputEvent(struct android_app* app, AInputEvent* inputEvent);

// Event handlers
static void HandleAppCmd(struct android_app* app, int32_t appCmd)
{
    switch (appCmd)
    {
    case APP_CMD_INIT_WINDOW:
        Init(app);
        break;
    case APP_CMD_TERM_WINDOW:
        Shutdown();
        break;
    }
}

static int32_t HandleInputEvent(struct android_app* app, AInputEvent* inputEvent)
{
    return ImGui_ImplAndroid_HandleInputEvent(inputEvent);
}

void android_main(struct android_app* app)
{
    app->onAppCmd = HandleAppCmd;
    app->onInputEvent = HandleInputEvent;

    while (true)
    {
        int out_events;
        struct android_poll_source* out_data;

        while (ALooper_pollOnce(g_Initialized ? 0 : -1, nullptr, &out_events, (void**)&out_data) >= 0)
        {
            if (out_data != nullptr)
                out_data->process(app, out_data);

            if (app->destroyRequested != 0)
            {
                if (g_Initialized)
                    Shutdown();
                return;
            }
        }

        MainLoopStep();
    }
}

void Init(struct android_app* app)
{
    if (g_Initialized) return;

    g_App = app;

    // Initialize EGL
    g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(g_EglDisplay, 0, 0);

    const EGLint egl_attributes[] = {
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    EGLConfig egl_config;
    EGLint num_configs;
    eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs);

    g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_App->window, nullptr);

    const EGLint egl_context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);

    eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplAndroid_Init(g_App->window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    g_Initialized = true;
}

void MainLoopStep()
{
    if (!g_Initialized) return;

    ImGuiIO& io = ImGui::GetIO();

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    // GUI State Variables
    static bool esp_box = false, esp_skeleton = false, esp_distance = false, esp_line = false, esp_name = false;
    static bool items_banana = false, items_apple = false, items_orange = false, items_grape = false, items_peach = false;
    static bool aimbot_fake1 = false, aimbot_fake2 = false, aimbot_fake3 = false, aimbot_fake4 = false, aimbot_fake5 = false;
    static int scale_level = 1; // Default GUI scale level
    const char* scale_labels[] = { "1", "2", "3" }; // Scale display labels
    static float scales[] = { 6.0f, 7.5f, 8.0f }; // Scale factors

    // Window configuration
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver); // Set initial size
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 300), ImVec2(800, 600)); // Set min and max window sizes

    // Begin main window
    ImGui::Begin("Thunder Mod 32 Bit", nullptr); // Main window without close button

    // Tab bar for GUI sections
    if (ImGui::BeginTabBar("MenuTabs"))
    {
        // Scale Tab
        if (ImGui::BeginTabItem("Scale"))
        {
            ImGui::Text("Adjust GUI Scale:");

            // Slider for scale levels
            if (ImGui::SliderInt("Scale", &scale_level, 1, 3, scale_labels[scale_level - 1]))
            {
                io.FontGlobalScale = scales[scale_level - 1]; // Apply the selected scale
            }

            // Display current scale for reference
            ImGui::Text("Current Scale: %.1f", io.FontGlobalScale);
            ImGui::EndTabItem();
        }

        // ESP Tab
        if (ImGui::BeginTabItem("ESP"))
        {
            ImGui::Checkbox("Box", &esp_box);
            ImGui::Checkbox("Skeleton", &esp_skeleton);
            ImGui::Checkbox("Distance", &esp_distance);
            ImGui::Checkbox("Line", &esp_line);
            ImGui::Checkbox("Name", &esp_name);
            ImGui::EndTabItem();
        }

        // Items Tab
        if (ImGui::BeginTabItem("Items"))
        {
            ImGui::Checkbox("Banana", &items_banana);
            ImGui::Checkbox("Apple", &items_apple);
            ImGui::Checkbox("Orange", &items_orange);
            ImGui::Checkbox("Grape", &items_grape);
            ImGui::Checkbox("Peach", &items_peach);
            ImGui::EndTabItem();
        }

        // Aimbot Tab
        if (ImGui::BeginTabItem("Aimbot"))
        {
            ImGui::Checkbox("Fake Aimbot 1", &aimbot_fake1);
            ImGui::Checkbox("Fake Aimbot 2", &aimbot_fake2);
            ImGui::Checkbox("Fake Aimbot 3", &aimbot_fake3);
            ImGui::Checkbox("Fake Aimbot 4", &aimbot_fake4);
            ImGui::Checkbox("Fake Aimbot 5", &aimbot_fake5);
            ImGui::EndTabItem();
        }
    }
    ImGui::EndTabBar();

    ImGui::End();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    eglSwapBuffers(g_EglDisplay, g_EglSurface);
}

void Shutdown()
{
    if (!g_Initialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();

    eglMakeCurrent(g_EglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(g_EglDisplay, g_EglContext);
    eglDestroySurface(g_EglDisplay, g_EglSurface);
    eglTerminate(g_EglDisplay);

    g_Initialized = false;
}
