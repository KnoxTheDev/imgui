#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>

// Data
static EGLDisplay           g_EglDisplay = EGL_NO_DISPLAY;
static EGLSurface           g_EglSurface = EGL_NO_SURFACE;
static EGLContext           g_EglContext = EGL_NO_CONTEXT;
static struct android_app*  g_App = nullptr;
static bool                 g_Initialized = false;
static char                 g_LogTag[] = "KnoxHAX";  // Rebranded log tag
static std::string          g_IniFilename = "";

// Forward declarations of helper functions
static void Init(struct android_app* app);
static void Shutdown();
static void MainLoopStep();
static int ShowSoftKeyboardInput();
static int PollUnicodeChars();
static int GetAssetData(const char* filename, void** out_data);
// Forward declaration of the font loading function
ImFont* LoadFontFromAsset(const char* fontPath, float size);
// Declare the global font variable (only one font now)
ImFont* font = nullptr;

// New variables for controlling the size of ImGui in pixels
int g_ImguiWidth = 800;  // Width in pixels
int g_ImguiHeight = 600; // Height in pixels

// Main code
static void handleAppCmd(struct android_app* app, int32_t appCmd)
{
    switch (appCmd)
    {
    case APP_CMD_SAVE_STATE:
        break;
    case APP_CMD_INIT_WINDOW:
        Init(app);
        break;
    case APP_CMD_TERM_WINDOW:
        Shutdown();
        break;
    case APP_CMD_GAINED_FOCUS:
    case APP_CMD_LOST_FOCUS:
        break;
    }
}

static int32_t handleInputEvent(struct android_app* app, AInputEvent* inputEvent)
{
    return ImGui_ImplAndroid_HandleInputEvent(inputEvent);
}

void android_main(struct android_app* app)
{
    app->onAppCmd = handleAppCmd;
    app->onInputEvent = handleInputEvent;

    while (true)
    {
        int out_events;
        struct android_poll_source* out_data;

        // Poll all events. If the app is not visible, this loop blocks until g_Initialized == true.
        while (ALooper_pollOnce(g_Initialized ? 0 : -1, nullptr, &out_events, (void**)&out_data) >= 0)
        {
            // Process one event
            if (out_data != nullptr)
                out_data->process(app, out_data);

            // Exit the app by returning from within the infinite loop
            if (app->destroyRequested != 0)
            {
                // shutdown() should have been called already while processing the
                // app command APP_CMD_TERM_WINDOW. But we play safe here
                if (!g_Initialized)
                    Shutdown();

                return;
            }
        }

        // Initiate a new frame
        MainLoopStep();
    }
}

// Function to load font from assets
ImFont* LoadFontFromAsset(const char* fontPath, float size)
{
    ImGuiIO& io = ImGui::GetIO();
    void* fontData = nullptr;
    int fontDataSize = GetAssetData(fontPath, &fontData);  // Get the asset data from the asset folder

    if (fontDataSize == 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "Font file not found: %s", fontPath);
        return nullptr;
    }

    // Load the font from the asset data
    ImFont* font = io.Fonts->AddFontFromMemoryTTF(fontData, fontDataSize, size);
    IM_FREE(fontData);  // Free the loaded font data
    return font;
}

void Init(struct android_app* app)
{
    if (g_Initialized)
        return;

    g_App = app;
    ANativeWindow_acquire(g_App->window);

    // Initialize EGL (same as before)
    {
        g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (g_EglDisplay == EGL_NO_DISPLAY)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglGetDisplay(EGL_DEFAULT_DISPLAY) returned EGL_NO_DISPLAY");

        if (eglInitialize(g_EglDisplay, 0, 0) != EGL_TRUE)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglInitialize() returned with an error");

        const EGLint egl_attributes[] = { EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE };
        EGLint num_configs = 0;
        if (eglChooseConfig(g_EglDisplay, egl_attributes, nullptr, 0, &num_configs) != EGL_TRUE)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglChooseConfig() returned with an error");
        if (num_configs == 0)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglChooseConfig() returned 0 matching config");

        // Get the first matching config
        EGLConfig egl_config;
        eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs);
        EGLint egl_format;
        eglGetConfigAttrib(g_EglDisplay, egl_config, EGL_NATIVE_VISUAL_ID, &egl_format);
        ANativeWindow_setBuffersGeometry(g_App->window, 0, 0, egl_format);

        const EGLint egl_context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
        g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);

        if (g_EglContext == EGL_NO_CONTEXT)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglCreateContext() returned EGL_NO_CONTEXT");

        g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_App->window, nullptr);
        eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Redirect loading/saving of .ini file to our location.
    g_IniFilename = std::string(app->activity->internalDataPath) + "/knoxhax.ini";  // Changed filename
    io.IniFilename = g_IniFilename.c_str();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init(g_App->window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    // Load Arial font only
    const char* fontPath = "fonts/Arial.ttf";  // Path to Arial font
    font = LoadFontFromAsset(fontPath, 22.0f);

    // Set Arial as default font
    if (font) {
        io.FontDefault = font;
    }

    // Arbitrary scaling calculation based on screen resolution
    const ImVec2 screenSize = io.DisplaySize;

    // Default scaling factor for most devices
    float scaleFactor = 3.5f;
    float scaleFactorText = 3.0f;

    // Apply scale factor to all UI elements
    ImGui::GetStyle().ScaleAllSizes(scaleFactor);

    // Apply scaling for font only
    io.FontGlobalScale = scaleFactorText;

    g_Initialized = true;
}

void MainLoopStep()
{
    ImGuiIO& io = ImGui::GetIO();
    if (g_EglDisplay == EGL_NO_DISPLAY)
        return;

    // Define clear color (you can modify this to any RGBA value you want)
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);  // Dark grayish-blue color

    // Poll Unicode characters via JNI
    PollUnicodeChars();

    // Open on-screen (soft) input if requested by Dear ImGui
    static bool WantTextInputLast = false;
    if (io.WantTextInput && !WantTextInputLast)
        ShowSoftKeyboardInput();
    WantTextInputLast = io.WantTextInput;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    // Calculate FPS and update title every 1 second
    static float timeElapsed = 0.0f;
    static int lastFPS = 0;

    timeElapsed += io.DeltaTime;  // Accumulate time since the last frame
    if (timeElapsed >= 1.0f)  // Update FPS every 1 second
    {
        timeElapsed = 0.0f;  // Reset the time accumulator
        lastFPS = static_cast<int>(io.Framerate);  // Update FPS to the nearest integer
    }

    // Create window with updated title showing FPS
    std::string title = "Knox HAX | " + std::to_string(lastFPS) + " FPS";  // Create title with FPS

    // Allow window to be movable and resizable (without overriding position/size)
    // No need to call SetNextWindowPos or SetNextWindowSize here!
    ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_None);  // No flag to disable movement or resizing

    // ImGui window contents
    ImGui::Text("Hello from ImGui in Android + OpenGL ES 3!"); // Just display a simple greeting
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    ImGui::End();  // End the window

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    eglSwapBuffers(g_EglDisplay, g_EglSurface);
}

void Shutdown()
{
    if (!g_Initialized)
        return;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();

    if (g_EglDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(g_EglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (g_EglContext != EGL_NO_CONTEXT)
            eglDestroyContext(g_EglDisplay, g_EglContext);

        if (g_EglSurface != EGL_NO_SURFACE)
            eglDestroySurface(g_EglDisplay, g_EglSurface);

        eglTerminate(g_EglDisplay);
    }

    g_EglDisplay = EGL_NO_DISPLAY;
    g_EglContext = EGL_NO_CONTEXT;
    g_EglSurface = EGL_NO_SURFACE;
    ANativeWindow_release(g_App->window);

    g_Initialized = false;
}

// Helper functions
static int ShowSoftKeyboardInput()
{
    JavaVM* java_vm = g_App->activity->vm;
    JNIEnv* java_env = nullptr;

    jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
    if (jni_return != JNI_OK)
        jni_return = java_vm->AttachCurrentThread(&java_env, nullptr);

    jclass j_cls = java_env->GetObjectClass(g_App->activity->clazz);
    jmethodID j_show_keyboard = java_env->GetMethodID(j_cls, "showSoftKeyboard", "()V");
    java_env->CallVoidMethod(g_App->activity->clazz, j_show_keyboard);

    if (jni_return == JNI_OK)
        java_vm->DetachCurrentThread();

    return 0;
}

static int PollUnicodeChars()
{
    return 0;
}

static int GetAssetData(const char* filename, void** out_data)
{
    AAsset* asset = AAssetManager_open(g_App->activity->assetManager, filename, AASSET_MODE_BUFFER);
    if (asset == nullptr)
        return 0;

    off_t asset_length = AAsset_getLength(asset);
    *out_data = malloc(asset_length);

    AAsset_read(asset, *out_data, asset_length);
    AAsset_close(asset);

    return asset_length;
}
