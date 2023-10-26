#pragma once

namespace nar {
   class Loader : public Singleton<Loader> {
     public:
      void Init();
   };
}