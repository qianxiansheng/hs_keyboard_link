#include "keylink.h"

// keylink.cpp : 定义应用程序的入口点。
//

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <unordered_map>

#include "resource.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

#include "function/function.h"
#include "sequencer/ingseq.h"
#include "keyboard/keyboard.h"
#include "util/utils.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
HWND hWnd;                                      // 
WCHAR szTitle[MAX_LOADSTRING]       = APP_TITLE;// 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING] = APP_TITLE;// 主窗口类名

// Data stored per platform window
struct WGL_WindowData { HDC hDC; };

// Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;
static int              g_Width;
static int              g_Height;

// Global Keyboard hook
static HHOOK            g_hook;
static bool             g_hook_enabled = false;

static bool             g_hidden = false;

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


// 此代码模块中包含的函数的前向声明:
static ATOM                MyRegisterClass(HINSTANCE hInstance);
static BOOL                InitInstance(HINSTANCE, int);
static void                CleanupInstance();
static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
static INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// WGL Context
static int initWGL(HWND hwnd);
static void CleanupWGL(HWND hwnd);
static void InitImGuiContext();
static void CleanupImGuiContext();

// ImGui multi viewport
static void Hook_Renderer_CreateWindow(ImGuiViewport* viewport);
static void Hook_Renderer_DestroyWindow(ImGuiViewport* viewport);
static void Hook_Platform_RenderWindow(ImGuiViewport* viewport, void*);
static void Hook_Renderer_SwapBuffers(ImGuiViewport* viewport, void*);

// Application
extern bool main_init(int argc, char* argv[]);
extern void main_shutdown(void);
extern int main_gui();

// Notify Icon
static BOOL AddNotificationIcon(HWND hwnd);
static BOOL DeleteNotificationIcon();

LRESULT KeyProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code >= 0)
    {
        KBDLLHOOKSTRUCT* pKbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        // 检查按键消息
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            // 在这里处理按键事件
            std::cout << "Key pressed, VK code: " << pKbStruct->vkCode << std::endl;

            auto it = kv_to_hid_map.find(pKbStruct->vkCode);
            if (it != kv_to_hid_map.end())
                UserInputKeyDown(it->second);
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
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


void HideApplication()
{
    g_hidden = true;
    printf("hidden\n");
}
void RestoreApplication()
{
    g_hidden = false;
    printf("restore\n");
}


int WINAPI WinMain(_In_ HINSTANCE hInstance, 
    _In_opt_ HINSTANCE hPrevInstance, 
    _In_ LPSTR lpCmdLine, 
    _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化全局字符串
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow))
    {
        ::UnregisterClassW(szWindowClass, hInst);
        return FALSE;
    }

    if (!initWGL(hWnd))
    {
        CleanupInstance();
        ::UnregisterClassW(szWindowClass, hInst);
        return FALSE;
    }

    InitImGuiContext();

    main_init(__argc, __argv);
    KeyboardGLInit();

    // 主消息循环
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        if (g_hidden)
        {
            int r = 0;
            while (r = ::GetMessage(&msg, NULL, 0U, 0U))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
                if (!g_hidden)
                    break;
            }
            if (r == 0)
                done = true;
        }
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        /* Limit the FPS */
        static const int fixed_fps = 60;
        static const int fixed_frame_time = 1000000 / 60;
        static long long last = utils::get_current_system_time_us();
        long long curr = utils::get_current_system_time_us();
        long long frameTime = curr - last;
        if (frameTime < fixed_frame_time)
            std::this_thread::sleep_for(std::chrono::microseconds(fixed_frame_time - frameTime));
        last = curr;

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
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            // Restore the OpenGL rendering context to the main window DC, since platform windows might have changed it.
            wglMakeCurrent(g_MainWindow.hDC, g_hRC);
        }

        // Present
        ::SwapBuffers(g_MainWindow.hDC);
    }
    DeleteNotificationIcon();

    KeyboardGLDestroy();
    main_shutdown();

    CleanupImGuiContext();
    CleanupInstance();
    ::UnregisterClassW(szWindowClass, hInst);

    return (int)0;
}




//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
static ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口和WGL上下文
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
        APP_X, APP_Y, APP_WIDTH, APP_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    if (!initWGL(hWnd))
    {
        ::DestroyWindow(hWnd);
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//   函数: CleanupInstance()
//
//   目标: 清理实例句柄，和WGL上下文
//
static void CleanupInstance()
{
    CleanupWGL(hWnd);
    ::DestroyWindow(hWnd);
}


static bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
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

static void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(NULL, NULL);
    ::ReleaseDC(hWnd, data->hDC);
}

//
//   函数: initWGL(HWND)
//
//   目标: 创建WGL上下文
//
//
static int initWGL(HWND hwnd)
{
    // Initialize OpenGL
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(szWindowClass, hInst);
        return FALSE;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);

    // Load glad
    gladLoadGL();
    return TRUE;
}

//
//   函数: CleanupWGL()
//
//   目标: 清理WGL上下文
//
//
static void CleanupWGL(HWND hwnd)
{
    CleanupDeviceWGL(hwnd, &g_MainWindow);
    wglDeleteContext(g_hRC);
}

//
//   函数: InitImGuiContext()
//
//   目标: 创建ImGui上下文
//
static void InitImGuiContext()
{
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

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(hWnd);
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

}

//
//   函数: CleanupImGuiContext()
//
//   目标: 清理ImGui上下文
//
static void CleanupImGuiContext()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

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

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32.lib")
#include <shellapi.h>
#include <commctrl.h>

UINT const WMAPP_NOTIFYCALLBACK = WM_USER + 1;
NOTIFYICONDATA nid;

static BOOL AddNotificationIcon(HWND hwnd)
{
    nid.cbSize = { sizeof(nid) };
    nid.hWnd = hwnd;
    // add the icon, setting the icon, tooltip, and callback message.
    // the icon will be identified with the GUID
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_SHOWTIP | NIF_GUID;
    nid.uCallbackMessage = WMAPP_NOTIFYCALLBACK;
    LoadIconMetric(hInst, MAKEINTRESOURCE(IDI_ICON1), LIM_SMALL, &nid.hIcon);
    //LoadString(g_hInst, IDS_TOOLTIP, nid.szTip, ARRAYSIZE(nid.szTip));
    return Shell_NotifyIcon(NIM_ADD, &nid);
}

static BOOL DeleteNotificationIcon()
{
    return Shell_NotifyIcon(NIM_DELETE, &nid);
}

// 显示右键菜单
static void ShowContextMenu(HWND hwnd, POINT pt) {
    HMENU hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, 1, L"Exit");
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);

    SetForegroundWindow(hwnd);

    UINT clicked = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hwnd, NULL);
    SendMessage(hwnd, WM_COMMAND, clicked, 0);

    DestroyMenu(hMenu);
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
    case WM_GETICON:
    {
        HICON hIcon = (HICON)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
        return (LRESULT)hIcon;
    }
    case WM_CREATE:
        // add the notification icon
        if (!AddNotificationIcon(hWnd))
        {
            MessageBox(hWnd,
                L"Please read the ReadMe.txt file for troubleshooting",
                L"Error adding icon", MB_OK);
            return -1;
        }
        break;
    case WMAPP_NOTIFYCALLBACK:
        // 处理通知图标消息
        switch (lParam) {
        case WM_LBUTTONDBLCLK:
            RestoreApplication();
            // 双击通知图标时还原窗口
            ShowWindow(hWnd, SW_RESTORE);
            break;
        case WM_RBUTTONDOWN:
        case WM_CONTEXTMENU:
            POINT pt;
            GetCursorPos(&pt);
            ShowContextMenu(hWnd, pt);
            break;
        }
        break;
    case WM_COMMAND:
        if (wParam == 1) { // "Exit"选项的ID为1
            PostQuitMessage(0);
        }
        break;
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
        DeleteNotificationIcon();
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


// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
