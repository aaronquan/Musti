add_library(geometry 
DrawShapes.cpp 
Virtuals.cpp 
Containers.cpp 
Util.cpp 
Primatives.cpp 
DrawVirtuals.cpp
)

find_package(Eigen3 REQUIRED NO_MODULE)
if (TARGET Eigen3::Eigen)
  # Use the imported target
  target_link_libraries(geometry Eigen3::Eigen)
endif (TARGET Eigen3::Eigen)

install(TARGETS geometry DESTINATION lib)

install(FILES 
DrawShapes.h
Virtuals.h
Containers.h
Util.h
Primatives.h
DrawVirtuals.h
DESTINATION include/Geometry)