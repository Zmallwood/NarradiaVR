#include "Pch.h"
#include "ModelPart.h"

namespace nar
{
    ModelPart::ModelPart()
        : timeline(std::make_unique<ModelPartTimeline>()) {
    }
}