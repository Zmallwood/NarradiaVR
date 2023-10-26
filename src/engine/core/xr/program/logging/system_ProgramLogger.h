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