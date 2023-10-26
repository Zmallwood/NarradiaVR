/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once

namespace nar {
   class ProgramLogger : public Singleton<ProgramLogger> {
     public:
      void LogInstanceInfo();
      void LogViewConfigurations();
      void LogReferenceSpaces();
      void LogActionSourceName(XrAction action, const std::string &action_name) const;
      static void LogLayersAndExtensions();

     private:
      void LogEnvironmentBlendMode(XrViewConfigurationType type);
   };
}