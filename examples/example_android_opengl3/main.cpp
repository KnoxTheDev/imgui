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
static EGLDisplay g_EglDisplay = EGL_NO_DISPLAY;
static EGLSurface g_EglSurface = EGL_NO_SURFACE;
static EGLContext g_EglContext = EGL_NO_CONTEXT;
static struct android_app* g_App = nullptr;
static bool g_Initialized = false;
static std::string g_IniFilename = "";  // Declare g_IniFilename globally
static const char* g_LogTag = "ImGuiExample";

// Forward declarations
static void Init(struct android_app* app);
static void Shutdown();
static void MainLoopStep();
static int ShowSoftKeyboardInput();
static int PollUnicodeChars();

// Event handlers
static void handleAppCmd(struct android_app* app, int32_t appCmd)
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

        while (ALooper_pollOnce(g_Initialized ? 0 : -1, nullptr, &out_events, (void**)&out_data) >= 0)
        {
            if (out_data != nullptr)
                out_data->process(app, out_data);

            if (app->destroyRequested != 0)
            {
                if (!g_Initialized)
                    Shutdown();
                return;
            }
        }

        MainLoopStep();
    }
}

void Init(struct android_app* app)
{
    if (g_Initialized)
        return;

    g_App = app;
    ANativeWindow_acquire(g_App->window);

    // Initialize EGL
    g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (g_EglDisplay == EGL_NO_DISPLAY)
        __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglGetDisplay(EGL_DEFAULT_DISPLAY) returned EGL_NO_DISPLAY");

    if (eglInitialize(g_EglDisplay, 0, 0) != EGL_TRUE)
        __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglInitialize() returned with an error");

    const EGLint egl_attributes[] = { 
        EGL_BLUE_SIZE, 8, 
        EGL_GREEN_SIZE, 8, 
        EGL_RED_SIZE, 8, 
        EGL_DEPTH_SIZE, 24, 
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT, 
        EGL_NONE 
    };
    
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

    const EGLint egl_context_attributes[] = { 
        EGL_CONTEXT_CLIENT_VERSION, 3, 
        EGL_NONE 
    };
    g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);

    if (g_EglContext == EGL_NO_CONTEXT)
        __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglCreateContext() returned EGL_NO_CONTEXT");

    g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_App->window, nullptr);
    eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Set the ini filename
    g_IniFilename = std::string(app->activity->internalDataPath) + "/imgui.ini";
    io.IniFilename = g_IniFilename.c_str();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init(g_App->window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    // Setup default font with size of 22 pixels
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    // Scale all ImGui styles by a factor of 3
    ImGui::GetStyle().ScaleAllSizes(3.0f);

    g_Initialized = true;
}

bool InitializeEGL()
{
    g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (g_EglDisplay == EGL_NO_DISPLAY)
        return false;

    if (eglInitialize(g_EglDisplay, 0, 0) != EGL_TRUE)
        return false;

    const EGLint egl_attributes[] = { EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE };
    EGLint num_configs = 0;
    if (eglChooseConfig(g_EglDisplay, egl_attributes, nullptr, 0, &num_configs) != EGL_TRUE || num_configs == 0)
        return false;

    EGLConfig egl_config;
    eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs);

    g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_App->window, nullptr);
    if (g_EglSurface == EGL_NO_SURFACE)
        return false;

    const EGLint egl_context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);
    if (g_EglContext == EGL_NO_CONTEXT)
        return false;

    eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);
    return true;
}

void MainLoopStep()
{
    ImGuiIO& io = ImGui::GetIO();
    if (g_EglDisplay == EGL_NO_DISPLAY)
        return;

    PollUnicodeChars();

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    static bool show_demo_window = true;
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui::Render();

    // Rendering
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
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

    if (g_EglDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(g_EglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (g_EglContext != EGL_NO_CONTEXT) eglDestroyContext(g_EglDisplay, g_EglContext);
        if (g_EglSurface != EGL_NO_SURFACE) eglDestroySurface(g_EglDisplay, g_EglSurface);
        eglTerminate(g_EglDisplay);
    }

    ANativeWindow_release(g_App->window);
    g_Initialized = false;
}

// Helper functions for input handling via JNI
static int ShowSoftKeyboardInput()
{
    JavaVM* java_vm = g_App->activity->vm;
    JNIEnv* java_env = nullptr;

    if (java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6) == JNI_ERR)
        return -1;

    if (java_vm->AttachCurrentThread(&java_env, nullptr) != JNI_OK)
        return -2;

    jclass native_activity_clazz = java_env->GetObjectClass(g_App->activity->clazz);
    if (!native_activity_clazz)
        return -3;

    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "showSoftInput", "()V");
    if (!method_id)
        return -4;

    java_env->CallVoidMethod(g_App->activity->clazz, method_id);
    java_vm->DetachCurrentThread();

    return 0;
}

static int PollUnicodeChars()
{
    JavaVM* java_vm = g_App->activity->vm;
    JNIEnv* java_env = nullptr;

    if (java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6) == JNI_ERR)
        return -1;

    if (java_vm->AttachCurrentThread(&java_env, nullptr) != JNI_OK)
        return -2;

    jclass native_activity_clazz = java_env->GetObjectClass(g_App->activity->clazz);
    if (!native_activity_clazz)
        return -3;

    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "pollUnicodeChar", "()I");
    if (!method_id)
        return -4;

    ImGuiIO& io = ImGui::GetIO();
    jint unicode_character;
    while ((unicode_character = java_env->CallIntMethod(g_App->activity->clazz, method_id)) != 0)
        io.AddInputCharacter(unicode_character);

    java_vm->DetachCurrentThread();
    return 0;
}
