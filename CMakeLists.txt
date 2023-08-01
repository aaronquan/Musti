# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Musti")

#find_package(Eigen3 CONFIG REQUIRED)

#target_link_libraries(window PRIVATE Eigen3::Eigen)

# Include sub-projects.
add_subdirectory("Musti")
add_subdirectory("Window")
add_subdirectory("Geometry")
add_subdirectory("InterfaceElements")
