add_library(include STATIC
    fgl_matrix.c
    fgl_transform.c
    fgl_vertex.c
    rl_draw_capsule.c
)

target_include_directories(include PUBLIC ${CMAKE_SOURCE_DIR}/lib/libfixmath/libfixmath)
target_link_libraries(include PUBLIC libfixmath)

target_include_directories(include PUBLIC ${CMAKE_SOURCE_DIR}/lib/raylib/src)
target_link_libraries(include PUBLIC raylib)