#pragma once

namespace nar {
   class MapGenerator : public Singleton<MapGenerator> {
     public:
      void GenerateMapArea();
   };
}