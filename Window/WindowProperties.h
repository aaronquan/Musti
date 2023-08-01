#pragma once

#include <windows.h>
#include <memory>

#include "../Geometry/Virtuals.h"
#include "Recorders/Updater.h"

using namespace std;
using namespace Eigen;

const unsigned int absoluteCoordinateMax = 65535;
const unsigned int maxDisplayDevices = 10;

class DisplayDevice {
private:
    unsigned int displayNumber;

    wstring displayName;
    //Vector2i pelsSize;
    //Array2i displayPosition;

    VirtualRectangle display;
    Array2f absoluteScale;
    //float scaleHeight;
    bool isMain;
    bool isActive;
    unsigned int displayFrequency;
public:
    DisplayDevice(unsigned int dn=0);
    bool requestDisplay();
    //DEVMODEW getDisplaySettings() const;
    //DISPLAY_DEVICEW getDisplayDevice() const;

    Vector2f getPelsSize() const;
    bool getIsActive() const;
    bool getIsMain() const;
    Array2f absoluteToMonitor(Array2f p) const;
    Array2i monitorToAbsolute(Array2f p) const;
    Array2i monitorToAbsolute(Array2i p) const;
};

class WindowDetails {
private:
    
    HWND* hwnd; 
    int resizeState;
    VirtualRectangle window;
    VirtualRectangle fullWindow;

    unsigned int frameRate;
    unsigned int frameTime; //in miliseconds

    vector<DisplayDevice> displays;
    DisplayDevice mainMonitor;

    unsigned int threadId;

    void initDisplayDevices();
    void setDefaultDetails();
public:
    WindowDetails();
    WindowDetails(HWND* h, unsigned int width=0, unsigned int height=0);

    void update();
    //WindowDetails(unsigned int width, unsigned int height, unsigned int fr = 60);
    void setHWND(HWND* h);
    void resize(int state, Vector2f dim);
    int getResizeState() const;

    static unsigned int frameRateToFrameTime(unsigned int fr);

    void setFrameRate(unsigned int fr);
    unsigned int getFrameRate() const;
    unsigned int getFrameTime() const;

    void updateThreadId();
    unsigned int getThreadId() const;

    Vector2f getDimensions() const;
    void setWindowPosition(Array2f position);
    Array2f getLeftTop() const;

    VirtualRectangle getWindowRectangle() const;

    Vector2f getFullDimensions() const;
    void newWindowPosition(Array2f position);

    Array2i toAbsolutePosition(Array2f position);
    Array2i toAbsolutePosition(Array2i position);
};

class ExternalWindowDetails {
private:
    //HWND* handle;
    unsigned int threadId;
    wstring title;

    VirtualRectangle windowRectangle;
public:
    ExternalWindowDetails();
    void update(HWND* hwnd);
    wstring getTitle() const;
    unsigned int getThreadId() const;
};

class ExternalWindows : public Updater {
private:
    struct WindowUpdateData{
        vector<ExternalWindowDetails>* windows;
        unsigned int currentThreadId;
        int* activeIndex;
        unsigned int index;
        WindowUpdateData(vector<ExternalWindowDetails>* w, unsigned int tid, int* ai)
        : windows(w), currentThreadId(tid), activeIndex(ai), index(0){};
    };
    vector<ExternalWindowDetails> windows;
    int activeIndex;
    //unique_ptr<ExternalWindowDetails> activeWindow;
    static bool __stdcall windowCallback(HWND hwnd, LPARAM lParam);
public:
    ExternalWindows();
    void update();
    vector<ExternalWindowDetails> getWindows() const;
    shared_ptr<ExternalWindowDetails> getActiveWindow() const;
};