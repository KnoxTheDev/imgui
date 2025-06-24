#include <jni.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include "xhook.h"
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>

static EGLDisplay g_EglDisplay = EGL_NO_DISPLAY;
static EGLContext g_EglContext = EGL_NO_CONTEXT;
static EGLSurface g_EglSurface = EGL_NO_SURFACE;
static bool g_Initialized = false;
static char g_LogTag[] = "ImGuiHook";
static float g_DpiScale = 2.5f;

static bool show_menu = true;

static int emu = 0, ver = 0;
static bool done = false;
static bool wide_enabled = false;
static float wide = 47.0f;
static bool r_no = false, t_no = false, g_no = false, cross_small = false;
static bool night = false, insta = false, xfx = false, luffy = false, zero = false;

void InitImGui();
void RenderUI();
void ApplySnakeTheme();

EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    if (!g_Initialized) {
        g_EglDisplay = dpy;
        g_EglSurface = surface;
        InitImGui();
        g_Initialized = true;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    RenderUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return orig_eglSwapBuffers(dpy, surface);
}

int32_t (*orig_handle_input)(AInputEvent* event);
int32_t hook_handle_input(AInputEvent* event) {
    if (g_Initialized) {
        ImGui_ImplAndroid_HandleInputEvent(event);
    }
    return orig_handle_input(event);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    const char* target_lib = "libUE4.so";

    xhook_register(target_lib, "eglSwapBuffers", (void*)hook_eglSwapBuffers, (void**)&orig_eglSwapBuffers);
    xhook_register(target_lib, "Android_JNI_handleInput", (void*)hook_handle_input, (void**)&orig_handle_input);

    xhook_refresh(1);

    return JNI_VERSION_1_6;
}

void InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    g_EglContext = eglGetCurrentContext();
    ImGui_ImplAndroid_Init(nullptr);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    ApplySnakeTheme();
    
    io.Fonts->AddFontDefault();
    
    ImGui::GetStyle().ScaleAllSizes(g_DpiScale);
}

void RenderUI()
{
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsMouseReleased(0) && io.MousePos.x < 100 && io.MousePos.y < 100) {
        show_menu = !show_menu;
    }
    
    if (!show_menu) return;

    const ImVec2 base_gui_size = ImVec2(445.0f, 432.0f);
    const ImVec2 scaled_gui_size = ImVec2(base_gui_size.x * g_DpiScale, base_gui_size.y * g_DpiScale);
    ImGui::SetNextWindowSize(scaled_gui_size, ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));

    if (ImGui::Begin("SNAKE BYPASS", &show_menu, ImGuiWindowFlags_NoResize))
    {
        if (ImGui::BeginTabBar("Tabs##Snake"))
        {
            if (ImGui::BeginTabItem("BYPASS"))
            {
                ImGui::Spacing(); ImGui::TextColored(ImVec4(0,1,0,1), "Welcome to SNAKE PRIVATE BYPASS"); ImGui::Separator();
                ImGui::Text("SELECT YOUR EMULATOR :");
                ImGui::RadioButton("GAMELOOP 7.1##emu", &emu, 0); ImGui::SameLine(); ImGui::RadioButton("SMARTGAGA##emu", &emu, 1);
                ImGui::Separator();
                ImGui::Text("SELECT YOUR GAME VERSION :");
                ImGui::RadioButton("Global##ver", &ver, 0); ImGui::SameLine(); ImGui::RadioButton("Korea##ver", &ver, 1); ImGui::SameLine();
                ImGui::RadioButton("Taiwan##ver", &ver, 2); ImGui::SameLine(); ImGui::RadioButton("Vietnam##ver", &ver, 3);
                if (ImGui::Button("BYPASS EMULATOR", ImVec2(-1, 0))) { done = true; }
                if (ImGui::Button("REST GUEST", ImVec2(-1, 0))) { done = false; }
                if (done) { ImGui::TextColored(ImVec4(0,1,0,1), "BYPASS DONE SUCCESSFUL"); }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("MISC"))
            {
                ImGui::Text("Memory Hacks"); ImGui::Separator();
                ImGui::Checkbox("Wide View##widechk", &wide_enabled); ImGui::SameLine(); ImGui::SliderFloat("##wide", &wide, 40.0f, 200.0f, "%.0f");
                ImGui::Checkbox("No Recoil##no", &r_no); ImGui::SameLine(); ImGui::Checkbox("No Tree##no", &t_no); ImGui::SameLine(); ImGui::Checkbox("No Grass##no", &g_no); ImGui::SameLine(); ImGui::Checkbox("Small Crosshair##cross", &cross_small);
                ImGui::Checkbox("Night Mode##nm", &night); ImGui::SameLine(); ImGui::Checkbox("InstaHit##ih", &insta); ImGui::SameLine(); ImGui::Checkbox("X-Effect##xf", &xfx); ImGui::SameLine(); ImGui::Checkbox("LUFFY-HAND##lf", &luffy);
                ImGui::Checkbox("ZeroHead##zh", &zero);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void ApplySnakeTheme()
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