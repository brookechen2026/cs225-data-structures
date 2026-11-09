# Path definitions.
set(lib_dir ${CMAKE_CURRENT_SOURCE_DIR})

# Add colorspace library.
set(colorspace_dir ${lib_dir}/colorspace)
file(GLOB_RECURSE colorspace_sources CONFIGURE_DEPENDS ${colorspace_dir}/*.cpp)
add_library(colorspace ${colorspace_sources})
set_target_properties(colorspace PROPERTIES LINKER_LANGUAGE CXX)