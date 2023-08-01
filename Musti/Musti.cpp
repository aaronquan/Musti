// Engine2DG.cpp : Defines the entry point for the application.
// https://www.glfw.org/ alternative to create window (open source)

#include "Musti.h"
#include <Eigen/Dense>
//#include <Add.h>

using namespace std;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR pCmdLine, int nCmdShow)
{
    //ControllersWindow win;
    //CustomTest win(600, 600);
    //AppTest at;
    //unique_ptr<AppTest> at = make_unique<AppTest>();
    AppWindow win(make_unique<AppTest>(), 800, 700);
    if (!win.Create(L"Musti", WS_OVERLAPPEDWINDOW, 0))
    {
        return 0;
    }
    ShowWindow(win.Window(), nCmdShow);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}