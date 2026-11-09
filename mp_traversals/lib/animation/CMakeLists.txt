# Path definitions.
set(lib_dir ${CMAKE_CURRENT_SOURCE_DIR})

# Add animation library.
set(animation_dir ${lib_dir}/animation)
file(GLOB_RECURSE animation_sources CONFIGURE_DEPENDS ${animation_dir}/*.cpp)
add_library(animation ${animation_sources})
set_target_properties(animation PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(animation INTERFACE ${lib_dir})


target_link_libraries(animation PRIVATE png)