project(rlImGui)

add_library(rlImGui STATIC rlImGui.cpp)

# include imgui.h
target_include_directories(rlImGui PUBLIC ${CMAKE_SOURCE_DIR}/lib/imgui/imgui)