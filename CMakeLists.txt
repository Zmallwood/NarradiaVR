# Copyright (c) 2017-2023, The Khronos Group Inc.
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wfatal-errors -std=c++20")

file(GLOB_RECURSE LOCAL_HEADERS "src/cpp/**.h")
file(GLOB_RECURSE LOCAL_SOURCE "src/cpp/**.cpp")
file(GLOB_RECURSE VULKAN_SHADERS "src/cpp/vulkan_shaders/**.glsl")

list(FILTER LOCAL_SOURCE EXCLUDE REGEX ".*inc.cpp$")

# For including compiled shaders
include_directories(${CMAKE_CURRENT_BINARY_DIR} "src/cpp" "src/external")


if(ANDROID)
    add_library(hello_xr MODULE
        ${LOCAL_SOURCE}
        ${LOCAL_HEADERS}
        ${VULKAN_SHADERS}
        $<TARGET_OBJECTS:android_native_app_glue>)

    target_link_libraries(hello_xr ${ANDROID_LIBRARY} ${ANDROID_LOG_LIBRARY} ${CMAKE_CURRENT_SOURCE_DIR}/lib/libpng.a ${CMAKE_CURRENT_SOURCE_DIR}/lib/libassimp.a ${CMAKE_CURRENT_SOURCE_DIR}/lib/libandroid_jniiosystem.a z)

    # Only for Android because it lacks a command line
    set(HELLOXR_DEFAULT_GRAPHICS_PLUGIN "OpenGLES" CACHE STRING "Which graphics plugin should be used by default for Hello XR artifacts?")
    # These are the recognized options: provide them as a helper in the CMake GUI
    set_property(CACHE HELLOXR_DEFAULT_GRAPHICS_PLUGIN PROPERTY STRINGS "OpenGLES" "Vulkan")

    # Now handle the value, normally provided by Gradle
    if(HELLOXR_DEFAULT_GRAPHICS_PLUGIN STREQUAL "OpenGLES")
        message(STATUS "hello_xr will default to OpenGL ES graphics plugin")
        target_compile_definitions(hello_xr PRIVATE DEFAULT_GRAPHICS_PLUGIN_OPENGLES)
    elseif(HELLOXR_DEFAULT_GRAPHICS_PLUGIN STREQUAL "Vulkan")
        message(STATUS "hello_xr will default to Vulkan graphics plugin")
        target_compile_definitions(hello_xr PRIVATE DEFAULT_GRAPHICS_PLUGIN_VULKAN)
    endif()

else()
    add_executable(hello_xr
        ${LOCAL_SOURCE}
        ${LOCAL_HEADERS}
        ${VULKAN_SHADERS})
endif()
set_target_properties(hello_xr PROPERTIES FOLDER ${SAMPLES_FOLDER})


source_group("Headers" FILES ${LOCAL_HEADERS})
source_group("Shaders" FILES ${VULKAN_SHADERS})

#compile_glsl(run_hello_xr_glsl_compiles ${VULKAN_SHADERS})

add_dependencies(hello_xr
    generate_openxr_header
    #run_hello_xr_glsl_compiles
)

target_include_directories(hello_xr
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/common
    ${PROJECT_SOURCE_DIR}/src/tests/hello_xr/vulkan_shaders

    # for helper headers
    ${PROJECT_SOURCE_DIR}/external/include
)

if(GLSLANG_VALIDATOR AND NOT GLSL_COMPILER)
    target_compile_definitions(hello_xr PRIVATE USE_GLSLANGVALIDATOR)
endif()

if(Vulkan_FOUND)
    target_include_directories(hello_xr
        PRIVATE
        ${Vulkan_INCLUDE_DIRS}
    )
endif()

target_link_libraries(hello_xr OpenXR::openxr_loader)
if(TARGET openxr-gfxwrapper)
    target_link_libraries(hello_xr openxr-gfxwrapper)
endif()
if(WIN32)
    if(MSVC)
        target_compile_definitions(hello_xr PRIVATE _CRT_SECURE_NO_WARNINGS)
        target_compile_options(hello_xr PRIVATE /Zc:wchar_t /Zc:forScope /W4)
        if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            # If actually msvc and not clang-cl
            target_compile_options(openxr_c_compile_test PRIVATE /WX)
        endif()
    endif()
    target_link_libraries(hello_xr ole32)
    if(MSVC)
        # Right now can't build this on MinGW because of directxcolors, directxmath, etc.
        target_link_libraries(hello_xr d3d11 d3d12 d3dcompiler dxgi)
    endif()
endif()

if(Vulkan_LIBRARY)
    target_link_libraries(hello_xr ${Vulkan_LIBRARY})
endif()

if(NOT ANDROID)
    install(TARGETS hello_xr
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT hello_xr)
    if(NOT WIN32)
        install(FILES hello_xr.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1/ COMPONENT ManPages)
    endif()
endif()


set(PCH_FILES
<array>
<exception>
<functional>
<iomanip>
<iostream>
<list>
<locale>
<map>
<memory>
<numeric>
<set>
<string>
<thread>
<vector>
<algorithm>
<stdarg.h>
<stddef.h>
<cmath>

<android/log.h>
<android/native_window.h>
<android_native_app_glue.h>
<jni.h>
<sys/system_properties.h>

<EGL/egl.h>

<openxr/openxr.h>
<openxr/openxr_platform.h>
<openxr/openxr_reflection.h>

"../../common/gfxwrapper_opengl.h"
"../../common/xr_linear.h"

"src/external/libpng/png.h"
"src/external/glm/glm.hpp"
"src/external/glm/gtc/matrix_transform.hpp"
"src/external/glm/gtc/type_ptr.hpp"
"src/external/glm/ext/matrix_projection.hpp"
"src/external/glm/gtx/closest_point.hpp"
"src/external/assimp/Importer.hpp"
"src/external/assimp/scene.h"
"src/external/assimp/port/AndroidJNI/AndroidJNIIOSystem.h"

"src/cpp/common/system/Typedefs.h"
"src/cpp/common/system/func/SingletonsCleanup.h"
"src/cpp/common/system/Singleton.h"
"src/cpp/matter/model/Color.h"
"src/cpp/matter/model/PointF.h"
"src/cpp/matter/model/Point3F.h"
"src/cpp/matter/model/Vertex3F.h"
"src/cpp/matter/model/Vertex.h"
"src/cpp/matter/model/VertexCube.h"
"src/cpp/matter/model/Point.h"
"src/cpp/matter/model/Colors.h"
"src/cpp/utilities/model/Util.h"
"src/cpp/utilities/view/UtilView.h"
)

target_precompile_headers(hello_xr PRIVATE ${PCH_FILES})
