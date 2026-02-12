file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.hpp)

FetchContent_GetProperties(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

# Compile commads for lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ImGui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp

    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

target_include_directories(ImGui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)
target_link_libraries(ImGui
        PUBLIC
        glfw
        Vulkan::Vulkan
)

target_include_directories(ImGui PUBLIC ${IMGUI_DIR})


add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
      PUBLIC
          ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog glm Taskflow ImGui)
