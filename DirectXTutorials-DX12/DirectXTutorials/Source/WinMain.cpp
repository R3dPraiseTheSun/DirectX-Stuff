#include "pch.h"

#include "Window.h"

uint32_t g_ClientWidth = WINDOW_WIDTH;
uint32_t g_ClientHeight = WINDOW_HEIGHT;


int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    auto window = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, L"MainWindow", L"My First DirectX12 Window!");
    window->RegisterNewClass();
    window->Initialize();

    //MSG msg = {};
    //while (msg.message != WM_QUIT)
    //{
    //    if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    //    {
    //        ::TranslateMessage(&msg);
    //        ::DispatchMessage(&msg);
    //    }
    //}

    return 0;
}