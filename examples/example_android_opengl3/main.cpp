#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include <android/log.h>
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

// Global data
static EGLDisplay g_EglDisplay = EGL_NO_DISPLAY;
static EGLSurface g_EglSurface = EGL_NO_SURFACE;
static EGLContext g_EglContext = EGL_NO_CONTEXT;
static struct android_app* g_App = nullptr;
static bool g_Initialized = false;
static AAssetManager* g_AssetManager = nullptr;

// Forward declarations
void Init(struct android_app* app);
void Shutdown();
void MainLoopStep();

// Helper function to log messages
void LogMessage(const char* message)
{
    __android_log_print(ANDROID_LOG_INFO, "ThunderMod", "%s", message);
}

// Event handlers
static void HandleAppCmd(struct android_app* app, int32_t appCmd)
{
    switch (appCmd)
    {
    case APP_CMD_INIT_WINDOW:
        Init(app); // Now this will work as Init is forward declared
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

// Initialize the Android app
void android_main(struct android_app* app)
{
    app->onAppCmd = HandleAppCmd;
    app->onInputEvent = HandleInputEvent;
    g_AssetManager = app->activity->assetManager; // Get the AssetManager

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

// Initialize EGL, ImGui, and custom font
void Init(struct android_app* app)
{
    if (g_Initialized) return;

    g_App = app;

    // Initialize EGL
    g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (g_EglDisplay == EGL_NO_DISPLAY) {
        LogMessage("Error: eglGetDisplay failed.");
        return;
    }
    
    if (eglInitialize(g_EglDisplay, 0, 0) == EGL_FALSE) {
        LogMessage("Error: eglInitialize failed.");
        return;
    }

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
    if (eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs) == EGL_FALSE || num_configs == 0) {
        LogMessage("Error: eglChooseConfig failed.");
        return;
    }

    g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_App->window, nullptr);
    if (g_EglSurface == EGL_NO_SURFACE) {
        LogMessage("Error: eglCreateWindowSurface failed.");
        return;
    }

    const EGLint egl_context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);
    if (g_EglContext == EGL_NO_CONTEXT) {
        LogMessage("Error: eglCreateContext failed.");
        return;
    }

    if (eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext) == EGL_FALSE) {
        LogMessage("Error: eglMakeCurrent failed.");
        return;
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplAndroid_Init(g_App->window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    // Load Roboto Regular font from assets/fonts folder
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear(); // Clear the default font
    AAsset* asset = AAssetManager_open(g_AssetManager, "fonts/Roboto-Regular.ttf", AASSET_MODE_BUFFER);
    
    if (asset != nullptr)
    {
        // Get the font data from the asset
        off_t assetLength = AAsset_getLength(asset);
        void* fontData = AAsset_getBuffer(asset);

        // Load the font from memory buffer
        io.Fonts->AddFontFromMemoryTTF(fontData, assetLength, 16.0f); // Set font size (e.g., 16.0f)

        // Optionally set this as the default font
        io.FontDefault = io.Fonts->AddFontFromMemoryTTF(fontData, assetLength, 16.0f);

        AAsset_close(asset);
    }
    else
    {
        LogMessage("Error: Could not open Roboto-Regular.ttf asset.");
    }

    g_Initialized = true;
}

// Main loop
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

    // Window configuration
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver); // Set initial size
    ImGui::SetNextWindowSizeConstraints(ImVec2(800, 600), ImVec2(1600, 1200)); // Set min and max window sizes

    // Begin main window
    ImGui::Begin("Thunder Mod 32 Bit", nullptr); // Main window without close button

    // Tab bar for GUI sections
    if (ImGui::BeginTabBar("MenuTabs"))
    {
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

// Shutdown and cleanup
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
