#include "SubWindow.h"

SubWindow::SubWindow(uint32_t width, uint32_t height, LPCWSTR classname, LPCWSTR title)
    :m_Width(width), m_Height(height), m_Handle(nullptr), m_ClassName(classname), m_Title(title)
{
}

SubWindow::~SubWindow()
{
}

void SubWindow::RegisterNewClass()
{
    // Register a window class for creating our render window with.
    WNDCLASSEXW windowClass = {};

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = SetupMessageHandler;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = m_hInstance;
    windowClass.hIcon = LoadIcon(m_hInstance, NULL);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = m_ClassName;
    windowClass.hIconSm = LoadIcon(m_hInstance, NULL);

    static ATOM atom = RegisterClassExW(&windowClass);
    assert(atom > 0 && "Failed to register window class");
}

LRESULT SubWindow::SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        SubWindow* const pWnd = static_cast<SubWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&AssignMessageHandler));
        pWnd->Handle(hWnd);
        return pWnd->MessageHandler(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT SubWindow::AssignMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    SubWindow* const pWnd = reinterpret_cast<SubWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->MessageHandler(hWnd, msg, wParam, lParam);
}

LRESULT SubWindow::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CLOSE: {
        PostQuitMessage(0);
    }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}