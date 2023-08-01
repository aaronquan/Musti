#pragma once

#include <windows.h>
#include <windowsx.h>
#include <memory>
#include <dwrite.h>
#pragma comment(lib, "Dwrite")
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "BaseWindow.h"
#include "WindowProperties.h"

#include "Util.h"
#include "Controllers/BrushController.h"
#include "Controllers/FontController.h"
#include "Controllers/InputController.h"

#include "Recorders/TimerManager.h"
#include "Recorders/Updater.h"

#include "../InterfaceElements/BasicElements.h"

#include "../Geometry/DrawShapes.h"
#include "../Geometry/Containers.h"
#include "../Geometry/Virtuals.h"
#include "../Geometry/Util.h"

using namespace std;
using namespace Eigen;

//class WindowDetails;

class BaseApp {
protected:
    shared_ptr<BrushController> brushController;
    shared_ptr<FontController> fontController;
    shared_ptr<InputController> inputController;
    shared_ptr<WindowDetails> windowDetails;
    shared_ptr<TimerManager> timerManager;
    shared_ptr<MouseAutoInput> mouseAutoInput;
    shared_ptr<UpdaterCollection> frameUpdater;
    shared_ptr<AutoInput> autoInput;
public:
    BaseApp();

    void addControllers(
    shared_ptr<BrushController> bc, 
    shared_ptr<FontController> fc, 
    shared_ptr<InputController> ic
    //shared_ptr<WindowDetails> wd
    );
    void addResources(shared_ptr<WindowDetails> wd, shared_ptr<TimerManager> tm,
    shared_ptr<UpdaterCollection> fu);
    void addTimer(function<void()> func, unsigned int interval);

    virtual void createResources() = 0;
    virtual VirtualRectangle updateObjects();
    virtual void drawFrame(ID2D1HwndRenderTarget* rt) = 0;

    //events (params may not be required relevant controllers hold the information
    virtual void handleKeyDown(unsigned int keyCode, char c){};
    virtual void handleKeyUp(unsigned int keyCode, char c){};

    virtual void handleLeftMouseDown(Array2f position){};
    virtual void handleLeftMouseUp(Array2f position) {};
    virtual void handleRightMouseDown(Array2f position) {};
    virtual void handleRightMouseUp(Array2f position) {};
    virtual void handleMouseMove(Array2f position, Vector2f movement){};
    virtual void handleWindowMove(){};
    virtual void handleWindowSize(){};
};

class ControllersWindow : public BaseDrawWriteWindow {
protected:
    shared_ptr<BrushController> brushController;
    shared_ptr<FontController> fontController;
    shared_ptr<InputController> inputController;

    shared_ptr<WindowDetails> windowDetails;
    //shared_ptr<GeometryDrawer> geometryDrawer;

    unsigned int currentId;

    void getWindows();

    virtual HRESULT createResources();
    virtual void discardResources();
    virtual void drawFrame();
    virtual void handleKeyDown(WPARAM wParam, LPARAM lParam);
    virtual void handleKeyUp(WPARAM wParam, LPARAM lParam);
    virtual void handleMouseMove(WPARAM wParam, LPARAM lParam);
    virtual void handleLeftMouseDown(WPARAM wParam, LPARAM lParam);
    virtual void handleLeftMouseUp(WPARAM wParam, LPARAM lParam);
    virtual void handleRightMouseDown(WPARAM wParam, LPARAM lParam);
    virtual void handleRightMouseUp(WPARAM wParam, LPARAM lParam);
    virtual void handleWindowMove(LPARAM lParam);
    virtual void handleWindowSize(WPARAM wParam, LPARAM lParam);
    virtual void handleTimer(WPARAM wParam, LPARAM lParam);

public:
    ControllersWindow();
    ControllersWindow(unsigned int width, unsigned int height);
    bool Create(LPCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0, 
    int x = 100, int y = 100, HWND hWndParent = 0, HMENU hMenu = 0);
    LPCWSTR className() const { return L"Controllers Window"; }
    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class AppWindow : public ControllersWindow {
protected:
    unique_ptr<BaseApp> app;

    //unsigned int frameRate;

    shared_ptr<TimerManager> timerManager;
    shared_ptr<UpdaterCollection> frameUpdater;
    //unordered_map<unsigned int, function<void()>> timerFunctions;
    //unsigned int timerIdReference;
    void onFrameRefresh();
public:
    //AppWindow(unsigned int width=500, unsigned int height=500); 
    AppWindow(unique_ptr<BaseApp> app, unsigned int width=500, unsigned int height=500, unsigned int framerate=60);
    void linkApp(unique_ptr<BaseApp>&);
    HRESULT createResources();

    void addTimer(function<void()> func, unsigned int interval);
    //void runTimers(WPARAM wParam, LPARAM lParam);

    void handleKeyDown(WPARAM wParam, LPARAM lParam);
    void handleKeyUp(WPARAM wParam, LPARAM lParam);

    void handleMouseMove(WPARAM wParam, LPARAM lParam);
    void handleLeftMouseDown(WPARAM wParam, LPARAM lParam);
    void handleLeftMouseUp(WPARAM wParam, LPARAM lParam);
    void handleRightMouseDown(WPARAM wParam, LPARAM lParam);
    void handleRightMouseUp(WPARAM wParam, LPARAM lParam);
    void handleWindowMove(LPARAM lParam);
    void handleWindowSize(WPARAM wParam, LPARAM lParam);
    void handleTimer(WPARAM wParam, LPARAM lParam);

    //VirtualRectangle updateObjects();
    void drawFrame();

    void discardResources();
};

class BaseAppWindow : public ControllersWindow {
protected:
    
public:
    //virtual void handleLeftMouseDown(Array2f arr);
};

/*
class WindowDetails {
private:
    int resizeState;
    VirtualRectangle window;
    unsigned int framerate;
    unsigned int frametime; //in miliseconds
public:
    WindowDetails();
    WindowDetails(unsigned int width, unsigned int height, unsigned int fr=60);
    void resize(int state, Vector2f dim);
    int getResizeState() const;

    static unsigned int framerateToFrametime(unsigned int fr);

    void setFramerate(unsigned int fr);
    unsigned int getFramerate() const;
    unsigned int getFrametime() const;

    Vector2f getDimensions() const;
    void setWindowPosition(Array2f position);
    Vector2f getLeftTop() const;

    VirtualRectangle getWindowDetails() const;

};
*/