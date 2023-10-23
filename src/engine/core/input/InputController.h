#pragma once

namespace nar {
  class InputController {
   public:
    bool HasBeenGrabbedPickResult();
    void RegisterBeenGrabbed(bool value);
    void RegisterNotGrabbed();

   private:
    bool is_grabbed_ = false;
    bool has_been_grabbed_ = false;
  };
}