#pragma once
#include "MapArea.h"

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