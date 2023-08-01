# CMakeList.txt : CMake project for Musti, include source and define
# project specific logic here.
#
  
# Add source to this project's executable.
add_executable (${PROJECT_NAME} WIN32 "Musti.cpp" "Musti.h" "CustomWindow.h" "CustomWindow.cpp")

#target_include_directories(${PROJECT_NAME} PUBLIC ../out/install/x86-debug/include)

target_include_directories(${PROJECT_NAME} PUBLIC ../Window)

target_link_libraries(${PROJECT_NAME} PRIVATE window)

target_include_directories(${PROJECT_NAME} PUBLIC ../Geometry)

target_link_libraries(${PROJECT_NAME} PRIVATE geometry)

target_include_directories(${PROJECT_NAME} PUBLIC ../InterfaceElements)

target_link_libraries(${PROJECT_NAME} PRIVATE interface_elements)

#set(CMAKE_PREFIX_PATH "D:/Coding/C++/vcpkg/installed/x86-windows/share")
find_package(Eigen3 REQUIRED NO_MODULE)
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)
#if (TARGET Eigen3::Eigen)
  # Use the imported target
  #target_link_libraries(${PROJECT_NAME} PUBLIC Eigen3::Eigen)
#endif (TARGET Eigen3::Eigen)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Musti PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
