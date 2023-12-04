// Dear ImGui: standalone example application for Win32 + OpenGL 3

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// This is provided for completeness, however it is strongly recommended you use OpenGL with SDL or GLFW.

#include "gui_win32_gl3.h"

#include "pch.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <windowsx.h>
#include <glad/glad/glad.h>
#include <tchar.h>

#include <unordered_map>
#include "function/function.h"
#include "keyboard/keyboard.h"
#include "sequencer/ingseq.h"


extern std::unordered_map<KLFunctionID, KLFunction> function_map;

std::unordered_map<USHORT, KLFunction> kv_to_hid_map = {
    {0x08, function_map[KLF_BS]},
    {0x09, function_map[KLF_TAB]},
    {0x0D, function_map[KLF_ENTER]},
    {0x10, function_map[KLF_SHIFT_L]},
    {0x11, function_map[KLF_CTRL_L]},
    {0x12, function_map[KLF_ALT_L]},
    {0x13, function_map[KLF_PAUSE]},
    {0x14, function_map[KLF_CAPS]},
    {0x1B, function_map[KLF_ESC]},
    {0x20, function_map[KLF_SP]},
    {0x21, function_map[KLF_PAGEUP]},
    {0x22, function_map[KLF_PAGEDOWN]},
    {0x23, function_map[KLF_END]},
    {0x24, function_map[KLF_HOME]},
    {0x25, function_map[KLF_LEFT]},
    {0x26, function_map[KLF_UP]},
    {0x27, function_map[KLF_RIGHT]},
    {0x28, function_map[KLF_DOWN]},
    {0x2C, function_map[KLF_PRINT]},
    {0x2D, function_map[KLF_INS]},
    {0x2E, function_map[KLF_DEL]},
    {0x30, function_map[KLF_0]},
    {0x31, function_map[KLF_1]},
    {0x32, function_map[KLF_2]},
    {0x33, function_map[KLF_3]},
    {0x34, function_map[KLF_4]},
    {0x35, function_map[KLF_5]},
    {0x36, function_map[KLF_6]},
    {0x37, function_map[KLF_7]},
    {0x38, function_map[KLF_8]},
    {0x39, function_map[KLF_9]},
    {0x41, function_map[KLF_A]},
    {0x42, function_map[KLF_B]},
    {0x43, function_map[KLF_C]},
    {0x44, function_map[KLF_D]},
    {0x45, function_map[KLF_E]},
    {0x46, function_map[KLF_F]},
    {0x47, function_map[KLF_G]},
    {0x48, function_map[KLF_H]},
    {0x49, function_map[KLF_I]},
    {0x4A, function_map[KLF_J]},
    {0x4B, function_map[KLF_K]},
    {0x4C, function_map[KLF_L]},
    {0x4D, function_map[KLF_M]},
    {0x4E, function_map[KLF_N]},
    {0x4F, function_map[KLF_O]},
    {0x50, function_map[KLF_P]},
    {0x51, function_map[KLF_Q]},
    {0x52, function_map[KLF_R]},
    {0x53, function_map[KLF_S]},
    {0x54, function_map[KLF_T]},
    {0x55, function_map[KLF_U]},
    {0x56, function_map[KLF_V]},
    {0x57, function_map[KLF_W]},
    {0x58, function_map[KLF_X]},
    {0x59, function_map[KLF_Y]},
    {0x5A, function_map[KLF_Z]},
    {0x5B, function_map[KLF_WIN_L]},
    {0x5C, function_map[KLF_WIN_R]},
    {0x60, function_map[KLF_KP_0]},
    {0x61, function_map[KLF_KP_1]},
    {0x62, function_map[KLF_KP_2]},
    {0x63, function_map[KLF_KP_3]},
    {0x64, function_map[KLF_KP_4]},
    {0x65, function_map[KLF_KP_5]},
    {0x66, function_map[KLF_KP_6]},
    {0x67, function_map[KLF_KP_7]},
    {0x68, function_map[KLF_KP_8]},
    {0x69, function_map[KLF_KP_9]},
    {0x6A, function_map[KLF_KP_MULT]},
    {0x6B, function_map[KLF_KP_PLUS]},
    {0x6D, function_map[KLF_KP_MINUS]},
    {0x6E, function_map[KLF_KP_DEL]},
    {0x6F, function_map[KLF_KP_DIV]},
    {0x70, function_map[KLF_F1]},
    {0x71, function_map[KLF_F2]},
    {0x72, function_map[KLF_F3]},
    {0x73, function_map[KLF_F4]},
    {0x74, function_map[KLF_F5]},
    {0x75, function_map[KLF_F6]},
    {0x76, function_map[KLF_F7]},
    {0x77, function_map[KLF_F8]},
    {0x78, function_map[KLF_F9]},
    {0x79, function_map[KLF_F10]},
    {0x7A, function_map[KLF_F11]},
    {0x7B, function_map[KLF_F12]},
    {0x90, function_map[KLF_KP_NUM]},
    {0x91, function_map[KLF_SCL]},
    {0xA0, function_map[KLF_SHIFT_L]},
    {0xA1, function_map[KLF_SHIFT_R]},
    {0xA2, function_map[KLF_CTRL_L]},
    {0xA3, function_map[KLF_CTRL_R]},
    {0xA4, function_map[KLF_ALT_L]},
    {0xA5, function_map[KLF_ALT_R]},
    {0xC0, function_map[KLF_TILDE]},
};

// Data stored per platform window
struct WGL_WindowData { HDC hDC; };

// Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;
static int              g_Width;
static int              g_Height;

// Forward declarations of helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
void ResetDeviceWGL();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Support function for multi-viewports
// Unlike most other backend combination, we need specific hooks to combine Win32+OpenGL.
// We could in theory decide to support Win32-specific code in OpenGL backend via e.g. an hypothetical ImGui_ImplOpenGL3_InitForRawWin32().
static void Hook_Renderer_CreateWindow(ImGuiViewport* viewport)
{
    assert(viewport->RendererUserData == NULL);

    WGL_WindowData* data = IM_NEW(WGL_WindowData);
    CreateDeviceWGL((HWND)viewport->PlatformHandle, data);
    viewport->RendererUserData = data;
}

static void Hook_Renderer_DestroyWindow(ImGuiViewport* viewport)
{
    if (viewport->RendererUserData != NULL)
    {
        WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData;
        CleanupDeviceWGL((HWND)viewport->PlatformHandle, data);
        IM_DELETE(data);
        viewport->RendererUserData = NULL;
    }
}

static void Hook_Platform_RenderWindow(ImGuiViewport* viewport, void*)
{
    // Activate the platform window DC in the OpenGL rendering context
    if (WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData)
        wglMakeCurrent(data->hDC, g_hRC);
}

static void Hook_Renderer_SwapBuffers(ImGuiViewport* viewport, void*)
{
    if (WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData)
        ::SwapBuffers(data->hDC);
}

HWND hwnd;

extern KeyboardGLContext kbv_draw_ctx;

HHOOK g_hook;
bool g_hook_enabled = false;

LRESULT KeyProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code >= 0) 
    {
        KBDLLHOOKSTRUCT* pKbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        // 检查按键消息
        if (wParam == WM_KEYDOWN) {
            // 在这里处理按键事件
            std::cout << "Key pressed, VK code: " << pKbStruct->vkCode << std::endl;

            auto it = kv_to_hid_map.find(pKbStruct->vkCode);
            if (it != kv_to_hid_map.end())
                UserInputKeyDown(it->second);
        }
        else if (wParam == WM_KEYUP) {
            // 在这里处理按键事件
            std::cout << "Key pressed, VK code: " << pKbStruct->vkCode << std::endl;

            auto it = kv_to_hid_map.find(pKbStruct->vkCode);
            if (it != kv_to_hid_map.end())
                UserInputKeyUp(it->second);
        }
        return 1;
    }
    return CallNextHookEx(g_hook, code, wParam, lParam);
}

void EnableKeyHook(bool enable)
{
    if (g_hook_enabled == enable)
        return;

    if (enable)
    {
        g_hook = SetWindowsHookEx(13, KeyProc, 0, 0);
    }
    else
    {
        UnhookWindowsHookEx(g_hook);
    }
    g_hook_enabled = enable;
}

bool main_init(int argc, char* argv[]);
void main_shutdown(void);
int main_gui();

// Main code
int main_(int argc, char** argv)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    hwnd = ::CreateWindowW(
        wc.lpszClassName, 
        APP_TITLE, 
        WS_POPUP,
        APP_X, APP_Y, APP_WIDTH, APP_HEIGHT, NULL, NULL, wc.hInstance, NULL);

    // Initialize OpenGL
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);
    gladLoadGL();

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();


    // Win32+GL needs specific hooks for viewport, as there are specific things needed to tie Win32 and GL api.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        IM_ASSERT(platform_io.Renderer_CreateWindow == NULL);
        IM_ASSERT(platform_io.Renderer_DestroyWindow == NULL);
        IM_ASSERT(platform_io.Renderer_SwapBuffers == NULL);
        IM_ASSERT(platform_io.Platform_RenderWindow == NULL);
        platform_io.Renderer_CreateWindow = Hook_Renderer_CreateWindow;
        platform_io.Renderer_DestroyWindow = Hook_Renderer_DestroyWindow;
        platform_io.Renderer_SwapBuffers = Hook_Renderer_SwapBuffers;
        platform_io.Platform_RenderWindow = Hook_Platform_RenderWindow;
    }

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    int dpi = GetDpiForWindow(hwnd);

    if (!main_init(argc, argv))
        ;//glfwSetWindowShouldClose(window, 1);

    KeyboardGLInit();

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();

        KeyboardGLDraw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0.0f, 0.0f, APP_WIDTH, APP_HEIGHT);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        ImGui::NewFrame();

        int r = main_gui();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, g_Width, g_Height);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            // Restore the OpenGL rendering context to the main window DC, since platform windows might have changed it.
            wglMakeCurrent(g_MainWindow.hDC, g_hRC);
        }

        // Present
        ::SwapBuffers(g_MainWindow.hDC);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    KeyboardGLDestroy();
    main_shutdown();

    CleanupDeviceWGL(hwnd, &g_MainWindow);
    wglDeleteContext(g_hRC);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);
    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(NULL, NULL);
    ::ReleaseDC(hWnd, data->hDC);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_Width = LOWORD(lParam);
            g_Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        if (VK_PROCESSKEY == wParam)
            return 0;
        std::cout << "Key pressed, VK code: " << wParam << std::endl;
        auto& function = kv_to_hid_map[wParam];
        UserInputKeyDown(function);
        return 0;
    }
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        std::cout << "Key pressed, VK code: " << wParam << std::endl;
        auto& function = kv_to_hid_map[wParam];
        UserInputKeyUp(function);
        return 0;
    }
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
