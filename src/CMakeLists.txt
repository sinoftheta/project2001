cmake_minimum_required(VERSION 3.2)

add_executable(main 
    main.cpp
    # add all sources here
)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/lib/gca-plus/src)# include directory with the library header files
target_link_libraries(main PUBLIC gcaplus)# link the library (use project name of the lib)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/lib/ggpo/src/lib/ggpo)
target_link_libraries(main PUBLIC GGPO)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/lib/raylib/src)
target_link_libraries(main PUBLIC raylib)