#pragma once

#include <windows.h>
#include <dwrite.h>
#pragma comment(lib, "Dwrite")
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include "Util.h"


template <class DERIVED_TYPE>
class BaseWindow
{
public:
    static LRESULT CALLBACK WindowProc(HWND h, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE* pThis = NULL;
        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
            SetWindowLongPtr(h, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->hwnd = h;
        }
        else
        {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(h, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->handleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProcW(h, uMsg, wParam, lParam);
        }
    }

    BaseWindow() : hwnd(NULL) { }


    //create the window
    BOOL Create(
        LPCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = 100,
        int y = 100,
        int nWidth = 500,
        int nHeight = 500,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    ) {
        WNDCLASSW wc = { 0 };
        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = className();

        RegisterClassW(&wc);
        RECT rect = { x, y, x+nWidth, y+nHeight };
        AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);
        hwnd = CreateWindowExW(
            dwExStyle, className(), lpWindowName, dwStyle, rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top, hWndParent, hMenu, GetModuleHandle(NULL), this
        );

        return (hwnd ? TRUE : FALSE);
    }

    HWND Window() const { return hwnd; }

protected:

    virtual LPCWSTR  className() const = 0;
    virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND hwnd;
};

//A base window with direct2d capabilities and writing
class BaseDrawWriteWindow : public BaseWindow<BaseDrawWriteWindow> {
protected:
    ID2D1Factory* drawFactory;
    IDWriteFactory* writeFactory;
    ID2D1HwndRenderTarget* renderTarget;

    HRESULT createResources();
    void discardResources();
    void drawTestFrame();
public:
    BaseDrawWriteWindow();
    LPCWSTR className() const { return L"Base Draw Write Window"; }
    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};