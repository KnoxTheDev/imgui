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
static std::string g_IniFilename = "";  // Store ini filename
static const char* g_LogTag = "ImGuiExample";
static std::ofstream logFile;  // File stream for logging

// Global JNI references
JavaVM* g_JVM = nullptr;
jobject g_Context = nullptr;

// Helper function to log to a file
void LogToFile(const std::string& message)
{
    if (logFile.is_open())
    {
        logFile << message << std::endl;
    }
    else
    {
        __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "Failed to write to log file!");
    }
}

// Function to open a URL in the default browser via JNI
void OpenURLInBrowser(const char* url)
{
    if (g_JVM == nullptr || g_Context == nullptr)
    {
        LogToFile("OpenURLInBrowser: JVM or context is null");
        return;
    }

    JNIEnv* env;
    if (g_JVM->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
    {
        LogToFile("OpenURLInBrowser: Failed to get JNI environment");
        return;
    }

    jclass context_class = env->GetObjectClass(g_Context);
    jmethodID method_id = env->GetMethodID(context_class, "openURLInBrowser", "(Ljava/lang/String;)V");

    if (method_id == nullptr)
    {
        LogToFile("OpenURLInBrowser: Failed to find openURLInBrowser method");
        return;
    }

    // Convert C string to JNI string (Java string)
    jstring j_url = env->NewStringUTF(url);

    // Call the openURLInBrowser method
    env->CallVoidMethod(g_Context, method_id, j_url);

    // Clean up local references
    env->DeleteLocalRef(j_url);
    env->DeleteLocalRef(context_class);
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

    // Open log file for writing in append mode
    std::string logFilePath = std::string(app->activity->internalDataPath) + "/log.txt";
    logFile.open(logFilePath, std::ios::out | std::ios::app);  // Open for appending
    if (logFile.is_open())
    {
        LogToFile("App Started");
    }
    else
    {
        __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "Failed to open log file!");
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
    if (g_EglDisplay == EGL_NO_DISPLAY)
    {
        LogToFile("EGL_NO_DISPLAY");
        return;
    }

    if (eglInitialize(g_EglDisplay, 0, 0) != EGL_TRUE)
    {
        LogToFile("eglInitialize() failed");
        return;
    }

    const EGLint egl_attributes[] = { 
        EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, 
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE 
    };

    EGLint num_configs = 0;
    EGLConfig egl_config;
    if (eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs) != EGL_TRUE || num_configs == 0)
    {
        LogToFile("eglChooseConfig() failed");
        return;
    }

    g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_App->window, nullptr);
    if (g_EglSurface == EGL_NO_SURFACE)
    {
        LogToFile("eglCreateWindowSurface() failed");
        return;
    }

    const EGLint egl_context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);
    if (g_EglContext == EGL_NO_CONTEXT)
    {
        LogToFile("eglCreateContext() failed");
        return;
    }

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

    // Get the JVM and context
    g_App->activity->vm->GetEnv((void**)&g_JVM, JNI_VERSION_1_6);
    g_Context = app->activity->clazz;
}

// Poll input events (simplified)
static int PollUnicodeChars()
{
    return 0;
}

void MainLoopStep()
{
    if (!g_Initialized) return;

    ImGuiIO& io = ImGui::GetIO();
    PollUnicodeChars();

    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    // ImGui Window
    static bool show_knoxy_window = true;
    ImGui::Begin("Knoxy HAX", &show_knoxy_window, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Welcome to Knoxy HAX!");
    ImGui::Text("This is a custom window, scaled for Android.");
    
    if (ImGui::Button("Click Me"))
    {
        LogToFile("Button clicked!");
        OpenURLInBrowser("https://www.youtube.com/@sultanrecords7");
    }
    ImGui::End();

    // Rendering setup
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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
