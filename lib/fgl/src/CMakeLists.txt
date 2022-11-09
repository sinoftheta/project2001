project(fgl)

add_library(fgl STATIC
    fgl_matrix.c
    fgl_skeleton.c
    fgl_transform.c
    fgl_vertex.c
)

target_include_directories(fgl PUBLIC ${CMAKE_SOURCE_DIR}/lib/libfixmath/libfixmath)
target_link_libraries(fgl PUBLIC libfixmath)
