/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "ModelPart.h"

namespace nar
{
    ModelPart::ModelPart()
        : timeline(std::make_unique<ModelPartTimeline>()) {
    }
}
