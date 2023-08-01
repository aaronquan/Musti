#pragma once

#include "WindowProperties.h"

DisplayDevice::DisplayDevice(unsigned int dn) : displayNumber(dn), isMain(false){
};

bool DisplayDevice::requestDisplay() {
    DISPLAY_DEVICEW dd;
    dd.cb = sizeof(dd);
    bool hasDisplay = EnumDisplayDevicesW(nullptr, displayNumber, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
    if(hasDisplay){
        isActive = dd.StateFlags & DISPLAY_DEVICE_ACTIVE;
        if(isActive && dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP){
            outputDebugLine(dd.DeviceName);
            DEVMODEW dm;
            dm.dmSize = sizeof(dm);
            EnumDisplaySettingsW(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm);

            displayName = dd.DeviceName;
            displayFrequency = dm.dmDisplayFrequency;
            POINTL pos = dm.dmPosition;
            //displayPosition = Array2i(pos.x, pos.y);
            //pelsSize = Vector2i(dm.dmPelsWidth, dm.dmPelsHeight);
            display = VirtualRectangle(Array2f(pos.x, pos.y), Vector2f(dm.dmPelsWidth, dm.dmPelsHeight));
            float absCoordMax = static_cast<float>(absoluteCoordinateMax);
            absoluteScale = Array2f(absCoordMax / dm.dmPelsWidth, absCoordMax / dm.dmPelsHeight);
            isMain = dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE;
        }
    }
    return hasDisplay;
}
/*
DEVMODEW DisplayDevice::getDisplaySettings() const {
    return displaySettings;
}

DISPLAY_DEVICEW DisplayDevice::getDisplayDevice() const {
    return displayDevice;
}*/

Vector2f DisplayDevice::getPelsSize() const {
    return display.getDimensions();
}

bool DisplayDevice::getIsActive() const {
    return isActive;
}

bool DisplayDevice::getIsMain() const {
    return isMain;
}

Array2f DisplayDevice::absoluteToMonitor(Array2f p) const {
    return p/absoluteScale;
}

Array2i DisplayDevice::monitorToAbsolute(Array2f p) const {
    Array2f absolute = p * absoluteScale;
    return Array2i(round(absolute(0)), round(absolute(1)));
}

Array2i DisplayDevice::monitorToAbsolute(Array2i p) const {
    Array2f absolute = Array2f(p(0), p(1)) * absoluteScale;
    return Array2i(round(absolute(0)), round(absolute(1)));
}

WindowDetails::WindowDetails() : WindowDetails(nullptr, 0, 0) {};

WindowDetails::WindowDetails(HWND* h, unsigned int width, unsigned int height) : hwnd(h),
window(Vector2f(width, height)){
    setDefaultDetails();
};

//WindowDetails::WindowDetails(unsigned int width, unsigned int height, unsigned int fr) :
//    window(Vector2f(width, height)), resizeState(0), framerate(fr) {
//
//};

void WindowDetails::initDisplayDevices() {
    displays.clear();
    for (unsigned int i = 0; i < maxDisplayDevices; ++i) {
        DisplayDevice dd(i);
        if(!dd.requestDisplay()) break;
        if(dd.getIsMain()){
            mainMonitor = dd;
        }
        if(dd.getIsActive()) displays.push_back(dd);
    }
}

void WindowDetails::setDefaultDetails() {
    initDisplayDevices();
    for(DisplayDevice dd: displays){
        outputDebugLine(dd.getPelsSize());
    }
    if (hwnd) {
        if (displays.size() > 0) {
            //assuming main monitor
            Vector2f main = displays[0].getPelsSize();
            RECT winRect;
            GetWindowRect(*hwnd, &winRect);
            VirtualRectangle windowVR(Array2f(winRect.left, winRect.top), Array2f(winRect.right, winRect.bottom));
        }
        updateThreadId();
    }
}

void WindowDetails::setHWND(HWND* h) {
    hwnd = h;
    setDefaultDetails();
}

unsigned int WindowDetails::frameRateToFrameTime(unsigned int fr) {
    return static_cast<unsigned int>(floor(1000/static_cast<float>(fr)));
}

void WindowDetails::resize(int state, Vector2f dim) {
    resizeState = state;
    window.setDimensions(dim);

    RECT r;
    GetWindowRect(*hwnd, &r);
    fullWindow = VirtualRectangle(Array2f(r.left, r.top), Array2f(r.right, r.bottom));
}

int WindowDetails::getResizeState() const {
    return resizeState;
}

void WindowDetails::setFrameRate(unsigned int fr) {
    frameRate = fr;
    frameTime = frameRateToFrameTime(fr);
}

unsigned int WindowDetails::getFrameRate() const {
    return frameRate;
}
unsigned int WindowDetails::getFrameTime() const {
    return frameTime;
}

void WindowDetails::updateThreadId() {
    threadId = GetCurrentThreadId();
}

unsigned int WindowDetails::getThreadId() const {
    return threadId;
}

Vector2f WindowDetails::getDimensions() const {
    return window.getDimensions();
}

void WindowDetails::setWindowPosition(Array2f position) {
    window.setLeftTopPosition(position);
    RECT r;
    GetWindowRect(*hwnd, &r);
    fullWindow.setLeftTopPosition(Array2f(r.left, r.top));
}

Array2f WindowDetails::getLeftTop() const {
    return window.getLeftTop();
}

VirtualRectangle WindowDetails::getWindowRectangle() const {
    return window;
}

void WindowDetails::newWindowPosition(Array2f position) {
    setWindowPosition(position);
    Array2f dims = fullWindow.getDimensions();
    outputDebugLine(dims);
    SetWindowPos(*hwnd, HWND_TOP, position(0), position(1), dims(0), dims(1), SWP_SHOWWINDOW);
}

Array2i WindowDetails::toAbsolutePosition(Array2f position) {
    Array2i pos = mainMonitor.monitorToAbsolute(position);
    return pos;
}

Array2i WindowDetails::toAbsolutePosition(Array2i position) {
    Array2i pos = mainMonitor.monitorToAbsolute(position);
    return pos;
}

ExternalWindowDetails::ExternalWindowDetails() : threadId(0) {};

void ExternalWindowDetails::update(HWND* hwnd) {
    int length = GetWindowTextLengthW(*hwnd);
    wchar_t* buffer;
    buffer = new wchar_t[length + 1];
    memset(buffer, 0, (length + 1) * sizeof(wchar_t));

    GetWindowTextW(*hwnd, buffer, length + 1);
    wstring windowTitle = wstring(buffer);
    delete[] buffer;

    threadId = GetWindowThreadProcessId(*hwnd, nullptr);
    //outputDebugLine(windowTitle);
    title = windowTitle;
}

wstring ExternalWindowDetails::getTitle() const {
    return title;
}

unsigned int ExternalWindowDetails::getThreadId() const {
    return threadId;
}

ExternalWindows::ExternalWindows() : activeIndex(-1) {
    update();
};

bool __stdcall ExternalWindows::windowCallback(HWND hwnd, LPARAM l) {
    HWND active = GetForegroundWindow();
    unsigned int threadId = GetWindowThreadProcessId(active, nullptr);
    if (IsWindowVisible(hwnd)) {
        int length = GetWindowTextLength(hwnd);
        if (0 == length) return true;

        RECT winRect;
        GetWindowRect(hwnd, &winRect);
        VirtualRectangle windowVR(Array2f(winRect.left, winRect.top), Array2f(winRect.right, winRect.bottom));


        ExternalWindowDetails exd;
        exd.update(&hwnd);
        //outputDebugLine(to_string(l));
        WindowUpdateData* wud = (WindowUpdateData*)l;
        //vector<ExternalWindowDetails>* windows = (vector<ExternalWindowDetails>*)l;
        wud->windows->push_back(exd);
        if (exd.getThreadId() == wud->currentThreadId) {
            *wud->activeIndex = wud->index;
        }
        wud->index++;
    }

    return true;
}

void ExternalWindows::update() {
    HWND active = GetForegroundWindow();
    unsigned int threadId = GetWindowThreadProcessId(active, nullptr);
    windows.clear();
    WindowUpdateData wud(&windows, threadId, &activeIndex);
    EnumWindows((WNDENUMPROC)windowCallback, reinterpret_cast<uintptr_t>(&wud));

}

vector<ExternalWindowDetails> ExternalWindows::getWindows() const {
    return windows;
}

shared_ptr<ExternalWindowDetails> ExternalWindows::getActiveWindow() const {
    if (activeIndex != -1 && activeIndex < windows.size()) {
        return make_shared<ExternalWindowDetails>(windows[activeIndex]);
    }
    return nullptr;
}