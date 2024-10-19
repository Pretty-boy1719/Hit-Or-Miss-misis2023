add_library(hitOrMiss hit_or_miss.cpp include/hitOrMiss/hit_or_miss.hpp)
set_property(TARGET hitOrMiss PROPERTY CXX_STANDART 20)
target_include_directories(hitOrMiss PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
install(TARGETS hitOrMiss)

target_link_libraries(hitOrMiss ${OpenCV_LIBS})
add_subdirectory(ctikz)