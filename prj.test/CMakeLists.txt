add_executable(hit_or_miss.test hit_or_miss.test.cpp "hit_or_miss.test.cpp")
target_link_libraries(hit_or_miss.test hitOrMiss ctikz)
add_test(NAME hit_or_miss.test COMMAND hit_or_miss.test)


install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE} DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../tests DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../doc_doxygen DESTINATION ./bin.rel)