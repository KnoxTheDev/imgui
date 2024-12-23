#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include <android/log.h>
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>
#include <fstream>

#include <jni.h>

// Global data
static EGLDisplay g_EglDisplay = EGL_NO_DISPLAY;
static EGLSurface g_EglSurface = EGL_NO_SURFACE;
static EGLContext g_EglContext = EGL_NO_CONTEXT;
static struct android_app* g_App = nullptr;
static bool g_Initialized = false;
static std::string g_IniFilename = "";
static const char* g_LogTag = "ThunderModExample";
static std::ofstream logFile;

// Helper function to log to a file
void LogToFile(const std::string& message)
{
    if (logFile.is_open())
    {
        logFile << message << std::endl;
    }
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

    // Open log file
    std::string logFilePath = std::string(app->activity->internalDataPath) + "/log.txt";
    logFile.open(logFilePath, std::ios::out | std::ios::app);
    if (logFile.is_open())
    {
        LogToFile("App Started");
    }

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
    ANativeWindow_acquire(g_App->window);

    // Initialize EGL
    g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(g_EglDisplay, 0, 0);
    const EGLint egl_attributes[] = { 
        EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, 
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE 
    };
    EGLConfig egl_config;
    EGLint num_configs = 0;
    eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs);
    g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_App->window, nullptr);
    const EGLint egl_context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);
    eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    g_IniFilename = std::string(app->activity->internalDataPath) + "/imgui.ini";
    io.IniFilename = g_IniFilename.c_str();

    ImGui::StyleColorsDark();
    ImGui::GetStyle().ScaleAllSizes(6.0f);

    ImGui_ImplAndroid_Init(g_App->window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    ImFontConfig font_cfg;
    font_cfg.SizePixels = 44.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    LogToFile("Initialization Complete");
    g_Initialized = true;
}

void MainLoopStep()
{
    if (!g_Initialized) return;

    ImGuiIO& io = ImGui::GetIO();

    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    // Thunder Mod menu
    static bool show_menu = true;
    if (show_menu)
    {
        ImGui::Begin("THUNDER MOD 32 BIT", &show_menu, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::CollapsingHeader("ESP"))
        {
            static bool esp_line = false, esp_box = false, esp_name = false;
            static bool esp_skeleton = false, esp_health = false, esp_teamid = false;
            static bool esp_distance = false, esp_vehicle = false, esp_nobot = false;

            ImGui::Checkbox("Line", &esp_line);
            ImGui::Checkbox("Box", &esp_box);
            ImGui::Checkbox("Name", &esp_name);
            ImGui::Checkbox("Skeleton", &esp_skeleton);
            ImGui::Checkbox("Health", &esp_health);
            ImGui::Checkbox("Team ID", &esp_teamid);
            ImGui::Checkbox("Distance", &esp_distance);
            ImGui::Checkbox("No Bot", &esp_nobot);
            ImGui::Checkbox("Vehicle", &esp_vehicle);
        }

        ImGui::Text("SETTINGS");
        ImGui::End();
    }

    // Rendering setup
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui::Render();

    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    eglSwapBuffers(g_EglDisplay, g_EglSurface);
}

void Shutdown()
{
    if (!g_Initialized) return;

    LogToFile("Shutting down");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();

    if (g_EglDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(g_EglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (g_EglContext != EGL_NO_CONTEXT) eglDestroyContext(g_EglDisplay, g_EglContext);
        if (g_EglSurface != EGL_NO_SURFACE) eglDestroySurface(g_EglDisplay, g_EglSurface);
        eglTerminate(g_EglDisplay);
    }

    ANativeWindow_release(g_App->window);
    g_Initialized = false;

    LogToFile("App Shutdown");
}
