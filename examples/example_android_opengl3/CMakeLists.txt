cmake_minimum_required(VERSION 3.6)

project(ImGuiHook)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- 1. Define xhook as a STATIC library ---

# Find all the .c source files in the xhook directory
file(GLOB XHOOK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/xhook/*.c")

# Create a static library target for xhook from its source files.
# A STATIC library is a .a archive file whose code will be embedded
# into any library that links against it.
add_library(xhook STATIC ${XHOOK_SOURCES})

# Specify the include directory for xhook.
# By setting it as PUBLIC, any other library that links to xhook
# will automatically inherit this include path.
target_include_directories(xhook PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/xhook)


# --- 2. Define your main SHARED library ---

add_library(${CMAKE_PROJECT_NAME} SHARED
  # Your main C++ code
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp

  # ImGui source files
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui_widgets.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../backends/imgui_impl_android.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../backends/imgui_impl_opengl3.cpp
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
  IMGUI_IMPL_OPENGL_ES3
)

# We only need the ImGui include paths here now, because the xhook path
# will be inherited automatically from the xhook library target.
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  ${CMAKE_CURRENT_SOURCE_DIR}/../../backends
)

# --- 3. Link your main library against the static xhook library and Android libs ---

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
  # This is the "static link" step. It tells CMake to embed the
  # compiled xhook library directly into your final .so file.
  xhook
  
  # Standard Android libraries
  android
  EGL
  GLESv3
  log
)