#include "Pch.h"
#include "Logger.h"

#include <sstream>

#if defined(ANDROID)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, "hello_xr", __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "hello_xr", __VA_ARGS__)
#endif

namespace nar {
  namespace {
    Log::Level gMinSeverity{Log::Level::Info};
    std::mutex gLogLock;
  }

  namespace Log {
    void SetLevel(Level min_severity) {
      gMinSeverity = min_severity;
    }

    void Write(Level severity, const std::string &msg) {
      if (severity < gMinSeverity)
        return;

      const auto now = std::chrono::system_clock::now();
      const time_t now_time = std::chrono::system_clock::to_time_t(now);
      tm now_tm;
#ifdef _WIN32
      localtime_s(&now_tm, &now_time);
#else
      localtime_r(&now_time, &now_tm);
#endif
      // time_t only has second precision. Use the rounding error to get sub-second precision.
      const auto second_remained = now - std::chrono::system_clock::from_time_t(now_time);
      const int64_t milliseconds =
          std::chrono::duration_cast<std::chrono::milliseconds>(second_remained).count();

      static std::map<Level, const char *> severity_name = {
          {Level::Verbose, "Verbose"},
          {Level::Info, "Info   "},
          {Level::Warning, "Warning"},
          {Level::Error, "Error  "}};

      std::ostringstream out;
      out.fill('0');
      out << "[" << std::setw(2) << now_tm.tm_hour << ":" << std::setw(2) << now_tm.tm_min << ":"
          << std::setw(2) << now_tm.tm_sec << "." << std::setw(3) << milliseconds << "]"
          << "[" << severity_name[severity] << "] " << msg << std::endl;

      std::lock_guard<std::mutex> lock(gLogLock); // Ensure output is serialized
      ((severity == Level::Error) ? std::clog : std::cout) << out.str();
#if defined(_WIN32)
      OutputDebugStringA(out.str().c_str());
#endif
#if defined(ANDROID)
      if (severity == Level::Error)
        ALOGE("%s", out.str().c_str());
      else
        ALOGV("%s", out.str().c_str());
#endif
    }
  }
}