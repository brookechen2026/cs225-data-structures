# Path definitions.
set(lib_dir ${CMAKE_CURRENT_SOURCE_DIR})

# Add graph library.
set(graph_dir ${lib_dir}/graph)
file(GLOB_RECURSE graph_sources CONFIGURE_DEPENDS ${graph_dir}/*.cpp)
add_library(graph ${graph_sources})
set_target_properties(graph PROPERTIES LINKER_LANGUAGE CXX)