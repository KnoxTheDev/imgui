cmake_minimum_required(VERSION 3.6)

project(ImGuiExample)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Specify supported architectures
set(CMAKE_ANDROID_ARCH_ABI arm64-v8a armeabi-v7a x86 x86_64)

# Add the source files for ImGui and Android backend
add_library(${CMAKE_PROJECT_NAME} SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/themes.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../imgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../imgui_widgets.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_android.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_android.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../backends/imgui_impl_opengl3.cpp
  ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

# Specify linker flags (ensure ANativeActivity onCreate symbol is used)
set(CMAKE_SHARED_LINKER_FLAGS
  "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate"
)

# Add preprocessor definitions
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
  IMGUI_IMPL_OPENGL_ES3
)

# Include directories for ImGui, Android native glue, and OpenGL
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../..          # Path to ImGui source files
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../backends # Path to ImGui backend files
  ${ANDROID_NDK}/sources/android/native_app_glue # Path to Android native app glue
)

# Link required libraries (OpenGL, EGL, Android, etc.)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
  android
  EGL
  GLESv3
  log
)
