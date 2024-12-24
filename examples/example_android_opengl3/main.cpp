#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include <android/log.h>
#include <android_native_app_glue.h>
#include <jni.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <string>
#include <cstring>

// Global variables
static EGLDisplay g_EglDisplay = EGL_NO_DISPLAY;
static EGLSurface g_EglSurface = EGL_NO_SURFACE;
static EGLContext g_EglContext = EGL_NO_CONTEXT;
static struct android_app* g_App = nullptr;
static bool g_Initialized = false;
static AAssetManager* g_AssetManager = nullptr;
static std::string g_TextInputBuffer; // Buffer to store text input

// Forward declarations
void Init(struct android_app* app);
void Shutdown();
void MainLoopStep();
void ShowKeyboard();
void HideKeyboard();

// Helper function to log messages
void LogMessage(const char* message) {
    __android_log_print(ANDROID_LOG_INFO, "ThunderMod", "%s", message);
}

// JNI callback for receiving text input
extern "C" JNIEXPORT void JNICALL
Java_imgui_example_android_MainActivity_onTextInput(JNIEnv* env, jclass clazz, jstring input) {
    const char* nativeString = env->GetStringUTFChars(input, nullptr);
    g_TextInputBuffer += nativeString; // Append new input to the buffer
    env->ReleaseStringUTFChars(input, nativeString);
}

// JNI function to show the soft keyboard
void ShowKeyboard() {
    JNIEnv* env;
    g_App->activity->vm->AttachCurrentThread(&env, nullptr);

    jclass mainActivityClass = env->GetObjectClass(g_App->activity->clazz);
    jmethodID showKeyboardMethod = env->GetMethodID(mainActivityClass, "showKeyboard", "()V");
    env->CallVoidMethod(g_App->activity->clazz, showKeyboardMethod);

    g_App->activity->vm->DetachCurrentThread();
}

// JNI function to hide the soft keyboard
void HideKeyboard() {
    JNIEnv* env;
    g_App->activity->vm->AttachCurrentThread(&env, nullptr);

    jclass mainActivityClass = env->GetObjectClass(g_App->activity->clazz);
    jmethodID hideKeyboardMethod = env->GetMethodID(mainActivityClass, "hideKeyboard", "()V");
    env->CallVoidMethod(g_App->activity->clazz, hideKeyboardMethod);

    g_App->activity->vm->DetachCurrentThread();
}

// Event handlers
static void HandleAppCmd(struct android_app* app, int32_t appCmd) {
    switch (appCmd) {
        case APP_CMD_INIT_WINDOW:
            Init(app);
            break;
        case APP_CMD_TERM_WINDOW:
            Shutdown();
            break;
    }
}

static int32_t HandleInputEvent(struct android_app* app, AInputEvent* inputEvent) {
    return ImGui_ImplAndroid_HandleInputEvent(inputEvent);
}

// Main entry point
void android_main(struct android_app* app) {
    app->onAppCmd = HandleAppCmd;
    app->onInputEvent = HandleInputEvent;
    g_AssetManager = app->activity->assetManager;

    while (true) {
        int out_events;
        struct android_poll_source* out_data;

        while (ALooper_pollOnce(g_Initialized ? 0 : -1, nullptr, &out_events, (void**)&out_data) >= 0) {
            if (out_data != nullptr)
                out_data->process(app, out_data);

            if (app->destroyRequested != 0) {
                if (g_Initialized)
                    Shutdown();
                return;
            }
        }

        MainLoopStep();
    }
}

// Initialize EGL, ImGui, and custom fonts
void Init(struct android_app* app) {
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

    // Load custom fonts
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    AAsset* asset = AAssetManager_open(g_AssetManager, "fonts/Roboto-Regular.ttf", AASSET_MODE_BUFFER);

    if (asset != nullptr) {
        off_t assetLength = AAsset_getLength(asset);
        const void* fontData = AAsset_getBuffer(asset);
        io.Fonts->AddFontFromMemoryTTF(const_cast<void*>(fontData), assetLength, 16.0f);
        io.FontDefault = io.Fonts->AddFontFromMemoryTTF(const_cast<void*>(fontData), assetLength, 16.0f);
        AAsset_close(asset);
    } else {
        LogMessage("Error: Could not open Roboto-Regular.ttf asset.");
    }

    g_Initialized = true;
}

// Main loop step
void MainLoopStep() {
    if (!g_Initialized) return;

    ImGuiIO& io = ImGui::GetIO();

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    static char buffer[128] = "";

    // Text input box
    if (ImGui::InputText("Input Text", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (ImGui::IsItemFocused()) {
            ShowKeyboard();
        }
    }

    // Append JNI-provided input to the buffer
    if (!g_TextInputBuffer.empty()) {
        strncat(buffer, g_TextInputBuffer.c_str(), sizeof(buffer) - strlen(buffer) - 1);
        g_TextInputBuffer.clear(); // Clear the buffer after appending
    }

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    eglSwapBuffers(g_EglDisplay, g_EglSurface);
}

// Shutdown EGL and ImGui
void Shutdown() {
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
