/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "MapArea.h"
#include <memory>

namespace nar {
   class World : public Singleton<World> {
     public:
      std::shared_ptr<MapArea> current_map_area() {
         return current_map_area_;
      }

     private:
      std::shared_ptr<MapArea> current_map_area_ = std::make_shared<MapArea>();
   };
}
