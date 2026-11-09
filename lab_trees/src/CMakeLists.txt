# Path definitions.
set(src_dir ${CMAKE_CURRENT_SOURCE_DIR})

# Add overall src library.
file(GLOB_RECURSE src_sources CONFIGURE_DEPENDS ${src_dir}/*.cpp ${src_dir}/*.hpp)
add_library(src ${src_sources})
set_target_properties(src PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(src PUBLIC ${src_dir})
target_link_libraries(src PRIVATE libs)
