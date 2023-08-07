#pragma once

#include "CustomWindow.h"

CustomTest::CustomTest() : CustomTest(600, 600) {};

CustomTest::CustomTest(int width, int height) : ControllersWindow(width, height), app(nullptr) {};

HRESULT CustomTest::createResources()
{
    
    HRESULT hr;
    hr = ControllersWindow::createResources();

    hr = brushController->addSolidBrush("testBrush", D2D1::ColorF(1.0f, 1.0f, 0));
    hr = brushController->addSolidBrush("red", D2D1::ColorF(1.0f, 0, 0));
    hr = brushController->addSolidBrush("rand", D2D1::ColorF(1.0f, 0.5f, 0.5f));

    fontController->addFont("label", L"Verdana", 20);
    fontController->setTextAlignment("label", DWRITE_TEXT_ALIGNMENT_CENTER);

    hr = fontController->addFont("mess", L"Verdana", 12);
    fontController->setTextAlignment("mess", DWRITE_TEXT_ALIGNMENT_CENTER);

    app = make_unique<MyApp>();
    app->addRectangle(DrawRectangle(Array2f(0, 0), Vector2f(100, 100), brushController->getBrush("testBrush")));
    shared_ptr<TextRectangleButton> rb = make_shared<TextRectangleButton>(Array2f(200, 50), Vector2f(100, 60), 
    brushController->getBrush("rand"), writeFactory,
    fontController->getFont("label"), brushController->getBrush("red"));
    rb->setLabel(L"Click ME");
    rb->setFunction([this]() { 
        outputDebugLine("Hello World"); 
        app->changeColour();
        app->startRecord();
    });
    app->setBrushes(brushController->getBrush("red"), brushController->getBrush("rand"));
    app->addButton(rb);

    shared_ptr<TextRectangleButton> end =  make_shared<TextRectangleButton>(Array2f(400, 50), Vector2f(100, 60), 
    brushController->getBrush("rand"), writeFactory,
        fontController->getFont("label"), brushController->getBrush("red"));
    end->setLabel(L"END");
    end->setFunction([this]() {
        app->newGoal();
        //app->endRecord();
    });
    app->addButton(end, true);
    
    shared_ptr<RectangleButton> b = make_shared<RectangleButton>(Array2f(100, 100), Vector2f(50, 50), brushController->getBrush("rand"));

    app->addB(b);
    b->setFunction([]() {
        outputDebugLine("BUT2");
    });
    //window transparency
    //SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    //SetLayeredWindowAttributes(hwnd, 0, (255 * 70) / 100, LWA_ALPHA);

    return hr;
}

void CustomTest::discardResources() {
    ControllersWindow::discardResources();
}

void CustomTest::drawFrame() {
    Array2f mouse = inputController->getMousePosition();
    renderTarget->BeginDraw();
    renderTarget->Clear();
    app->draw(renderTarget);
    renderTarget->EndDraw();
}

void CustomTest::handleMouseMove(WPARAM wParam, LPARAM lParam) {
    ControllersWindow::handleMouseMove(wParam, lParam);
    Vector2f movement = inputController->getMouseMovement();
    app->handleMouseMove(movement);
}

void CustomTest::handleLeftMouseDown(WPARAM wParam, LPARAM lParam) {
    ControllersWindow::handleLeftMouseDown(wParam, lParam);
    app->handleMouseClick(inputController->getMousePosition());
}

void CustomTest::handleLeftMouseUp(WPARAM wParam, LPARAM lParam) {
    ControllersWindow::handleLeftMouseUp(wParam, lParam);
    app->handleMouseUp(inputController->getMousePosition());
}

void CustomTest::handleRightMouseDown(WPARAM wParam, LPARAM lParam) {
    ControllersWindow::handleRightMouseDown(wParam, lParam);
}

LRESULT CustomTest::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return ControllersWindow::handleMessage(uMsg, wParam, lParam);
}

MyApp::MyApp() : Canvas(), b1(nullptr), b2(nullptr), isB1(true), endButton(nullptr){};

void MyApp::addRectangle(DrawRectangle dr) {
    rectangle = dr;
    ds = DraggableShape(make_shared<DrawRectangle>(dr));
}

void MyApp::addButton(TextRectangleButton mb, bool isEnd) {
    /*if(!isEnd){
        myButton = make_shared<TextRectangleButton>(mb);
    }
    else {
        endButton = make_shared<TextRectangleButton>(mb);
    }*/
    addButton(make_shared<TextRectangleButton>(mb), isEnd);
}

void MyApp::addButton(shared_ptr<TextRectangleButton> mb, bool isEnd) {
    if (!isEnd) {
        myButton = mb;
    }
    else {
        endButton = mb;
    }
}

void MyApp::addB(shared_ptr<Button> b) {
    but = b;
}

void MyApp::handleMouseMove(Vector2f movement) {
    ds.move(movement);
}

void MyApp::handleMouseClick(Array2f point) {
    ds.handleMouseDown(point);
    myButton->activate(point);
    endButton->activate(point);
    but->activate(point);
}

void MyApp::handleMouseUp(Array2f point) {
    ds.handleMouseUp();
}

void MyApp::setBrushes(ID2D1Brush* a, ID2D1Brush* b){
    b1 = a; b2 = b;
}

void MyApp::changeColour() {
    if(b1 && b2){
        if (isB1) {
            rectangle.setBrush(b2);
            isB1 = false;
        }else{
            rectangle.setBrush(b1);
            isB1 = true;
        }
    }
}

void MyApp::startRecord() {
    
}

void MyApp::newGoal() {
    int x = rand() % 400;
    int y = rand() % 400;
    endButton->setPosition(Array2f(x, y));
}

//void setDraggable(DraggableShape d)

void MyApp::draw(ID2D1HwndRenderTarget* rt) {
    drawObjects.clear();
    drawObjects.push_back(make_shared<DrawRectangle>(rectangle));
    drawObjects.push_back(make_shared<DraggableShape>(ds));
    drawObjects.push_back(myButton);
    drawObjects.push_back(endButton);
    //outputDebugLine(to_string(drawObjects.size()));
    Canvas::draw(rt);
    but->draw(rt);
}

AppTest::AppTest() 
: numClicked(0)
{}

void AppTest::test() {
    outputDebugLine("TESTING");
}

void AppTest::clickMessage() {
    dlm4.setMessage(to_wstring(numClicked));
}

LRESULT CALLBACK keyHookProc(
    int nCode,
    WPARAM wParam,
    LPARAM lParam
)
{

    outputDebugLine("Key Pressed");
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK mouseHookProc(
    int nCode,
    WPARAM wParam,
    LPARAM lParam
) {

    outputDebugLine("MOUSEY");
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void AppTest::createResources() {
    //brushController->addSolidBrush("green", D2D1::ColorF(0, 1.0f, 0));
    HRESULT hr;
    hr = brushController->addSolidBrush("green", D2D1::ColorF(0, 1.0f, 0));
    hr = brushController->addSolidBrush("red", D2D1::ColorF(1.0f, 0, 0));
    hr = brushController->addSolidBrush("blue", D2D1::ColorF(0, 0, 1.0f));
    fontController->addFont("label", L"Verdana", 20);
    fontController->addFont("mess", L"Verdana", 12);

    mouseRec = make_shared<MouseRecorder>(inputController, timerManager);
    mousePlayer = make_shared<MouseRecordPlayer>(timerManager, mouseAutoInput);
    mousePlayer->setMouseRecorder(mouseRec);

    inputRecorder = make_shared<InputRecorder>();
    inputPlayer = make_shared<InputRecordPlayer>(autoInput);
    //inputPlayer->changeRecord(inputRecorder);

    externalWindows = make_shared<ExternalWindows>();
    frameUpdater->addUpdater(externalWindows);
    //externalWindows->update();
    vector<ExternalWindowDetails> wins = externalWindows->getWindows();
    for (ExternalWindowDetails win : wins) {
        outputDebugLine(win.getTitle());
    }
    shared_ptr<ExternalWindowDetails> main = externalWindows->getActiveWindow();
    outputDebugLine(main->getTitle());

    //rect = DrawRectangle(Array2f(5, 125), Vector2f(400, 225), brushController->getBrush("blue"));
    dlm = DrawLabelledMessage(Array2f(0, 0), Vector2f(100, 100), brushController->getBrush("blue"), fontController->getWriteFactory(),
    fontController->getFont("label"), fontController->getFont("mess"), brushController->getBrush("green"), brushController->getBrush("red"));
    dlm.setLabel(L"Region");

    dlm2 = DrawLabelledMessage(Array2f(100, 0), Vector2f(100, 100), brushController->getBrush("green"), fontController->getWriteFactory(),
        fontController->getFont("label"), fontController->getFont("mess"), brushController->getBrush("blue"), brushController->getBrush("red"));
    dlm2.setLabel(L"Mouse");

    dlm3 = DrawLabelledMessage(Array2f(200, 0), Vector2f(100, 100), brushController->getBrush("green"), fontController->getWriteFactory(),
        fontController->getFont("label"), fontController->getFont("mess"), brushController->getBrush("blue"), brushController->getBrush("red"));
    dlm3.setLabel(L"Current Window");

    function<void()> f = [this]() {
        POINT p;
        GetCursorPos(&p);
        Array2f pt(p.x, p.y);
        Array2i absPos = windowDetails->toAbsolutePosition(pt);
        //dlm3.setMessage(array2iToWString(absPos));
        shared_ptr<ExternalWindowDetails> main = externalWindows->getActiveWindow();
        //outputDebugLine(main->getTitle());
        dlm3.setMessage(main->getTitle());
    };
    timerManager->addTimer(f, windowDetails->getFrameTime());

    dlm4 = DrawLabelledMessage(Array2f(350, 0), Vector2f(50, 50), brushController->getBrush("green"), fontController->getWriteFactory(),
        fontController->getFont("mess"), fontController->getFont("mess"), brushController->getBrush("blue"), brushController->getBrush("red"));
    dlm4.setLabel(L"Clicks");
    clickMessage();

    playButton = make_shared<TextRectangleButton>(Array2f(300, 0), Vector2f(50,50), brushController->getBrush("red"),
        fontController->getWriteFactory(), fontController->getFont("label"), brushController->getBrush("blue")
    );
    playButton->setLabel(L"Play");
    playButton->setFunction([this]() {
        playPause();
    });
    bm.addButton(playButton);
    start = make_shared<TextRectangleButton>(Array2f(120, 150), Vector2f(50, 50), brushController->getBrush("red"),
        fontController->getWriteFactory(), fontController->getFont("label"), brushController->getBrush("blue")
    );
    bm.addButton(start);
    start->setLabel(L"Start");
    start->setFunction([this]() {
        inputRecorder->start();
        //mouseRec->start(windowDetails->getFrameTime());
    });
    end = make_shared<TextRectangleButton>(Array2f(400, 340), Vector2f(50, 50), brushController->getBrush("red"),
        fontController->getWriteFactory(), fontController->getFont("label"), brushController->getBrush("blue")
    );
    end->setLabel(L"End");
    end->setFunction([this]() {
        inputRecorder->end();
        inputPlayer->setRecord(inputRecorder->getRecordStream());
    });
    bm.addButton(end);

    //frameUpdater->addUpdater(mouseRec);
    //frameUpdater->addUpdater(mousePlayer);

    //shared_ptr<VirtualRectangle> vs = make_shared<VirtualRectangle>(Array2f(150, 100), Vector2f(100, 100));
    //drawShapeVir = DrawShapeVir(vs, brushController->getBrush("blue"));

    connRectangle = make_shared<VirtualRectangle>(Array2f(0, 0), Vector2f(20, 40));
    connRectangle->setBrush(brushController->getBrush("green"));
    dragRect = make_shared<VirtualRectangle>(Array2f(150, 100), Vector2f(100, 100));
    //ds = DraggableShape(vs, brushController->getBrush("red"));
    //ds = DraggableShape(make_shared<DrawRectangle>(Array2f(150, 100), Vector2f(100, 100), brushController->getBrush("green")));
    //shared_ptr<VirtualRectangle> vr = dynamic_pointer_cast<VirtualRectangle>(ds.toVirtualObject());
    //vr->setLeftTopPosition(Array2f(300, 300));

    //shared_ptr<DrawRectangleVir> drv = make_shared<DrawRectangleVir>(vs, brushController->getBrush("blue"));

    frameUpdater->addUpdater(inputRecorder);
    frameUpdater->addUpdater(inputPlayer);
    
    /*HHOOK hookHandle = SetWindowsHookExA(WH_KEYBOARD, (HOOKPROC)keyHookProc, nullptr, GetCurrentThreadId());
    HHOOK hookHandle2 = SetWindowsHookExA(WH_MOUSE, (HOOKPROC)mouseHookProc, nullptr, GetCurrentThreadId());
    if (hookHandle) {
        outputDebugLine("HOOK CREATE");
    }*/

    //nds = NewDragShape(make_unique<VirtualRectangle>(connRectangle), brushController->getBrush("blue"));
    nds = NewDragShape(dragRect, brushController->getBrush("blue"));
    //dragRect = make_shared<VirtualRectangle>(connRectangle);
    //dragRect->
    list = DropDownList(Array2f(0,240), Vector2f(200, 20), brushController->getBrush("green"), fontController->getWriteFactory(),
        fontController->getFont("label"), brushController->getBrush("blue"));
    list.setSelected(L"None");
    //list.addButton(L"HI", [](){});
    //list.addButton(L"BYE", [](){});
    vector<ExternalWindowDetails> winds = externalWindows->getWindows();
    for (ExternalWindowDetails win : winds) {
        list.addButton(win.getTitle());
    }

    vector<Array2f> v;
    v.push_back(Array2f(100, 100));
    v.push_back(Array2f(200, 250));
    sg = SimpleGeometry(v, brushController->getBrush("blue"));

    p1 = Array2f(240, 300);
    p2 = Array2f(280, 350);
    line_sta = make_shared<VirtualCircle>(p1, 5, 5, brushController->getBrush("green"));
    line_end = make_shared<VirtualCircle>(p2, 5, 5, brushController->getBrush("red"));
    virt_line = VirtualLine(p1, p2, brushController->getBrush("blue"));
    //dl = DrawLine(p1, p2, brushController->getBrush("blue"));
    circ_sta = NewDragShape(line_sta, brushController->getBrush("green"));
    circ_end = NewDragShape(line_end, brushController->getBrush("green"));

    test_tri = VirtualTriangle(p1, p2, Array2f(200, 350), brushController->getBrush("red"));

    float div = 5;
    Vector2f monDim = windowDetails->getDisplays()[0].getPelsSize()/div;

    vector<DisplayDevice> devices = windowDetails->getDisplays();
    Array2f stPt = Array2f(50, 400);
    Array2f monPt = stPt;
    for (DisplayDevice dd : devices) {
        Vector2f monDim = dd.getPelsSize() / div;
        monitors.push_back(VirtualRectangle(monPt, monDim, brushController->getBrush("blue")));
        monPt = Array2f(monPt(0)+monDim(0), monPt(1));
    }
    small_window = make_shared<VirtualRectangle>(Array2f(), Vector2f(), brushController->getBrush("green"));
    list.setFunction([this, stPt, div](unsigned int i) {
        ExternalWindowDetails ewd = externalWindows->getWindow(i);
        VirtualRectangle wr = ewd.getWindowRectangle();
        //selectedWindow = ewd;
        small_window->setDimensions(wr.getDimensions()/div);
        small_window->setLeftTopPosition((wr.getLeftTop()/div) + stPt);
    });

    function<void()> externWindowsUpdate = [this, stPt, div]() {
        if (GetActiveWindow() != GetForegroundWindow()) {
            list.clearList();
            small_windows.clear();
            vector<ExternalWindowDetails> winds = externalWindows->getWindows();
            for (ExternalWindowDetails win : winds) {
                list.addButton(win.getTitle());
                VirtualRectangle wr = win.getWindowRectangle();
                wr.setDimensions(wr.getDimensions() / div);
                wr.setLeftTopPosition((wr.getLeftTop()/div) + stPt);
                wr.setBrush(brushController->getBrush("red"));
                small_windows.push_back(wr);
            }
        }
    };
    externWindowsUpdate();
    frameUpdater->addFunction(externWindowsUpdate);

    arr = Arrow(Array2f(200, 200), Array2f(350, 150), 10, 20);
    arr.setBrush(brushController->getBrush("green"));

    circ = make_shared<VirtualCircle>(Array2f(400, 300), 20, 20, brushController->getBrush("blue"));
    //circ->setBrush(brushController->getBrush("blue"));
    hoverCirc = HoverShape(circ, brushController->getBrush("blue"));
    hoverCirc.setFunctionIn([this]() {
        circ->setBrush(brushController->getBrush("green"));
    });
    hoverCirc.setFunctionOut([this]() {
        circ->setBrush(brushController->getBrush("red"));
    });
}



void AppTest::playPause() {
    if (!inputPlayer->getIsPlaying()) {
        bool isPlaying = inputPlayer->play();
        if(isPlaying) playButton->setLabel(L"Pause");
    }
    else {
        inputPlayer->pause();
        playButton->setLabel(L"Play");
    }
}

VirtualRectangle AppTest::updateObjects() {
    if (!mousePlayer->next()) {
        playButton->setLabel(L"Play");
    }
    return VirtualRectangle();
}

void AppTest::drawFrame(ID2D1HwndRenderTarget* rt) {
    //rect.draw(rt);
    //ds.draw(rt);
    list.draw(rt);
    connRectangle->draw(rt);
    nds.draw(rt);
    sg.draw(rt);
    //drawShapeVir.draw(rt);
    dlm.draw(rt); dlm2.draw(rt); dlm3.draw(rt); dlm4.draw(rt);
    bm.draw(rt);

    arr.draw(rt);

    hoverCirc.fill(rt);
    //circ->fill(rt);

    //virt_line.draw(rt);
    //circ_sta.fill(rt);
    //circ_end.fill(rt);

    //test_tri.fill(rt);
    for (const VirtualRectangle& monitor : monitors) {
        monitor.draw(rt);
    }
    for(const VirtualRectangle& swindow : small_windows) {
        swindow.draw(rt);
    }
    small_window->draw(rt);
    //window_drag.fill(rt);
}

void AppTest::handleKeyDown(unsigned int keyCode, char c) {
    //outputDebugLine(to_string(keyCode));
    //outputDebugLine(c);
    if (c == 'a') {
        outputDebugLine("a pressed");
    }
    if (c == 'r') {
        inputRecorder->start();
    }
    else if (c == 's') {
        inputRecorder->end();
        inputPlayer->setRecord(inputRecorder->getRecordStream());
    }
    else if (c == 'q') {
        //windowDetails->newWindowPosition(Array2f(100, 100));
    }
    else if (c == ' ') {
        playPause();
    }
    //mouseRec->addKey();
    inputRecorder->addKeyDown(keyCode);
}

void AppTest::handleKeyUp(unsigned int keyCode, char c) {
    inputRecorder->addKeyUp(keyCode);
}

void AppTest::handleMouseMove(Array2f position, Vector2f movement) {
    hoverCirc.activate(position);
    //ds.move(movement);
    //shared_ptr<VirtualObject> vr = ds.toVirtualObject();
    //vr->isPointInside(position);
    //Region r = vr->getPointRegion(position);

    Region r = nds.getShape()->getPointRegion(position);
    dlm.setMessage(r.toWString());
    //dlm2.setMessage(array2fToWString(position));

    nds.movement(movement);

    float f = virt_line.side(position);
    wstring s;
    if (f == 0) {
        s = L"on";
    }
    else if (f < 0) {
        s = L"L";
    }
    else {
        s = L"R";
    }
    dlm2.setMessage(s);

    bool has_move_sta = circ_sta.movement(movement);
    if(has_move_sta) virt_line.translateStart(movement);
    bool has_move_end = circ_end.movement(movement);
    if (has_move_end) virt_line.translateEnd(movement);
    //bool has_move_win = window_drag.movement(movement);
    /*if (has_move_win) {
        Vector2f move5 = movement*5;
        VirtualRectangle vr = selectedWindow.getWindowRectangle();
        vr.translate(move5);
        //AttachThreadInput()
        if (selectedWindow.getThreadId() != GetCurrentThreadId()) {
            selectedWindow.attachThread(GetCurrentThreadId(), true);

            selectedWindow.setWindowRectangle(vr);
            selectedWindow.attachThread(GetCurrentThreadId(), false);
            //windowDetails->newWindowPosition(vr.getLeftTop());
        }
    }*/
}

void AppTest::handleLeftMouseDown(Array2f position){    
    list.activate(position);
    nds.handleMouseDown(position);
    //ds.handleMouseDown(position);
    //bm.activate(position);
    numClicked++;
    clickMessage();
    mouseRec->addLeftClickDown();
    inputRecorder->addMouseLeftDown();

    Region r = nds.getShape()->getPointRegion(position);
    connRectangle->setDimensions(Vector2f(10, 200));
    Alignment ali = LEFT;
    if(inputController->isPrintKeyDown(81)) ali = RIGHT;
    if(inputController->isPrintKeyDown(87)) ali = CENTRE;
    dragRect->connect(connRectangle, r, ali);
    
    //window_drag.handleMouseDown(position);

    circ_sta.handleMouseDown(position);
    circ_end.handleMouseDown(position);
}

void AppTest::handleLeftMouseUp(Array2f position) {
    nds.handleMouseUp();
    bm.activate(position);
    mouseRec->addLeftClickUp();
    inputRecorder->addMouseLeftUp();

    //window_drag.handleMouseUp();

    circ_sta.handleMouseUp();
    circ_end.handleMouseUp();
}

void AppTest::handleRightMouseDown(Array2f position) {
    mousePlayer->stop();
}