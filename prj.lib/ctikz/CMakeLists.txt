add_library(ctikz ctikz.cpp include/ctikz/ctikz.hpp)
set_property(TARGET ctikz PROPERTY CXX_STANDARD 20)
target_include_directories(ctikz PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# if(OPENCV_DEFINE)
#   target_link_libraries(ctikz opencv::opencv)
#   target_compile_definitions(ctikz PRIVATE "OPENCV_DEFINE")
#  endif(OPENCV_DEFINE)

install(TARGETS ctikz)