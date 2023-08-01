add_library(interface_elements 
BasicElements.cpp 
ElementContainer.cpp 
SubElement.cpp)

#target_include_directories(interface_elements PUBLIC ../Geometry)
#target_link_libraries(interface_elements PRIVATE geometry)

find_package(Eigen3 REQUIRED NO_MODULE)
target_link_libraries(interface_elements PRIVATE Eigen3::Eigen)

install(TARGETS interface_elements DESTINATION lib)

install(FILES 
BasicElements.h
ElementContainer.h
SubElement.h
DESTINATION include/InterfaceElements)