#include "Window.h"

Window::Window(uint32_t width, uint32_t height, LPCWSTR className, LPCWSTR title)
    :SubWindow(width, height, className, title), m_Handle(SubWindow::GetHandle())
    /*Graphics(SubWindow::GetHandle(), width, height, false)*/
{
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    ParseCommandLineArguments();
}

Window::~Window() {
    DestroyWindow(m_Handle);
}

void Window::ParseCommandLineArguments() {
    int argc;
    wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

    for (size_t i = 0; i < argc; ++i)
    {
        if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0)
        {
            m_Width = ::wcstol(argv[++i], nullptr, 10);
        }
        if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0)
        {
            m_Height = ::wcstol(argv[++i], nullptr, 10);
        }
        if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0)
        {
            graphics->g_UseWarp = true;
        }
    }

    // Free memory allocated by CommandLineToArgvW
    ::LocalFree(argv);
}

void Window::Initialize() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    RECT windowRect = { 0, 0, static_cast<LONG>(m_Width), static_cast<LONG>(m_Height) };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    // Center the window within the screen. Clamp to 0, 0 for the top-left corner.
    int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);
    int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);

    m_Handle = CreateWindowExW(
        NULL,
        m_ClassName,
        m_Title,
        WS_OVERLAPPEDWINDOW,
        windowX,
        windowY,
        windowWidth,
        windowHeight,
        NULL,
        NULL,
        m_hInstance,
        (void*)this
    );
    if (!m_Handle) {
        DWORD error = GetLastError();

        LPWSTR errorMsg;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            nullptr,
            error,
            0, // Default language
            (LPWSTR)&errorMsg,
            0,
            nullptr
        );

        OutputDebugString(errorMsg);

        // Release the allocated buffer for the error message
        LocalFree(errorMsg);

        assert(m_Handle && "Failed to create window");
    }

    graphics = new Graphics(m_Handle, m_Width, m_Height, false);
    graphics->Initialize();
}

void Window::SetFullscreen(bool fullscreen) {
    if (graphics->g_Fullscreen != fullscreen)
    {
        graphics->g_Fullscreen = fullscreen;

        if (graphics->g_Fullscreen) // Switching to fullscreen.
        {
            // Store the current window dimensions so they can be restored 
            // when switching out of fullscreen state.
            ::GetWindowRect(m_Handle, &graphics->g_WindowRect);

            // Set the window style to a borderless window so the client area fills
            // the entire screen.
            UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

            ::SetWindowLongW(m_Handle, GWL_STYLE, windowStyle);

            // Query the name of the nearest display device for the window.
            // This is required to set the fullscreen dimensions of the window
            // when using a multi-monitor setup.
            HMONITOR hMonitor = ::MonitorFromWindow(m_Handle, MONITOR_DEFAULTTONEAREST);
            MONITORINFOEX monitorInfo = {};
            monitorInfo.cbSize = sizeof(MONITORINFOEX);
            ::GetMonitorInfo(hMonitor, &monitorInfo);

            ::SetWindowPos(m_Handle, HWND_TOP,
                monitorInfo.rcMonitor.left,
                monitorInfo.rcMonitor.top,
                monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
                monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
                SWP_FRAMECHANGED | SWP_NOACTIVATE);

            ::ShowWindow(m_Handle, SW_MAXIMIZE);
        }
        else
        {
            // Restore all the window decorators.
            ::SetWindowLong(m_Handle, GWL_STYLE, WS_OVERLAPPEDWINDOW);

            ::SetWindowPos(m_Handle, HWND_NOTOPMOST,
                graphics->g_WindowRect.left,
                graphics->g_WindowRect.top,
                graphics->g_WindowRect.right - graphics->g_WindowRect.left,
                graphics->g_WindowRect.bottom - graphics->g_WindowRect.top,
                SWP_FRAMECHANGED | SWP_NOACTIVATE);

            ::ShowWindow(m_Handle, SW_NORMAL);
        }
    }
}

LRESULT Window::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message)
    {
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    {
        bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
        switch (wParam)
        {
        case 'V':
            graphics->g_VSync = !graphics->g_VSync;
            break;
        case VK_ESCAPE:
            ::PostQuitMessage(0);
            break;
        case VK_RETURN:
            if (alt)
            {
        case VK_F11:
            SetFullscreen(!graphics->g_Fullscreen);
            }
            break;
        }
    }
    break;
    case WM_SYSCHAR:
        break;
    case WM_SIZE:
    {
        RECT clientRect = {};
        ::GetClientRect(m_Handle, &clientRect);
        int width = clientRect.right - clientRect.left;
        int height = clientRect.bottom - clientRect.top;
        graphics->Resize(width, height);
    }
    break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}
