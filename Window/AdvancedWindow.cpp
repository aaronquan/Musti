#pragma once
#include "AdvancedWindow.h"

#include <memory>


BaseApp::BaseApp() : brushController(nullptr), 
fontController(nullptr), inputController(nullptr),
windowDetails(nullptr), timerManager(nullptr),
mouseAutoInput(make_shared<MouseAutoInput>()),
autoInput(make_shared<AutoInput>())
{

}

VirtualRectangle BaseApp::updateObjects() {
    return windowDetails->getWindowRectangle();
}

void BaseApp::addControllers(shared_ptr<BrushController> bc, shared_ptr<FontController> fc, 
shared_ptr<InputController> ic) {
    brushController = bc;
    fontController = fc;
    inputController = ic;
    mouseAutoInput->addMouseController(ic);
    autoInput->addInputController(ic);

}

void BaseApp::addResources(shared_ptr<WindowDetails> wd, shared_ptr<TimerManager> tm,
    shared_ptr<UpdaterCollection> fu) {
    windowDetails = wd;
    timerManager = tm;
    frameUpdater = fu;
    mouseAutoInput->addWindowDetails(wd);
    autoInput->addWindowDetails(wd);
}

void BaseApp::addTimer(function<void()> func, unsigned int interval) {

}

//void BaseApp::handleLeftMouseDown(Array2f position){};

//void handleMouseMove(Vector2f movement)

BOOL CALLBACK windowsProc(_In_ HWND hwnd, _In_ LPARAM lParam);

BOOL CALLBACK windowsProc(_In_ HWND hwnd, _In_ LPARAM lParam) {
    int length = ::GetWindowTextLength(hwnd);
    if (0 == length) return TRUE;
    if(IsWindowVisible(hwnd)){
        CHAR* buffer;
        buffer = new TCHAR[length + 1];
        memset(buffer, 0, (length + 1) * sizeof(CHAR));

        GetWindowText(hwnd, buffer, length + 1);
        string windowTitle = string(buffer);
        delete[] buffer;

        outputDebugLine(windowTitle);
    }

    return TRUE;
}

void ControllersWindow::getWindows() {
    EnumWindows(windowsProc, 0);
}

ControllersWindow::ControllersWindow() : ControllersWindow(500, 500){};

ControllersWindow::ControllersWindow(unsigned int width, unsigned int height) : BaseDrawWriteWindow(),
brushController(nullptr), fontController(nullptr), inputController(make_shared<InputController>()),
windowDetails(make_shared<WindowDetails>(&hwnd, width, height))
{
    //timerManager = make_shared<TimerManager>(hwnd);
};

HRESULT ControllersWindow::createResources()
{
    //getWindows();
    HRESULT hr;
	hr = BaseDrawWriteWindow::createResources();
    brushController = make_shared<BrushController>(renderTarget);
    fontController = make_shared<FontController>(writeFactory);
    inputController = make_shared<InputController>();
    //geometryDrawer = make_shared<GeometryDrawer>(drawFactory);
    SimpleGeometry::createResources(drawFactory);
    VirtualTriangle::createResources(drawFactory);

    return hr;
}

void ControllersWindow::discardResources() {
	BaseDrawWriteWindow::discardResources();
	brushController->release();
    fontController->release();
    //geometryDrawer->release();
}

void ControllersWindow::drawFrame() {};

void ControllersWindow::handleKeyDown(WPARAM wParam, LPARAM lParam) {
    char key = inputController->onKeyPress(wParam, lParam);
}

void ControllersWindow::handleKeyUp(WPARAM wParam, LPARAM lParam) {
    outputDebugLine(wParam);
    char key = inputController->onKeyRelease(wParam, lParam);
}

void ControllersWindow::handleMouseMove(WPARAM wParam, LPARAM lParam) {
    inputController->onMouseMove(wParam, lParam);
}

void ControllersWindow::handleLeftMouseDown(WPARAM wParam, LPARAM lParam) {
    inputController->onLeftMouseDown(wParam, lParam);
}

void ControllersWindow::handleLeftMouseUp(WPARAM wParam, LPARAM lParam) {
    inputController->onLeftMouseUp(wParam, lParam);
}

void ControllersWindow::handleRightMouseDown(WPARAM wParam, LPARAM lParam) {
    
}

void ControllersWindow::handleRightMouseUp(WPARAM wParam, LPARAM lParam) {

}

void ControllersWindow::handleWindowMove(LPARAM lParam) {
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);
    //windowCoordinates = Array2f(xPos, yPos);
    windowDetails->setWindowPosition(Array2f(xPos, yPos));
}

void ControllersWindow::handleWindowSize(WPARAM wParam, LPARAM lParam) {
    int width = GET_X_LPARAM(lParam);
    int height = GET_Y_LPARAM(lParam);

    RECT r;
    GetWindowRect(hwnd, &r);
    Array2i dims1(r.right-r.left, r.bottom-r.top);
    Array2i dims2(width, height);
    outputDebugLine(dims1);
    outputDebugLine(dims2);

    windowDetails->resize(wParam, Vector2f(width, height));
}

void ControllersWindow::handleTimer(WPARAM wParam, LPARAM lParam){};

bool ControllersWindow::Create(LPCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle,
    int x, int y, HWND hWndParent, HMENU hMenu) {
    Vector2f windowDimensions = windowDetails->getDimensions();
    windowDetails->setWindowPosition(Array2f(x, y));
    return BaseWindow::Create(lpWindowName, dwStyle, dwExStyle, x, y, 
    windowDimensions(0), windowDimensions(1), hWndParent, hMenu);
}

LRESULT ControllersWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_CREATE:
        createResources();
        return 0;
    case WM_PAINT:
        drawFrame();
        return 0;
    case WM_MOUSEMOVE:
        handleMouseMove(wParam, lParam);
        return 0;
    case WM_LBUTTONDOWN:
        handleLeftMouseDown(wParam, lParam);
        return 0;
    case WM_LBUTTONUP:
        handleLeftMouseUp(wParam, lParam);
        return 0;
    case WM_RBUTTONDOWN:
        handleRightMouseDown(wParam, lParam);
        return 0;
    case WM_RBUTTONUP:
        handleRightMouseUp(wParam, lParam);
        return 0;
    case WM_KEYDOWN: 
        handleKeyDown(wParam, lParam);
        return 0;
    case WM_KEYUP:
        handleKeyUp(wParam, lParam);
        return 0;
    case WM_MOVE:
        handleWindowMove(lParam);
        return 0;
    case WM_SIZE:
        handleWindowSize(wParam, lParam);
        return 0;
    case WM_TIMER: {
        handleTimer(wParam, lParam);
        return 0;
        }
    /*
    case WM_SHOWWINDOW: // runs only on start up
        outputDebugLine("SHOWME");
        return 0;
    case WM_ACTIVATEAPP: // runs every time window is set off active or activated
        outputDebugLine("ACTIVATE");
        return 0;
    */
    case WM_CLOSE:
        discardResources();
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}

//AppWindow::AppWindow(unsigned int width, unsigned int height) : AppWindow(nullptr, width, height) {}

AppWindow::AppWindow(unique_ptr<BaseApp> ba, unsigned int width, unsigned int height, unsigned int fr) 
: ControllersWindow(width, height), frameUpdater(make_shared<UpdaterCollection>()) {
    linkApp(move(ba));
    timerManager = make_shared<TimerManager>(&hwnd);
    windowDetails->setFrameRate(fr);
}

void AppWindow::onFrameRefresh() {
    frameUpdater->update();
    VirtualRectangle updateRegion = app->updateObjects();
    InvalidateRect(hwnd, nullptr, TRUE);
}

void AppWindow::linkApp(unique_ptr<BaseApp>& ba) {
    if (!ba) {
        throw exception("App is null");
    }
    app = move(ba);
    //app->addTimerMan
}
HRESULT AppWindow::createResources() {
    HRESULT hr = ControllersWindow::createResources();
    if (SUCCEEDED(hr)) {
        brushController->addSolidBrush("green", D2D1::ColorF(0, 1.0f, 0));
        app->addControllers(brushController, fontController, inputController);
        app->addResources(windowDetails, timerManager, frameUpdater);
        
        app->createResources();
    }
    function<void()> frameRefresh = [this]() {
        onFrameRefresh();
    };
    //unsigned int frametime = floor(1000 / framerate);
    //addTimer(f2, frametime);

    timerManager->addTimer(frameRefresh, windowDetails->getFrameTime());

    return hr;
}

void AppWindow::addTimer(function<void()> func, unsigned int interval) {
    timerManager->addTimer(func, interval);
}

void AppWindow::handleKeyDown(WPARAM wParam, LPARAM lParam) {
    //ControllersWindow::handleKeyDown(wParam, lParam);
    char key = inputController->onKeyPress(wParam, lParam);
    app->handleKeyDown(wParam, key);
}
void AppWindow::handleKeyUp(WPARAM wParam, LPARAM lParam) {
    //ControllersWindow::handleKeyUp(wParam, lParam);
    char key = inputController->onKeyRelease(wParam, lParam);
    app->handleKeyUp(wParam, key);
}

void AppWindow::handleMouseMove(WPARAM wParam, LPARAM lParam) {
    ControllersWindow::handleMouseMove(wParam, lParam);
    app->handleMouseMove(inputController->getMousePosition(), inputController->getMouseMovement());
}

void AppWindow::handleLeftMouseDown(WPARAM wParam, LPARAM lParam) {
    ControllersWindow::handleLeftMouseDown(wParam, lParam);
    app->handleLeftMouseDown(inputController->getMousePosition());
}

void AppWindow::handleLeftMouseUp(WPARAM wParam, LPARAM lParam) {
    ControllersWindow::handleLeftMouseUp(wParam, lParam);
    app->handleLeftMouseUp(inputController->getMousePosition());
}

void AppWindow::handleRightMouseDown(WPARAM wParam, LPARAM lParam) {
    ControllersWindow::handleRightMouseDown(wParam, lParam);
    app->handleRightMouseDown(inputController->getMousePosition());
}

void AppWindow::handleRightMouseUp(WPARAM wParam, LPARAM lParam) {
    ControllersWindow::handleRightMouseUp(wParam, lParam);
    app->handleRightMouseUp(inputController->getMousePosition());
}

void AppWindow::handleWindowMove(LPARAM lParam) {
    ControllersWindow::handleWindowMove(lParam);
    app->handleWindowMove();
}

void AppWindow::handleWindowSize(WPARAM wParam, LPARAM lParam) {
    ControllersWindow::handleWindowSize(wParam, lParam);
    app->handleWindowSize();
}

void AppWindow::handleTimer(WPARAM wParam, LPARAM lParam) {
    timerManager->runTimers(wParam, lParam);
}

void AppWindow::drawFrame() {
    renderTarget->BeginDraw();
    renderTarget->Clear(); // needed invalidate rect only clears 'background'
    app->drawFrame(renderTarget);
    ValidateRect(hwnd, nullptr);
    renderTarget->EndDraw();
}

void AppWindow::discardResources() {

}

//WindowDetails::WindowDetails() : WindowDetails(0, 0, 60){};
//
//WindowDetails::WindowDetails(unsigned int width, unsigned int height, unsigned int fr) : 
//window(Vector2f(width, height)), resizeState(0), framerate(fr) {
//    
//};
//
//unsigned int WindowDetails::framerateToFrametime(unsigned int fr) {
//    return static_cast<unsigned int>(floor(static_cast<float>(fr)/1000));
//}
//
//void WindowDetails::resize(int state, Vector2f dim) {
//    resizeState = state;
//    window.setDimensions(dim);
//}
//
//int WindowDetails::getResizeState() const {
//    return resizeState;
//}
//
//void WindowDetails::setFramerate(unsigned int fr) {
//    framerate = fr;
//    frametime = framerateToFrametime(fr);
//}
//
//unsigned int WindowDetails::getFramerate() const{
//    return framerate;
//}
//unsigned int WindowDetails::getFrametime() const{
//    return frametime;
//}
//
//Vector2f WindowDetails::getDimensions() const {
//    return window.getDimensions();
//}
//
//void WindowDetails::setWindowPosition(Array2f position) {
//    window.setLeftTopPosition(position);
//}
//
//Vector2f WindowDetails::getLeftTop() const {
//    return window.getLeftTop();
//}
//
//VirtualRectangle WindowDetails::getWindowDetails() const {
//    return window;
//}