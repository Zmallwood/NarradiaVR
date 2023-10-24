#pragma once
#include "core/xr/options/Options.h"

namespace nar {
  struct Options;

  class OptionsManager : public Singleton<OptionsManager> {
   public:
    bool Init();

    std::shared_ptr<Options> options() {
      return options_;
    }

   private:
    bool UpdateOptionsFromSystemProperties();

    std::shared_ptr<Options> options_ = std::make_shared<Options>();
  };
}