/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

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