add_library(window 
FullWindow.cpp 
Util.cpp 
BaseWindow.cpp
AdvancedWindow.cpp
WindowProperties.cpp
Controllers/BrushController.cpp
Controllers/FontController.cpp 
Controllers/InputController.cpp 
Recorders/InputRecorder.cpp 
Recorders/TimerManager.cpp
Recorders/Updater.cpp)

#set(CMAKE_PREFIX_PATH "D:/Coding/C++/vcpkg/installed/x86-windows/include")
#set(Eigen3_DIR "D:/Coding/C++/Libraries/eigen-3.4.0/cmake")

#find_package(fmt)
find_package(Eigen3 REQUIRED NO_MODULE)
#include_directories(D:/Coding/C++/vcpkg/installed/x86-windows/include)
if (TARGET Eigen3::Eigen)
  # Use the imported target
  target_link_libraries(window Eigen3::Eigen)
endif (TARGET Eigen3::Eigen)

install(TARGETS window DESTINATION lib)

install(FILES 
FullWindow.h Util.h 
BaseWindow.h 
AdvancedWindow.h
WindowProperties.h
DESTINATION include/Window)

install(FILES
Controllers/BrushController.h
Controllers/FontController.h
Controllers/InputController.h
DESTINATION include/Window/Controllers)

install(FILES 
Recorders/InputRecorder.h
Recorders/TimerManager.h
Recorders/Updater.h
DESTINATION include/Window/Recorders)