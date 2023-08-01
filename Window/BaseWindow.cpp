#pragma once
#include "BaseWindow.h"

BaseDrawWriteWindow::BaseDrawWriteWindow() : drawFactory(NULL), writeFactory(NULL), renderTarget(NULL) {};

HRESULT BaseDrawWriteWindow::createResources() {
    HRESULT hr;
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &drawFactory);
    if (SUCCEEDED(hr)) {
        RECT rc;
        GetClientRect(hwnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
        hr = drawFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &renderTarget);
        

        //put write controller in fontController / rename writecontroller?
        if (SUCCEEDED(hr)) {
            hr = DWriteCreateFactory(
                DWRITE_FACTORY_TYPE_SHARED,
                __uuidof(writeFactory),
                reinterpret_cast<IUnknown**>(&writeFactory)
            );
        }
    }
    return hr;
};

void BaseDrawWriteWindow::discardResources() {
    safeRelease(&drawFactory);
    safeRelease(&writeFactory);
    safeRelease(&renderTarget);
}

void BaseDrawWriteWindow::drawTestFrame() {
    //to clear, have lower level call
    renderTarget->BeginDraw();
    renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
    renderTarget->EndDraw();
}

LRESULT BaseDrawWriteWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_CREATE:
        createResources();
        return 0;
    case WM_CLOSE: {
            discardResources();
            DestroyWindow(hwnd);
        }
        return 0;
    case WM_PAINT:
        drawTestFrame();
        return 0;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}
