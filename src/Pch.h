/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once

#include <array>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <stdarg.h>
#include <stddef.h>
#include <cmath>

/**
 * Platform Headers
 */
#ifdef XR_USE_PLATFORM_ANDROID
#include <android/log.h>
#include <android/native_window.h>
#include <android_native_app_glue.h>
#include <jni.h>
#include <sys/system_properties.h>
#endif

/**
 * Graphics Headers
 */
#include <EGL/egl.h>

/**
 * OpenXR Headers
 */
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <openxr/openxr_reflection.h>

/**
 * Narradia Headers
 */
#include "common/class_Singleton.h"
#include "engine/func_Utilities.h"
#include "matter/struct_Vertex.h"
#include "matter/class_VertexCube.h"
#include "matter/struct_Point.h"
#include "matter/const_Colors.h"