/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma pnce

namespace nar {
   class Object {
     public:
      Object(std::string_view type)
          : type_(type) {
      }
      Object() = delete;

      std::string_view type() {
         return type_;
      }

     private:
      std::string_view type_;
   };
}
