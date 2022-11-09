
add_executable(main 
    # c files in src
    main.cpp
    p2k1_capsule_collision.c
    p2k1_game_state.c
)

# include the headers in src/include TODO: refactor this out
#target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
#target_link_libraries(main PUBLIC include)
#add_subdirectory(include)


# TODO: change these to private

# include library headers
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/lib/gca-plus/src) # include directory with the library header files
target_link_libraries(main PUBLIC gcaplus) # link the library (use project name of the lib)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/lib/ggpo/src/lib/ggpo)
target_link_libraries(main PUBLIC GGPO)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/lib/raylib/src)
target_link_libraries(main PUBLIC raylib)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/lib/libfixmath/libfixmath)
target_link_libraries(main PUBLIC libfixmath)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/lib/imgui/imgui)
target_link_libraries(main PUBLIC imgui)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/lib/rlImGui)
target_link_libraries(main PUBLIC rlImGui)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/lib/fgl/include)
target_link_libraries(main PUBLIC fgl)

