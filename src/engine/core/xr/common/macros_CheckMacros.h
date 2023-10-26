#pragma once

#define CHK_STRINGIFY(x) #x
#define TOSTRING(x) CHK_STRINGIFY(x)
#define FILE_AND_LINE __FILE__ ":" TOSTRING(__LINE__)

#define CHECK(exp)                                                                                 \
   {                                                                                               \
      if (!(exp))                                                                                  \
         Throw("Check failed", #exp, FILE_AND_LINE);                                               \
   }
#define CHECK_XRCMD(cmd) CheckXrResult(cmd, #cmd, FILE_AND_LINE);
