# Path definitions.
set(lib_dir ${CMAKE_CURRENT_SOURCE_DIR})

# Add binarytree library.
set(binarytree_dir ${lib_dir}/binarytree)
file(GLOB_RECURSE cs225_sources CONFIGURE_DEPENDS ${binarytree_dir}/*.cpp)
add_library(binarytree ${cs225_sources})
set_target_properties(binarytree PROPERTIES LINKER_LANGUAGE CXX)