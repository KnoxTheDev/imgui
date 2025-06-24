// dear imgui: standalone example application for Android + OpenGL ES 3
//
// PATCH: This file has been modified to include the UI and logic from the
// "Snake Bypass" example. The core Android EGL/main loop structure is
// preserved, while the ImGui rendering part is replaced.

#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>
#include <stdio.h> // For logging if needed

// Data
static EGLDisplay           g_EglDisplay = EGL_NO_DISPLAY;
static EGLSurface           g_EglSurface = EGL_NO_SURFACE;
static EGLContext           g_EglContext = EGL_NO_CONTEXT;
static struct android_app*  g_App = nullptr;
static bool                 g_Initialized = false;
static char                 g_LogTag[] = "ImGuiExample";
static std::string          g_IniFilename = "";

// PATCH: State variables from the Snake Bypass example
static int emu = 0, ver = 0;
static bool done = false;
static bool wide_enabled = false;
static float wide = 47.0f;
static bool r_no = false, t_no = false, g_no = false, cross_small = false;
static bool night = false, insta = false, xfx = false, luffy = false, zero = false;

// Forward declarations of helper functions
static void Init(struct android_app* app);
static void Shutdown();
static void MainLoopStep();
static int ShowSoftKeyboardInput();
static int PollUnicodeChars();
static int GetAssetData(const char* filename, void** out_data);
static void ApplySnakeTheme();

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

        while (ALooper_pollOnce(g_Initialized ? 0 : -1, nullptr, &out_events, (void**)&out_data) >= 0)
        {
            if (out_data != nullptr)
                out_data->process(app, out_data);

            if (app->destroyRequested != 0)
            {
                if (g_Initialized) // Ensure shutdown is called
                    Shutdown();
                return;
            }
        }
        if (g_Initialized)
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
    {
        g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (g_EglDisplay == EGL_NO_DISPLAY) __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglGetDisplay(EGL_DEFAULT_DISPLAY) returned EGL_NO_DISPLAY");
        if (eglInitialize(g_EglDisplay, 0, 0) != EGL_TRUE) __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglInitialize() returned with an error");
        const EGLint egl_attributes[] = { EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE };
        EGLint num_configs = 0;
        if (eglChooseConfig(g_EglDisplay, egl_attributes, nullptr, 0, &num_configs) != EGL_TRUE) __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglChooseConfig() returned with an error");
        if (num_configs == 0) __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglChooseConfig() returned 0 matching config");
        EGLConfig egl_config;
        eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs);
        EGLint egl_format;
        eglGetConfigAttrib(g_EglDisplay, egl_config, EGL_NATIVE_VISUAL_ID, &egl_format);
        ANativeWindow_setBuffersGeometry(g_App->window, 0, 0, egl_format);
        const EGLint egl_context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
        g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);
        if (g_EglContext == EGL_NO_CONTEXT) __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglCreateContext() returned EGL_NO_CONTEXT");
        g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_App->window, nullptr);
        eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    g_IniFilename = std::string(app->activity->internalDataPath) + "/imgui.ini";
    io.IniFilename = g_IniFilename.c_str();

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init(g_App->window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    // PATCH: Apply the custom theme from the Snake Bypass example
    ApplySnakeTheme();

    // PATCH: Load custom font from assets
    // IMPORTANT: You must place 'Ruda-Bold.ttf' inside 'examples/example_android_opengl3/android/app/src/main/assets/'
    void* font_data;
    int font_data_size;
    font_data_size = GetAssetData("Ruda-Bold.ttf", &font_data);
    if (font_data_size > 0)
    {
        ImFont* font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 24.0f); // Increased size for mobile
        IM_ASSERT(font != nullptr);
        io.FontDefault = font;
    }
    else
    {
        __android_log_print(ANDROID_LOG_WARN, g_LogTag, "Could not load Ruda-Bold.ttf from assets. Using default font.");
        ImFontConfig font_cfg;
        font_cfg.SizePixels = 22.0f;
        io.Fonts->AddFontDefault(&font_cfg);
    }

    // PATCH: Scaled up for better readability on high-DPI mobile screens
    ImGui::GetStyle().ScaleAllSizes(2.5f);

    g_Initialized = true;
}

void MainLoopStep()
{
    ImGuiIO& io = ImGui::GetIO();
    if (g_EglDisplay == EGL_NO_DISPLAY)
        return;

    // Our state (defined as static globals at the top of the file)
    static ImVec4 clear_color = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

    // Poll Unicode characters via JNI (for text input)
    PollUnicodeChars();
    static bool WantTextInputLast = false;
    if (io.WantTextInput && !WantTextInputLast)
        ShowSoftKeyboardInput();
    WantTextInputLast = io.WantTextInput;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    // PATCH: Main UI rendering code from Snake Bypass example is now here.
    // WARNING: Hardcoded window size is not ideal for Android's diverse screen sizes.
    // For a real app, consider making the window movable/resizable or scaling based on screen size.
    const ImVec2 gui_size = ImVec2(io.DisplaySize.x * 0.9f, io.DisplaySize.y * 0.8f);
    ImGui::SetNextWindowSize(gui_size, ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));

    static bool show_main_window = true;
    if (ImGui::Begin("SNAKE BYPASS", &show_main_window, ImGuiWindowFlags_NoResize))
    {
        if (ImGui::BeginTabBar("Tabs##Snake"))
        {
            if (ImGui::BeginTabItem("BYPASS"))
            {
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "Welcome to SNAKE PRIVATE BYPASS");
                ImGui::Separator();
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::Text("SELECT YOUR EMULATOR :");
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::RadioButton("GAMELOOP 7.1##emu", &emu, 0); ImGui::SameLine();
                ImGui::RadioButton("SMARTGAGA##emu", &emu, 1);
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::Text("SELECT YOUR GAME VERSION :");
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::RadioButton("Global##ver", &ver, 0); ImGui::SameLine();
                ImGui::RadioButton("Korea##ver", &ver, 1); ImGui::SameLine();
                ImGui::RadioButton("Taiwan##ver", &ver, 2); ImGui::SameLine();
                ImGui::RadioButton("Vietnam##ver", &ver, 3);
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                if (ImGui::Button("BYPASS EMULATOR", ImVec2(-1, 0))) { done = true; }
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                if (ImGui::Button("SAFE EXIT", ImVec2(-1, 0))) { ANativeActivity_finish(g_App->activity); } // PATCH: Android-specific exit
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                if (ImGui::Button("REST GUEST", ImVec2(-1, 0))) { done = false; }
                if (done)
                {
                    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                    ImGui::TextColored(ImVec4(0, 1, 0, 1), "BYPASS DONE SUCCESSFUL");
                    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("MISC"))
            {
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "SNAKE PRIVATE BYPASS");
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::Text("Memory Hacks");
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::Checkbox("Wide View##widechk", &wide_enabled);
                ImGui::SameLine();
                ImGui::SliderFloat("##wide", &wide, 0.0f, 100.0f, "%.0f");
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::Checkbox("No Recoil##no", &r_no); ImGui::SameLine();
                ImGui::Checkbox("No Tree##no", &t_no); ImGui::SameLine();
                ImGui::Checkbox("No Grass##no", &g_no); ImGui::SameLine();
                ImGui::Checkbox("Small Crosshair##cross", &cross_small);
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::Checkbox("Night Mode##nm", &night); ImGui::SameLine();
                ImGui::Checkbox("InstaHit##ih", &insta); ImGui::SameLine();
                ImGui::Checkbox("X-Effect##xf", &xfx); ImGui::SameLine();
                ImGui::Checkbox("LUFFY-HAND##lf", &luffy);
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::Checkbox("ZeroHead##zh", &zero);
                ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
    if (!show_main_window)
        ANativeActivity_finish(g_App->activity); // PATCH: Android-specific exit

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
    g_Initialized = false; // Set early to stop the main loop
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
    g_EglDisplay = EGL_NO_DISPLAY;
    g_EglContext = EGL_NO_CONTEXT;
    g_EglSurface = EGL_NO_SURFACE;
    ANativeWindow_release(g_App->window);
}

// PATCH: The ApplySnakeTheme function, copied directly
static void ApplySnakeTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.95f);
    ImVec4 color1 = ImVec4(232.0f/255.0f, 163.0f/255.0f, 33.0f/255.0f, 1.00f);
    ImVec4 color2 = ImVec4(130.0f/255.0f, 92.0f/255.0f, 38.0f/255.0f, 1.00f);
    ImVec4 tab_focused   = ImVec4(199.0f/255.0f, 140.0f/255.0f, 54.0f/255.0f, 1.00f);
    ImVec4 tab_unfocused = color2;
    style.Colors[ImGuiCol_Header]        = color2;
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(140.0f/255.0f, 100.0f/255.0f, 45.0f/255.0f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]  = ImVec4(150.0f/255.0f, 110.0f/255.0f, 50.0f/255.0f, 1.00f);
    style.Colors[ImGuiCol_Tab]                 = tab_unfocused;
    style.Colors[ImGuiCol_TabUnfocused]        = tab_unfocused;
    style.Colors[ImGuiCol_TabUnfocusedActive]  = tab_unfocused;
    style.Colors[ImGuiCol_TabActive]           = tab_focused;
    style.Colors[ImGuiCol_TabHovered]          = tab_focused;
    style.Colors[ImGuiCol_TitleBg]          = color1;
    style.Colors[ImGuiCol_TitleBgActive]    = color1;
    style.Colors[ImGuiCol_TitleBgCollapsed] = color1;
    style.Colors[ImGuiCol_Button]        = color2;
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(140.0f/255.0f, 100.0f/255.0f, 45.0f/255.0f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]  = ImVec4(150.0f/255.0f, 110.0f/255.0f, 50.0f/255.0f, 1.00f);
    style.Colors[ImGuiCol_Text]           = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]   = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.80f, 0.00f, 0.50f);
    style.Colors[ImGuiCol_FrameBg]        = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive]  = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_Separator]      = ImVec4(0.45f, 0.40f, 0.40f, 0.50f);
    style.Colors[ImGuiCol_CheckMark] = tab_focused;
    style.Colors[ImGuiCol_SliderGrab]        = color1;
    style.Colors[ImGuiCol_SliderGrabActive]  = color1;
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize  = 0.0f;
    style.TabBorderSize    = 0.0f;
    style.GrabMinSize      = 10.0f;
    style.Colors[ImGuiCol_ResizeGrip]         = ImVec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_ResizeGripHovered]  = ImVec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_ResizeGripActive]   = ImVec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_Border]        = ImVec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_BorderShadow]  = ImVec4(0, 0, 0, 0);
    style.FrameRounding      = 3.0f;
    style.GrabRounding       = 3.0f;
    style.TabRounding        = 2.0f;
    style.WindowBorderSize   = 1.0f;
    style.FrameBorderSize    = 1.0f;
    style.TabBorderSize      = 1.0f;
}

// Helper functions (unchanged)
static int ShowSoftKeyboardInput()
{
    JavaVM* java_vm = g_App->activity->vm;
    JNIEnv* java_env = nullptr;
    jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
    if (jni_return == JNI_ERR) return -1;
    jni_return = java_vm->AttachCurrentThread(&java_env, nullptr);
    if (jni_return != JNI_OK) return -2;
    jclass native_activity_clazz = java_env->GetObjectClass(g_App->activity->clazz);
    if (native_activity_clazz == nullptr) return -3;
    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "showSoftInput", "()V");
    if (method_id == nullptr) return -4;
    java_env->CallVoidMethod(g_App->activity->clazz, method_id);
    jni_return = java_vm->DetachCurrentThread();
    if (jni_return != JNI_OK) return -5;
    return 0;
}

static int PollUnicodeChars()
{
    JavaVM* java_vm = g_App->activity->vm;
    JNIEnv* java_env = nullptr;
    jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
    if (jni_return == JNI_ERR) return -1;
    jni_return = java_vm->AttachCurrentThread(&java_env, nullptr);
    if (jni_return != JNI_OK) return -2;
    jclass native_activity_clazz = java_env->GetObjectClass(g_App->activity->clazz);
    if (native_activity_clazz == nullptr) return -3;
    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "pollUnicodeChar", "()I");
    if (method_id == nullptr) return -4;
    ImGuiIO& io = ImGui::GetIO();
    jint unicode_character;
    while ((unicode_character = java_env->CallIntMethod(g_App->activity->clazz, method_id)) != 0)
        io.AddInputCharacter(unicode_character);
    jni_return = java_vm->DetachCurrentThread();
    if (jni_return != JNI_OK) return -5;
    return 0;
}

static int GetAssetData(const char* filename, void** outData)
{
    int num_bytes = 0;
    AAsset* asset_descriptor = AAssetManager_open(g_App->activity->assetManager, filename, AASSET_MODE_BUFFER);
    if (asset_descriptor)
    {
        num_bytes = AAsset_getLength(asset_descriptor);
        *outData = IM_ALLOC(num_bytes);
        int64_t num_bytes_read = AAsset_read(asset_descriptor, *outData, num_bytes);
        AAsset_close(asset_descriptor);
        IM_ASSERT(num_bytes_read == num_bytes);
    }
    return num_bytes;
}